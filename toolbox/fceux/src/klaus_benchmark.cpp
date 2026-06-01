#include <fceux_toolbox.hpp>

#include <fceu.h>
#include <x6502.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>

static const std::uint8_t s_klaus_nes_rom[0x10000] =
#include "asm_6502/6502_functional_test_nes_h.hex"

namespace {

constexpr std::uint16_t klaus_start_address = 0x0400u;
constexpr std::uint16_t klaus_success_address = 0x336du;
constexpr std::uint64_t max_cycles = 200000000ull;
constexpr int run_slice_cycles = 1;

std::array<std::uint8_t, 65536> memory{};

uint8 read_byte(uint32 address)
{
    return memory[static_cast<std::uint16_t>(address)];
}

void write_byte(uint32 address, uint8 value)
{
    memory[static_cast<std::uint16_t>(address)] = value;
}

void install_handlers()
{
    for (auto& read_handler : ARead) {
        read_handler = read_byte;
    }
    for (auto& write_handler : BWrite) {
        write_handler = write_byte;
    }
    RAM = memory.data();
}

struct run_result {
    bool passed;
    std::uint64_t cycles;
};

run_result run_klaus_once()
{
    std::memcpy(memory.data(), s_klaus_nes_rom, memory.size());
    install_handlers();

    X6502_Init();
    X6502_Power();

    X.PC = klaus_start_address;
    X.IRQlow = 0;
    X.jammed = 0;
    timestamp = 0;
    soundtimestamp = 0;

    while (timestamp < max_cycles) {
        if (X.PC == klaus_success_address) {
            return {true, timestamp};
        }
        if (X.jammed) {
            return {false, timestamp};
        }

        X6502_Run(run_slice_cycles);
    }

    return {false, timestamp};
}

} // namespace

namespace fceux_toolbox {

benchmark6502::klaus_benchmark_result run_klaus_nes_standard(const int measured_runs)
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

} // namespace fceux_toolbox
