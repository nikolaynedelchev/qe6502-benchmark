#include <applewin_toolbox.hpp>

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

struct run_result {
    bool passed;
    std::uint64_t cycles;
};

run_result run_klaus_once()
{
    applewin_toolbox::reset_memory();
    std::memcpy(applewin_toolbox::memory(), s_klaus_nmos_rom, sizeof(s_klaus_nmos_rom));
    applewin_toolbox::reset_cpu(klaus_start_address, applewin_toolbox::CpuMode::nmos6502);

    while (applewin_toolbox::cumulative_cycles() < max_cycles) {
        if (applewin_toolbox::pc() == klaus_success_address) {
            return {true, applewin_toolbox::cumulative_cycles()};
        }
        if (applewin_toolbox::jammed()) {
            return {false, applewin_toolbox::cumulative_cycles()};
        }
        const auto executed = applewin_toolbox::execute(1);
        if (executed == 0u) {
            return {false, applewin_toolbox::cumulative_cycles()};
        }
    }

    return {false, applewin_toolbox::cumulative_cycles()};
}

} // namespace

namespace applewin_toolbox {

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

} // namespace applewin_toolbox
