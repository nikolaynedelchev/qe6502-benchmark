#include <floooh_chips_toolbox.hpp>

#include "chips/m6502.h"

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>

static const std::uint8_t s_klaus_nmos_rom[0x10000] =
#include "cores/qe6502/harness/klaus2m5/6502_functional_test.hex"

namespace {

constexpr std::uint16_t klaus_start_address = 0x0400u;
constexpr std::uint16_t klaus_success_address = 0x3469u;

struct run_result {
    bool passed;
    std::uint64_t bus_ticks;
};

run_result run_klaus_once()
{
    std::array<std::uint8_t, 0x10000> memory{};
    std::memcpy(memory.data(), s_klaus_nmos_rom, memory.size());

    m6502_t cpu{};
    m6502_desc_t desc{};
    uint64_t pins = m6502_init(&cpu, &desc);

    m6502_set_pc(&cpu, klaus_start_address);
    pins = M6502_RW | M6502_SYNC;
    M6502_SET_ADDR(pins, klaus_start_address);
    M6502_SET_DATA(pins, memory[klaus_start_address]);

    std::uint64_t bus_ticks = 0;

    for (;;)
    {
        const std::uint16_t address = M6502_GET_ADDR(pins);

        if (address == klaus_success_address)
        {
            return {true, bus_ticks};
        }

        if ((pins & M6502_RW) != 0u)
        {
            M6502_SET_DATA(pins, memory[address]);
        }
        else
        {
            memory[address] = M6502_GET_DATA(pins);
        }

        pins = m6502_tick(&cpu, pins);
        ++bus_ticks;
    }
}

} // namespace

namespace floooh_chips_toolbox {

benchmark6502::klaus_benchmark_result run_klaus_nmos_standard(const int measured_runs)
{
    const run_result cold = run_klaus_once();
    if (!cold.passed)
    {
        return {false, measured_runs, 0, 0.0};
    }

    std::uint64_t total_bus_ticks = 0;
    const auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < measured_runs; ++i)
    {
        const run_result result = run_klaus_once();
        if (!result.passed)
        {
            return {false, measured_runs, total_bus_ticks, 0.0};
        }
        total_bus_ticks += result.bus_ticks;
    }

    const auto stop = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed = stop - start;

    return {true, measured_runs, total_bus_ticks, elapsed.count()};
}

} // namespace floooh_chips_toolbox
