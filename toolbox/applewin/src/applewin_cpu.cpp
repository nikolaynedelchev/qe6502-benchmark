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

static __forceinline void Fetch(BYTE& iOpcode, ULONG) {
    iOpcode = ram[regs.pc++];
}

static __forceinline bool NMI(ULONG&, BOOL&, BOOL&, BOOL&, BOOL&) { return false; }
static __forceinline bool IRQ(ULONG&, BOOL&, BOOL&, BOOL&, BOOL&) { return false; }
static __forceinline void CheckSynchronousInterruptSources(UINT, ULONG) {}
static __forceinline void DoIrqProfiling(std::uint32_t) {}
static __forceinline void NTSC_VideoUpdateCycles(ULONG) {}

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
#include "CPU/cpu6502.h"

#undef READ
#undef WRITE
#undef HEATMAP_X

namespace applewin_toolbox {

void reset_memory() {
    std::memset(ram, 0, sizeof(ram));
    initialise_pages();
}

std::uint8_t* memory() {
    return ram;
}

void reset_cpu(std::uint16_t pc, bool cmos) {
    g_main_cpu = cmos ? CPU_65C02 : CPU_6502;
    g_active_cpu = g_main_cpu;
    regs.a = 0;
    regs.x = 0;
    regs.y = 0;
    regs.ps = AF_RESERVED | AF_INTERRUPT;
    regs.pc = pc;
    regs.sp = 0x01fd;
    regs.bJammed = 0;
    g_nCumulativeCycles = 0;
}

std::uint32_t execute(std::uint32_t cycles) {
    const auto executed = Cpu6502(cycles, false);
    g_nCumulativeCycles += executed;
    return executed;
}

State state() {
    return State{regs.a, regs.x, regs.y, regs.ps, regs.pc, regs.sp, regs.bJammed != 0};
}

} // namespace applewin_toolbox
