#include <sflib6502_toolbox.hpp>

extern "C" {
#include <lib6502.h>
}

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>

static const std::uint8_t s_klaus_nmos_rom[0x10000] =
#include "cores/qe6502/harness/klaus2m5/6502_functional_test.hex"

namespace {

constexpr std::uint16_t klaus_start_address = 0x0400u;
constexpr std::uint16_t klaus_success_address = 0x3469u;
constexpr std::uint64_t max_instructions = 100000000ull;

struct run_result {
    bool passed;
    std::uint64_t cycles;
};

run_result run_klaus_once()
{
    M6502_Registers registers{};
    M6502_Memory memory{};
    M6502_Callbacks callbacks{};

    std::memcpy(memory, s_klaus_nmos_rom, sizeof(memory));

    M6502* cpu = M6502_new(&registers, memory, &callbacks);
    if (cpu == nullptr) {
        return {false, 0};
    }

    M6502_reset(cpu);
    registers.pc = klaus_start_address;

    std::uint64_t cycles = 0;

    for (std::uint64_t i = 0; i < max_instructions; ++i) {
        if (registers.pc == klaus_success_address) {
            M6502_delete(cpu);
            return {true, cycles};
        }

        cpu->ticks = 0;
        M6502_step(cpu);
        cycles += cpu->ticks;
    }

    M6502_delete(cpu);
    return {false, cycles};
}

} // namespace

namespace sflib6502_toolbox {

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

} // namespace sflib6502_toolbox
