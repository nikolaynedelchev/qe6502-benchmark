#include <o2_toolbox.hpp>

#include <O2.hpp>

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>

static const std::uint8_t s_klaus_nmos_rom[0x10000] =
#include "cores/qe6502/harness/klaus2m5/6502_functional_test.hex"

namespace {

constexpr std::uint16_t klaus_start_address = 0x0400u;
constexpr std::uint16_t klaus_success_address = 0x3469u;
constexpr std::uint64_t max_klaus_ticks = 200000000ull;

struct run_result {
    bool passed;
    std::uint64_t ticks;
};

run_result run_klaus_once()
{
    std::array<std::uint8_t, 0x10000> memory{};
    std::memcpy(memory.data(), s_klaus_nmos_rom, memory.size());

    O2::CPU cpu(
        [&memory](const std::uint16_t address) { return memory[address]; },
        [&memory](const std::uint16_t address, const std::uint8_t value) { memory[address] = value; });

    cpu.reset();
    cpu.PC = klaus_start_address;
    cpu.tsc = 0;

    bool passed = false;
    while (cpu.tsc < max_klaus_ticks) {
        if (cpu.PC == klaus_success_address) {
            passed = true;
            break;
        }
        cpu.cycle();
    }

    return {passed, static_cast<std::uint64_t>(cpu.tsc)};
}

} // namespace

namespace o2_toolbox {

benchmark6502::klaus_benchmark_result run_klaus_nmos_standard(const int measured_runs)
{
    const run_result cold = run_klaus_once();
    if (!cold.passed) {
        return {false, measured_runs, 0, 0.0};
    }

    std::uint64_t total_ticks = 0;
    const auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < measured_runs; ++i) {
        const run_result result = run_klaus_once();
        if (!result.passed) {
            return {false, measured_runs, total_ticks, 0.0};
        }
        total_ticks += result.ticks;
    }

    const auto stop = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed = stop - start;

    return {true, measured_runs, total_ticks, elapsed.count()};
}

} // namespace o2_toolbox
