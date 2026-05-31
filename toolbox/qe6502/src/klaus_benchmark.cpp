#include <qe6502_toolbox.hpp>

#include <qe6502/cpu.hpp>

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

    qe6502::cpu cpu(qe6502::model::nmos);
    cpu.restart();
    cpu.jump_to(klaus_start_address);

    const qe6502_tick_t* tick = &cpu.raw_tick();
    std::uint64_t bus_ticks = 0;

    for (;;)
    {
        const std::uint16_t address = tick->address;

        if (address == klaus_success_address)
        {
            return {true, bus_ticks};
        }

        std::uint8_t data;
        if ((tick->status & qe6502_status_writing) != 0u)
        {
            data = tick->bus;
            memory[address] = data;
        }
        else
        {
            data = memory[address];
        }

        tick = &cpu.tick(data);
        ++bus_ticks;
    }
}

} // namespace

namespace qe6502_toolbox {

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

} // namespace qe6502_toolbox
