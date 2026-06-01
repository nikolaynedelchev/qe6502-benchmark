#include "applewin_toolbox/applewin_cpu.hpp"

#include <cassert>
#include <cstdint>
#include <cstring>

using BYTE = std::uint8_t;
using WORD = std::uint16_t;
using USHORT = std::uint16_t;
using UINT = std::uint32_t;
using UINT32 = std::uint32_t;
using ULONG = std::uint32_t;
using BOOL = int;
using LPBYTE = BYTE*;
using LPWORD = WORD*;

#define _ASSERT(x) assert(x)
#define __forceinline inline
#define __stdcall

#include "MemoryDefs.h"

enum {
    AF_SIGN = 0x80,
    AF_OVERFLOW = 0x40,
    AF_RESERVED = 0x20,
    AF_BREAK = 0x10,
    AF_DECIMAL = 0x08,
    AF_INTERRUPT = 0x04,
    AF_ZERO = 0x02,
    AF_CARRY = 0x01
};

enum eCpuType { CPU_UNKNOWN = 0, CPU_6502 = 1, CPU_65C02, CPU_Z80 };

enum { MEM_Normal = 0, MEM_IORead, MEM_FloatingBus, MEM_NoSlotClock };
using iofunction = BYTE (__stdcall *)(WORD nPC, WORD nAddr, BYTE nWriteFlag, BYTE nWriteValue, ULONG nExecutedCycles);

struct regsrec {
    BYTE a;
    BYTE x;
    BYTE y;
    BYTE ps;
    WORD pc;
    WORD sp;
    BYTE bJammed;
};

regsrec regs{};
unsigned long long g_nCumulativeCycles = 0;
static eCpuType g_main_cpu = CPU_6502;
static eCpuType g_active_cpu = CPU_6502;
static bool g_irq_asserted = false;
static bool g_nmi_pending = false;
static BYTE ram[65536];
static BYTE dirty[256];
LPBYTE mem = ram;
LPBYTE memdirty = dirty;
LPBYTE memVidHD = nullptr;
LPBYTE memshadow[0x100];
LPBYTE memwrite[0x100];
BYTE memreadPageType[0x100];
iofunction IORead[256];
iofunction IOWrite[256];

static BYTE __stdcall io_stub(WORD, WORD address, BYTE write, BYTE value, ULONG) {
    if(write) ram[address] = value;
    return ram[address];
}

static void initialise_pages() {
    for(unsigned i = 0; i < 0x100; ++i) {
        memshadow[i] = ram + (i << 8);
        memwrite[i] = ram + (i << 8);
        memreadPageType[i] = MEM_Normal;
        IORead[i] = io_stub;
        IOWrite[i] = io_stub;
        dirty[i] = 0;
    }
}

static __forceinline WORD read_word(WORD address) {
    return static_cast<WORD>(ram[address] | (static_cast<WORD>(ram[(address + 1u) & 0xffffu]) << 8));
}

static __forceinline void Fetch(BYTE& iOpcode, ULONG) {
    iOpcode = ram[regs.pc++];
}

static eCpuType GetActiveCpu() { return g_active_cpu; }
static eCpuType GetMainCpu() { return g_main_cpu; }
static BYTE MemReadFloatingBus(const ULONG) { return 0; }
static BYTE IO_F8xx(WORD, WORD address, BYTE write, BYTE value, ULONG) { return io_stub(0, address, write, value, 0); }
static int z80_mainloop(int, int) { return 0; }

#define HEATMAP_X(address)
#define READ(addr) _READ_WITH_IO_F8xx(addr)
#define WRITE(value) _WRITE_WITH_IO_F8xx(value)

#include "CPU/cpu_general.inl"
#include "CPU/cpu_instructions.inl"

static __forceinline void push_interrupt_state(BOOL& flagc, BOOL& flagn, BOOL& flagv, BOOL& flagz, WORD vector) {
    _PUSH(regs.pc >> 8)
    _PUSH(regs.pc & 0xff)
    EF_TO_AF
    _PUSH(regs.ps & ~AF_BREAK)
    regs.ps |= AF_INTERRUPT;
    if(GetMainCpu() == CPU_65C02) {
        regs.ps &= ~AF_DECIMAL;
    }
    regs.pc = read_word(vector);
}

static __forceinline bool NMI(ULONG& uExecutedCycles, BOOL& flagc, BOOL& flagn, BOOL& flagv, BOOL& flagz) {
    if(!g_nmi_pending) {
        return false;
    }

    g_nmi_pending = false;
    push_interrupt_state(flagc, flagn, flagv, flagz, _6502_NMI_VECTOR);
    UINT uExtraCycles = 0;
    CYC(7)
    return true;
}

static __forceinline bool IRQ(ULONG& uExecutedCycles, BOOL& flagc, BOOL& flagn, BOOL& flagv, BOOL& flagz) {
    if(!g_irq_asserted || (regs.ps & AF_INTERRUPT)) {
        return false;
    }

    push_interrupt_state(flagc, flagn, flagv, flagz, _6502_INTERRUPT_VECTOR);
    UINT uExtraCycles = 0;
    CYC(7)
    return true;
}

