#include <olcNES_toolbox.hpp>

#include <Bus.h>

#include <chrono>
#include <cstdint>
#include <cstring>

static const std::uint8_t s_klaus_nmos_rom[0x10000] =
#include "cores/qe6502/harness/klaus2m5/6502_functional_test.hex"

namespace {

constexpr std::uint16_t klaus_start_address = 0x0400u;
constexpr std::uint16_t klaus_success_address = 0x3469u;
constexpr std::uint64_t max_cycles = 200000000ull;

void run_until_complete(olc6502& cpu)
{
    do {
        cpu.clock();
    } while (!cpu.complete());
}

struct run_result {
    bool passed;
    std::uint64_t cycles;
};

run_result run_klaus_once()
{
    Bus bus;
    std::memcpy(bus.ram.data(), s_klaus_nmos_rom, bus.ram.size());

    bus.ram[0xFFFC] = static_cast<std::uint8_t>(klaus_start_address & 0xFFu);
    bus.ram[0xFFFD] = static_cast<std::uint8_t>(klaus_start_address >> 8u);

    bus.cpu.reset();
    run_until_complete(bus.cpu);
    bus.cpu.pc = klaus_start_address;

    std::uint64_t cycles = 0;

    for (; cycles < max_cycles; ++cycles) {
        if (bus.cpu.pc == klaus_success_address) {
            return {true, cycles};
        }

        bus.cpu.clock();
    }

    return {false, cycles};
}

} // namespace

namespace olcNES_toolbox {

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

} // namespace olcNES_toolbox
