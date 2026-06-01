#include <altirrasdl_toolbox.hpp>

#include <at/atcore/scheduler.h>
#include <at/atcpu/breakpoints.h>
#include <at/atcpu/co6502.h>
#include <at/atcpu/execstate.h>
#include <at/atcpu/memorymap.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>

static const std::uint8_t s_klaus_nmos_rom[0x10000] =
#include "cores/qe6502/harness/klaus2m5/6502_functional_test.hex"

namespace {

constexpr std::uint16_t klaus_start_address = 0x0400u;
constexpr std::uint16_t klaus_success_address = 0x3469u;
constexpr std::uint64_t max_cycles = 200000000ull;
constexpr std::uint32_t run_slice_cycles = 100u;

struct breakpoint_handler final : IATCPUBreakpointHandler {
    bool hit = false;

    bool CheckBreakpoint(uint32 pc) override
    {
        hit = pc == klaus_success_address;
        return hit;
    }
};

struct run_result {
    bool passed;
    std::uint64_t cycles;
};

run_result run_klaus_once()
{
    std::array<uint8, 65536> memory{};
    std::memcpy(memory.data(), s_klaus_nmos_rom, memory.size());

    ATCoProc6502 cpu(false, false);
    ATCoProcMemoryMapView map(cpu.GetReadMap(), cpu.GetWriteMap(), cpu.GetTraceMap());
    map.SetMemory(0, 256, memory.data());

    std::array<bool, 65536> breakpoint_map{};
    breakpoint_map[klaus_success_address] = true;
    breakpoint_handler handler;
    cpu.SetBreakpointMap(breakpoint_map.data(), &handler);

    ATCPUExecState state{};
    state.m6502.mPC = klaus_start_address;
    state.m6502.mA = 0;
    state.m6502.mX = 0;
    state.m6502.mY = 0;
    state.m6502.mS = 0xfd;
    state.m6502.mP = 0x24;
    cpu.SetExecState(state);

    std::uint64_t total_cycles = 0;

    while (total_cycles < max_cycles) {
        if (cpu.GetPC() == klaus_success_address || handler.hit) {
            return {true, total_cycles};
        }

        ATScheduler scheduler;
        const std::uint32_t start_tick = scheduler.GetTick();
        scheduler.SetStopTime(start_tick + run_slice_cycles);
        const bool consumed = cpu.Run(scheduler);
        total_cycles += scheduler.GetTick() - start_tick;

        if (cpu.GetPC() == klaus_success_address || handler.hit) {
            return {true, total_cycles};
        }
        if (!consumed) {
            return {false, total_cycles};
        }
    }

    return {false, total_cycles};
}

} // namespace

namespace altirrasdl_toolbox {

benchmark6502::klaus_benchmark_result run_klaus_nmos_standard(const int measured_runs)
{
    const run_result cold = run_klaus_once();
    if (!cold.passed) {
        return {false, measured_runs, 0, 0.0};
    }

    std::uint64_t total_cycles = 0;
    const auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < measured_runs; ++i) {
        const run_result result = run_klaus_once();
        if (!result.passed) {
            return {false, measured_runs, total_cycles, 0.0};
        }
        total_cycles += result.cycles;
    }

    const auto stop = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed = stop - start;

    return {true, measured_runs, total_cycles, elapsed.count()};
}

} // namespace altirrasdl_toolbox
