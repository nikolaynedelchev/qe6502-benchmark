#include <mame_toolbox.hpp>

#include <chrono>
#include <cstdint>

static const std::uint8_t s_klaus_nmos_rom[0x10000] =
#include "cores/qe6502/harness/klaus2m5/6502_functional_test.hex"

static const std::uint8_t s_klaus_65c02_extended_rom[0x10000] =
#include "cores/qe6502/harness/klaus2m5/65C02_extended_opcodes_test.hex"

namespace {

constexpr std::uint16_t klaus_start_address = 0x0400u;
constexpr std::uint16_t klaus_nmos_success_address = 0x3469u;
constexpr std::uint16_t klaus_65c02_extended_success_address = 0x24F1u;
constexpr std::uint64_t max_cycles = 200000000ull;
constexpr unsigned run_slice_cycles = 10000u;

struct run_result {
    bool passed;
    std::uint64_t cycles;
};

run_result run_klaus_once(const benchmark6502::mame::CpuMode mode,
                          const std::uint8_t* const rom,
                          const std::uint16_t success_address)
{
    benchmark6502::mame::Mame6502Cpu cpu(mode);
    cpu.clear_memory();
    cpu.load_program(0x0000u, rom, 0x10000u);
    cpu.set_pc_and_prefetch(klaus_start_address);
    cpu.set_sp(0xfdu);
    cpu.set_p(0x24u);
    cpu.reset_cycle_counter();
    cpu.watch_sync_address(success_address);

    while (cpu.total_cycles() < max_cycles) {
        if (cpu.pc() == success_address || cpu.sync_watch_seen()) {
            return {true, cpu.total_cycles()};
        }
        const unsigned executed = cpu.execute(run_slice_cycles);
        if (cpu.sync_watch_seen()) {
            return {true, cpu.total_cycles()};
        }
        if (cpu.is_jammed()) {
            return {false, cpu.total_cycles()};
        }
        if (executed == 0u) {
            return {false, cpu.total_cycles()};
        }
    }

    return {false, cpu.total_cycles()};
}

benchmark6502::klaus_benchmark_result run_klaus_rom(const benchmark6502::mame::CpuMode mode,
                                                    const std::uint8_t* const rom,
                                                    const std::uint16_t success_address,
                                                    const int measured_runs)
{
    const run_result cold = run_klaus_once(mode, rom, success_address);
    if (!cold.passed) {
        return {false, measured_runs, 0, 0.0};
    }

    std::uint64_t total_cycles = 0;
    const auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < measured_runs; ++i) {
        const run_result result = run_klaus_once(mode, rom, success_address);
        if (!result.passed) {
            return {false, measured_runs, total_cycles, 0.0};
        }
        total_cycles += result.cycles;
    }

    const auto stop = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed = stop - start;

    return {true, measured_runs, total_cycles, elapsed.count()};
}

benchmark6502::klaus_benchmark_result run_standard(const benchmark6502::mame::CpuMode mode, const int measured_runs)
{
    return run_klaus_rom(mode, s_klaus_nmos_rom, klaus_nmos_success_address, measured_runs);
}

benchmark6502::klaus_benchmark_result run_extended(const benchmark6502::mame::CpuMode mode, const int measured_runs)
{
    return run_klaus_rom(mode, s_klaus_65c02_extended_rom, klaus_65c02_extended_success_address, measured_runs);
}

} // namespace

namespace mame_toolbox {

benchmark6502::klaus_benchmark_result run_klaus_nmos_standard(const int measured_runs)
{
    return run_standard(benchmark6502::mame::CpuMode::nmos6502, measured_runs);
}

benchmark6502::klaus_benchmark_result run_klaus_wdc_65c02_standard(const int measured_runs)
{
    return run_standard(benchmark6502::mame::CpuMode::wdc65c02, measured_runs);
}

benchmark6502::klaus_benchmark_result run_klaus_wdc_65c02_extended(const int measured_runs)
{
    return run_extended(benchmark6502::mame::CpuMode::wdc65c02, measured_runs);
}

benchmark6502::klaus_benchmark_result run_klaus_rockwell_65c02_standard(const int measured_runs)
{
    return run_standard(benchmark6502::mame::CpuMode::rockwell65c02, measured_runs);
}

benchmark6502::klaus_benchmark_result run_klaus_rockwell_65c02_extended(const int measured_runs)
{
    return run_extended(benchmark6502::mame::CpuMode::rockwell65c02, measured_runs);
}

benchmark6502::klaus_benchmark_result run_klaus_wdc65c02s_standard(const int measured_runs)
{
    return run_standard(benchmark6502::mame::CpuMode::wdc65c02s, measured_runs);
}

benchmark6502::klaus_benchmark_result run_klaus_wdc65c02s_extended(const int measured_runs)
{
    return run_extended(benchmark6502::mame::CpuMode::wdc65c02s, measured_runs);
}

} // namespace mame_toolbox
