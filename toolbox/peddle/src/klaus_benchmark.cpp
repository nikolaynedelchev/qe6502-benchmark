#include <peddle_toolbox.hpp>

#include <Peddle.h>

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

class KlausCpu final : public peddle::Peddle {
public:
    std::array<std::uint8_t, 0x10000> memory{};

    KlausCpu()
    {
        std::memcpy(memory.data(), s_klaus_nmos_rom, memory.size());
        setModel(peddle::MOS_6502);
    }

    peddle::u8 read(const peddle::u16 address) override
    {
        return memory[address];
    }

    void write(const peddle::u16 address, const peddle::u8 value) override
    {
        memory[address] = value;
    }

    peddle::u8 readDasm(const peddle::u16 address) const override
    {
        return memory[address];
    }
};

struct run_result {
    bool passed;
    std::uint64_t ticks;
};

run_result run_klaus_once()
{
    KlausCpu cpu;
    cpu.reset();
    cpu.reg.pc = klaus_start_address;
    cpu.reg.pc0 = klaus_start_address;

    std::uint64_t ticks = 0;
    while (ticks < max_klaus_ticks) {
        if (cpu.inFetchPhase() && cpu.reg.pc == klaus_success_address) {
            return {true, ticks};
        }
        cpu.execute();
        ++ticks;
    }

    return {false, ticks};
}

} // namespace

namespace peddle_toolbox {

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

} // namespace peddle_toolbox
