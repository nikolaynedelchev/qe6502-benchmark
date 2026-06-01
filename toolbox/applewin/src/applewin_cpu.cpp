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

namespace applewin_toolbox::detail {
Registers registers{};
CpuMode current_mode = CpuMode::nmos6502;
bool irq_asserted = false;
bool nmi_pending_flag = false;
std::uint64_t cumulative_cycles_counter = 0;
} // namespace applewin_toolbox::detail

using regsrec = applewin_toolbox::detail::Registers;
regsrec& regs = applewin_toolbox::detail::registers;
static BYTE dirty[256];
LPBYTE mem = nullptr;
LPBYTE memdirty = dirty;
LPBYTE memVidHD = nullptr;
LPBYTE memshadow[0x100];
LPBYTE memwrite[0x100];
BYTE memreadPageType[0x100];
iofunction IORead[256];
iofunction IOWrite[256];

static BYTE __stdcall io_stub(WORD, WORD address, BYTE write, BYTE value, ULONG) {
    if(write) mem[address] = value;
    return mem[address];
}

// The benchmark extraction deliberately bypasses Apple II memory-mapped I/O
// and ROM/soft-switch special cases for CPU correctness tests. The AppleWin
// opcode core still inlines memory access through READ/WRITE macros, but these
// definitions make those accesses a flat 64K RAM model.

static void initialise_pages() {
    for(unsigned i = 0; i < 0x100; ++i) {
        memshadow[i] = mem + (i << 8);
        memwrite[i] = mem + (i << 8);
        memreadPageType[i] = MEM_Normal;
        IORead[i] = io_stub;
        IOWrite[i] = io_stub;
        dirty[i] = 0;
    }
}

static __forceinline WORD read_word(WORD address) {
    return static_cast<WORD>(mem[address] | (static_cast<WORD>(mem[(address + 1u) & 0xffffu]) << 8));
}

static __forceinline void Fetch(BYTE& iOpcode, ULONG) {
    iOpcode = mem[regs.pc++];
}

static eCpuType to_cpu_type(applewin_toolbox::CpuMode mode) { return mode == applewin_toolbox::CpuMode::cmos65c02 ? CPU_65C02 : CPU_6502; }
static eCpuType GetActiveCpu() { return to_cpu_type(applewin_toolbox::detail::current_mode); }
static eCpuType GetMainCpu() { return to_cpu_type(applewin_toolbox::detail::current_mode); }
static BYTE MemReadFloatingBus(const ULONG) { return 0; }
static BYTE IO_F8xx(WORD, WORD address, BYTE write, BYTE value, ULONG) { return io_stub(0, address, write, value, 0); }
static int z80_mainloop(int, int) { return 0; }

#define HEATMAP_X(address)
#define READ(addr) (mem[static_cast<WORD>(addr)])
#define WRITE(value) do { mem[addr] = static_cast<BYTE>(value); dirty[addr >> 8] = 0xff; } while(0);

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
    if(!applewin_toolbox::detail::nmi_pending_flag) {
        return false;
    }

    applewin_toolbox::detail::nmi_pending_flag = false;
    push_interrupt_state(flagc, flagn, flagv, flagz, _6502_NMI_VECTOR);
    UINT uExtraCycles = 0;
    CYC(7)
    return true;
}

static __forceinline bool IRQ(ULONG& uExecutedCycles, BOOL& flagc, BOOL& flagn, BOOL& flagv, BOOL& flagz) {
    if(!applewin_toolbox::detail::irq_asserted || (regs.ps & AF_INTERRUPT)) {
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

#define READ(addr) (mem[static_cast<WORD>(addr)])
#define WRITE(value) do { mem[addr] = static_cast<BYTE>(value); dirty[addr >> 8] = 0xff; } while(0);
#include "CPU/cpu65C02.h"

#undef HEATMAP_X


namespace applewin_toolbox {

void attach_memory(std::uint8_t* memory, std::uint32_t size) {
    assert(memory != nullptr);
    assert(size >= 65536u);
    (void)size;
    mem = memory;
    initialise_pages();
}

std::uint16_t read_word(std::uint16_t address) {
    assert(mem != nullptr);
    return static_cast<std::uint16_t>(
        mem[address] |
        (static_cast<std::uint16_t>(mem[(address + 1u) & 0xffffu]) << 8));
}

namespace detail {

std::uint32_t execute_nmos6502(std::uint32_t cycles) {
    return Cpu6502(cycles, false);
}

std::uint32_t execute_cmos65c02(std::uint32_t cycles) {
    return Cpu65C02(cycles, false);
}

} // namespace detail

} // namespace applewin_toolbox
