#include <gianlucag_mos6502_toolbox.hpp>

#include <mos6502.h>

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

std::array<std::uint8_t, 65536> memory{};

std::uint8_t read_byte(std::uint16_t address)
{
    return memory[address];
}

void write_byte(std::uint16_t address, std::uint8_t value)
{
    memory[address] = value;
}

struct run_result {
    bool passed;
    std::uint64_t cycles;
};

run_result run_klaus_once()
{
    std::memcpy(memory.data(), s_klaus_nmos_rom, memory.size());

    mos6502 cpu(read_byte, write_byte);
    cpu.Reset();
    cpu.SetPC(klaus_start_address);

    std::uint64_t cycles = 0;

    for (std::uint64_t i = 0; i < max_instructions; ++i)
    {
        if (cpu.GetPC() == klaus_success_address)
        {
            return {true, cycles};
        }

        cpu.Run(1, cycles, mos6502::INST_COUNT);
    }

    return {false, cycles};
}

} // namespace

namespace gianlucag_mos6502_toolbox {

benchmark6502::klaus_benchmark_result run_klaus_nmos_standard(const int measured_runs)
{
    const run_result cold = run_klaus_once();
    if (!cold.passed)
    {
        return {false, measured_runs, 0, 0.0};
    }

    std::uint64_t total_cycles = 0;
    const auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < measured_runs; ++i)
    {
        const run_result result = run_klaus_once();
        if (!result.passed)
        {
            return {false, measured_runs, total_cycles, 0.0};
        }
        total_cycles += result.cycles;
    }

    const auto stop = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed = stop - start;

    return {true, measured_runs, total_cycles, elapsed.count()};
}

} // namespace gianlucag_mos6502_toolbox
