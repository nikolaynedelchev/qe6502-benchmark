#include <ares_toolbox.hpp>

#include <ares/ares.hpp>
#include <component/processor/mos6502/mos6502.hpp>

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>

static const std::uint8_t s_klaus_nmos_rom[0x10000] =
#include "cores/qe6502/harness/klaus2m5/6502_functional_test.hex"

namespace {

constexpr std::uint16_t klaus_start_address = 0x0400u;
constexpr std::uint16_t klaus_success_address = 0x3469u;
constexpr std::uint64_t max_bus_cycles = 200000000ull;

class klaus_cpu final : public ares::MOS6502 {
public:
    std::array<std::uint8_t, 65536> memory{};
    std::uint64_t bus_cycles = 0;

    auto read(n16 address) -> n8 override
    {
        ++bus_cycles;
        return memory[static_cast<std::uint16_t>(address)];
    }

    auto write(n16 address, n8 data) -> void override
    {
        ++bus_cycles;
        memory[static_cast<std::uint16_t>(address)] = static_cast<std::uint8_t>(data);
    }
};

struct run_result {
    bool passed;
    std::uint64_t bus_cycles;
};

run_result run_klaus_once()
{
    klaus_cpu cpu;
    std::memcpy(cpu.memory.data(), s_klaus_nmos_rom, cpu.memory.size());

    cpu.power();
    cpu.BCD = 1;
    cpu.PC = klaus_start_address;
    cpu.S = 0xfdu;
    cpu.P = 0x24u;

    while (cpu.bus_cycles < max_bus_cycles) {
        if (static_cast<std::uint16_t>(cpu.PC) == klaus_success_address) {
            return {true, cpu.bus_cycles};
        }
        cpu.instruction();
    }

    return {false, cpu.bus_cycles};
}

} // namespace

namespace ares_toolbox {

benchmark6502::klaus_benchmark_result run_klaus_nmos_standard(const int measured_runs)
{
    const run_result cold = run_klaus_once();
    if (!cold.passed) {
        return {false, measured_runs, 0, 0.0};
    }

    std::uint64_t total_bus_cycles = 0;
    const auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < measured_runs; ++i) {
        const run_result result = run_klaus_once();
        if (!result.passed) {
            return {false, measured_runs, total_bus_cycles, 0.0};
        }
        total_bus_cycles += result.bus_cycles;
    }

    const auto stop = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed = stop - start;

    return {true, measured_runs, total_bus_cycles, elapsed.count()};
}

} // namespace ares_toolbox
