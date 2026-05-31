#include <fake65c02_toolbox.hpp>

#include <chrono>
#include <cstdint>

extern "C" int fake65c02_klaus_extended_run_once(unsigned long long* cycles_out);

namespace {

struct run_result {
    bool passed;
    std::uint64_t cycles;
};

run_result run_klaus_once()
{
    unsigned long long cycles = 0;
    const int status = fake65c02_klaus_extended_run_once(&cycles);
    return {status == 0, static_cast<std::uint64_t>(cycles)};
}

} // namespace

namespace fake65c02_toolbox {

benchmark6502::klaus_benchmark_result run_klaus_65c02_extended(const int measured_runs)
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

} // namespace fake65c02_toolbox