static __forceinline void CheckSynchronousInterruptSources(UINT, ULONG) {}
static __forceinline void DoIrqProfiling(std::uint32_t) {}
static __forceinline void NTSC_VideoUpdateCycles(ULONG) {}

#include "CPU/cpu6502.h"

#define READ(addr) _READ_WITH_IO_F8xx(addr)
#define WRITE(value) _WRITE_WITH_IO_F8xx(value)
#include "CPU/cpu65C02.h"

#undef HEATMAP_X

namespace applewin_toolbox {
namespace {

eCpuType to_cpu_type(CpuMode mode) {
    return mode == CpuMode::cmos65c02 ? CPU_65C02 : CPU_6502;
}

CpuMode from_cpu_type(eCpuType type) {
    return type == CPU_65C02 ? CpuMode::cmos65c02 : CpuMode::nmos6502;
}

void set_mode(CpuMode mode) {
    g_main_cpu = to_cpu_type(mode);
    g_active_cpu = g_main_cpu;
}

} // namespace

void reset_memory() {
    std::memset(ram, 0, sizeof(ram));
    initialise_pages();
}

std::uint8_t* memory() {
    return ram;
}

const std::uint8_t* memory_data() {
    return ram;
}

std::uint8_t read_memory(std::uint16_t address) {
    return ram[address];
}

void write_memory(std::uint16_t address, std::uint8_t value) {
    ram[address] = value;
}

void load_program(std::uint16_t address, const std::uint8_t* data, std::uint32_t size) {
    for(std::uint32_t i = 0; i < size; ++i) {
        ram[(address + i) & 0xffffu] = data[i];
    }
}

void set_vector(std::uint16_t vector_address, std::uint16_t target_address) {
    ram[vector_address] = static_cast<std::uint8_t>(target_address & 0xffu);
    ram[(vector_address + 1u) & 0xffffu] = static_cast<std::uint8_t>(target_address >> 8);
}

void reset_cpu(std::uint16_t pc, CpuMode mode) {
    set_mode(mode);
    regs.a = 0;
    regs.x = 0;
    regs.y = 0;
    regs.ps = AF_RESERVED | AF_INTERRUPT;
    regs.pc = pc;
    regs.sp = 0x01fd;
    regs.bJammed = 0;
    g_irq_asserted = false;
    g_nmi_pending = false;
    g_nCumulativeCycles = 0;
}

void reset_cpu(std::uint16_t pc, bool cmos) {
    reset_cpu(pc, cmos ? CpuMode::cmos65c02 : CpuMode::nmos6502);
}

void reset_from_vector(CpuMode mode) {
    reset_cpu(read_word(_6502_RESET_VECTOR), mode);
}

void set_state(const State& state) {
    set_mode(state.mode);
    regs.a = state.a;
    regs.x = state.x;
    regs.y = state.y;
    regs.ps = state.p;
    regs.pc = state.pc;
    regs.sp = state.sp;
    regs.bJammed = state.jammed ? 1 : 0;
}

State state() {
    return State{regs.a, regs.x, regs.y, regs.ps, regs.pc, regs.sp, regs.bJammed != 0, from_cpu_type(g_main_cpu)};
}

void set_a(std::uint8_t value) { regs.a = value; }
void set_x(std::uint8_t value) { regs.x = value; }
void set_y(std::uint8_t value) { regs.y = value; }
void set_p(std::uint8_t value) { regs.ps = value; }
void set_pc(std::uint16_t value) { regs.pc = value; }
void set_sp(std::uint16_t value) { regs.sp = value; }
void set_jammed(bool value) { regs.bJammed = value ? 1 : 0; }

std::uint8_t a() { return regs.a; }
std::uint8_t x() { return regs.x; }
std::uint8_t y() { return regs.y; }
std::uint8_t p() { return regs.ps; }
std::uint16_t pc() { return regs.pc; }
std::uint16_t sp() { return regs.sp; }
bool jammed() { return regs.bJammed != 0; }
CpuMode mode() { return from_cpu_type(g_main_cpu); }

void set_irq_line(bool asserted) {
    g_irq_asserted = asserted;
}

void pulse_nmi() {
    g_nmi_pending = true;
}

void clear_interrupt_lines() {
    g_irq_asserted = false;
    g_nmi_pending = false;
}

bool irq_line() {
    return g_irq_asserted;
}

bool nmi_pending() {
    return g_nmi_pending;
}

std::uint32_t execute(std::uint32_t cycles) {
    const auto executed = (g_main_cpu == CPU_65C02)
        ? Cpu65C02(cycles, false)
        : Cpu6502(cycles, false);
    g_nCumulativeCycles += executed;
    return executed;
}

std::uint64_t cumulative_cycles() {
    return g_nCumulativeCycles;
}

void clear_cumulative_cycles() {
    g_nCumulativeCycles = 0;
}

} // namespace applewin_toolbox
