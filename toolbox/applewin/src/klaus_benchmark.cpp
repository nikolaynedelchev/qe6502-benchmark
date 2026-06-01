#include <applewin_toolbox.hpp>

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>

static const std::uint8_t s_klaus_nmos_rom[0x10000] =
#include "cores/qe6502/harness/klaus2m5/6502_functional_test.hex"

static const std::uint8_t s_klaus_65c02_extended_rom[0x10000] =
#include "cores/qe6502/harness/klaus2m5/65C02_extended_opcodes_test.hex"

namespace {

constexpr std::uint16_t klaus_start_address = 0x0400u;
constexpr std::uint16_t klaus_nmos_success_address = 0x3469u;
constexpr std::uint16_t klaus_65c02_extended_success_address = 0x24F1u;
constexpr std::uint64_t max_cycles = 200000000ull;

struct client_memory {
    std::array<std::uint8_t, 65536> ram{};

    void reset() {
        ram.fill(0);
        applewin_toolbox::attach_memory(ram.data(), static_cast<std::uint32_t>(ram.size()));
    }

    void load_rom(const std::uint8_t* data, const std::size_t size) {
        std::memcpy(ram.data(), data, size);
    }
};

struct run_result {
    bool passed;
    std::uint64_t cycles;
};

run_result run_klaus_once(const std::uint8_t* const rom,
                          const std::uint16_t success_address,
                          const applewin_toolbox::CpuMode mode)
{
    client_memory memory;
    memory.reset();
    memory.load_rom(rom, 0x10000u);
    applewin_toolbox::reset_cpu(klaus_start_address, mode);

    while (applewin_toolbox::cumulative_cycles() < max_cycles) {
        if (applewin_toolbox::pc() == success_address) {
            return {true, applewin_toolbox::cumulative_cycles()};
        }
        if (applewin_toolbox::jammed()) {
            return {false, applewin_toolbox::cumulative_cycles()};
        }
        const auto executed = applewin_toolbox::execute(1);
        if (executed == 0u) {
            return {false, applewin_toolbox::cumulative_cycles()};
        }
    }

    return {false, applewin_toolbox::cumulative_cycles()};
}

benchmark6502::klaus_benchmark_result run_klaus_rom(const std::uint8_t* const rom,
                                                    const std::uint16_t success_address,
                                                    const applewin_toolbox::CpuMode mode,
                                                    const int measured_runs)
{
    const run_result cold = run_klaus_once(rom, success_address, mode);
    if (!cold.passed) {
        return {false, measured_runs, 0, 0.0};
    }

    std::uint64_t total_cycles = 0;
    const auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < measured_runs; ++i) {
        const run_result result = run_klaus_once(rom, success_address, mode);
        if (!result.passed) {
            return {false, measured_runs, total_cycles, 0.0};
        }
        total_cycles += result.cycles;
    }

    const auto stop = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed = stop - start;

    return {true, measured_runs, total_cycles, elapsed.count()};
}

} // namespace

namespace applewin_toolbox {

benchmark6502::klaus_benchmark_result run_klaus_nmos_standard(const int measured_runs)
{
    return run_klaus_rom(s_klaus_nmos_rom, klaus_nmos_success_address, CpuMode::nmos6502, measured_runs);
}

benchmark6502::klaus_benchmark_result run_klaus_wdc_65c02_standard(const int measured_runs)
{
    return run_klaus_rom(s_klaus_nmos_rom, klaus_nmos_success_address, CpuMode::cmos65c02, measured_runs);
}

benchmark6502::klaus_benchmark_result run_klaus_wdc_65c02_extended(const int measured_runs)
{
    return run_klaus_rom(s_klaus_65c02_extended_rom, klaus_65c02_extended_success_address, CpuMode::cmos65c02, measured_runs);
}

} // namespace applewin_toolbox
