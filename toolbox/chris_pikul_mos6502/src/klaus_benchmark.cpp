#include <chris_pikul_mos6502_toolbox.hpp>

#include <bus.h>
#include <cpu.h>
#include <memory.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <memory>

static const std::uint8_t s_klaus_nmos_rom[0x10000] =
#include "cores/qe6502/harness/klaus2m5/6502_functional_test.hex"

namespace {

constexpr std::uint16_t klaus_start_address = 0x0400u;
constexpr std::uint16_t klaus_success_address = 0x3469u;
constexpr std::uint64_t max_ticks = 200000000ull;

class benchmark_cpu final : public mos6502::CPU {
public:
    using mos6502::CPU::CPU;

    unsigned int cycles_executed() const
    {
        return m_CyclesExecuted;
    }
};

struct run_result {
    bool passed;
    std::uint64_t cycles;
};

run_result run_klaus_once()
{
    const auto memory = std::make_shared<mos6502::Memory>();
    const auto bus = std::make_shared<mos6502::Bus>(memory);
    benchmark_cpu cpu(bus);

    std::memcpy(memory->GetData(), s_klaus_nmos_rom, sizeof(s_klaus_nmos_rom));
    memory->WriteByte(0xFFFC, static_cast<mos6502::byte>(klaus_start_address & 0xFFu));
    memory->WriteByte(0xFFFD, static_cast<mos6502::byte>(klaus_start_address >> 8u));

    cpu.Reset();

    for (std::uint64_t i = 0; i < max_ticks; ++i) {
        if (cpu.GetProgramCounter() == klaus_success_address) {
            return {true, cpu.cycles_executed()};
        }

        cpu.Tick();
    }

    return {false, cpu.cycles_executed()};
}

} // namespace

namespace chris_pikul_mos6502_toolbox {

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

} // namespace chris_pikul_mos6502_toolbox
