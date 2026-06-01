#include <altirrasdl_toolbox.hpp>

#include <at/atcore/scheduler.h>
#include <at/atcpu/co6502.h>
#include <at/atcpu/execstate.h>
#include <at/atcpu/memorymap.h>

#include <array>
#include <cstdint>

namespace {
std::array<uint8, 65536> memory{};
}

namespace altirrasdl_toolbox {
benchmark6502::smoke_result run_smoke_test() {
    memory.fill(0);
    constexpr uint16 program_address = 0x0200;
    memory[program_address + 0] = 0xa9; // LDA #$42
    memory[program_address + 1] = 0x42;
    memory[program_address + 2] = 0xaa; // TAX
    memory[program_address + 3] = 0xea; // NOP
    memory[0xfffc] = program_address & 0xff;
    memory[0xfffd] = program_address >> 8;

    ATCoProc6502 cpu(false, false);
    ATCoProcMemoryMapView map(cpu.GetReadMap(), cpu.GetWriteMap(), cpu.GetTraceMap());
    map.SetMemory(0, 256, memory.data());
    cpu.ColdReset();

    ATScheduler scheduler;
    scheduler.SetStopTime(scheduler.GetTick() + 6);
    cpu.Run(scheduler);

    ATCPUExecState state{};
    cpu.GetExecState(state);
    if (state.m6502.mA != 0x42) return {false, "LDA immediate did not set A to 0x42"};
    if (state.m6502.mX != 0x42) return {false, "TAX did not copy A to X"};
    if (state.m6502.mPC != program_address + 3) return {false, "unexpected PC after LDA/TAX/NOP smoke program"};
    return {true, "passed"};
}
}
