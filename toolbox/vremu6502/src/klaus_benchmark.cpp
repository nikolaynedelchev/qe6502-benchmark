#include <vremu6502_toolbox.hpp>

#include <vrEmu6502.h>

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
constexpr std::uint64_t max_klaus_ticks = 200000000ull;

std::array<std::uint8_t, 0x10000>* g_memory = nullptr;

std::uint8_t memory_read(const std::uint16_t address, const bool /*is_debug*/)
{
    return (*g_memory)[address];
}

void memory_write(const std::uint16_t address, const std::uint8_t value)
{
    (*g_memory)[address] = value;
}

vrEmu6502Model to_vremu_model(const vremu6502_toolbox::klaus_model model)
{
    switch (model) {
    case vremu6502_toolbox::klaus_model::nmos:
        return CPU_6502U;
    case vremu6502_toolbox::klaus_model::wdc:
        return CPU_W65C02;
    case vremu6502_toolbox::klaus_model::rockwell:
        return CPU_R65C02;
    }

    return CPU_6502U;
}

struct run_result {
    bool passed;
    std::uint64_t ticks;
};

run_result run_klaus_once(const vremu6502_toolbox::klaus_model model, const vremu6502_toolbox::klaus_test test)
{
    if (test == vremu6502_toolbox::klaus_test::extended &&
        model != vremu6502_toolbox::klaus_model::wdc &&
        model != vremu6502_toolbox::klaus_model::rockwell) {
        return {false, 0};
    }

    std::array<std::uint8_t, 0x10000> memory{};
    std::uint16_t success_address = klaus_nmos_success_address;

    if (test == vremu6502_toolbox::klaus_test::extended) {
        std::memcpy(memory.data(), s_klaus_65c02_extended_rom, memory.size());
        success_address = klaus_65c02_extended_success_address;
    } else {
        std::memcpy(memory.data(), s_klaus_nmos_rom, memory.size());
        success_address = klaus_nmos_success_address;
    }

    g_memory = &memory;
    VrEmu6502* cpu = vrEmu6502New(to_vremu_model(model), memory_read, memory_write);
    if (cpu == nullptr) {
        g_memory = nullptr;
        return {false, 0};
    }

    vrEmu6502Reset(cpu);
    vrEmu6502SetPC(cpu, klaus_start_address);

    std::uint64_t ticks = 0;
    bool passed = false;

    while (ticks < max_klaus_ticks) {
        if (vrEmu6502GetPC(cpu) == success_address) {
            passed = true;
            break;
        }
        ticks += vrEmu6502InstCycle(cpu);
    }

    vrEmu6502Destroy(cpu);
    g_memory = nullptr;
    return {passed, ticks};
}

} // namespace

namespace vremu6502_toolbox {

benchmark6502::klaus_benchmark_result run_klaus(const klaus_model model,
                                                 const klaus_test test,
                                                 const int measured_runs)
{
    const run_result cold = run_klaus_once(model, test);
    if (!cold.passed) {
        return {false, measured_runs, 0, 0.0};
    }

    std::uint64_t total_ticks = 0;
    const auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < measured_runs; ++i) {
        const run_result result = run_klaus_once(model, test);
        if (!result.passed) {
            return {false, measured_runs, total_ticks, 0.0};
        }
        total_ticks += result.ticks;
    }

    const auto stop = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed = stop - start;

    return {true, measured_runs, total_ticks, elapsed.count()};
}

benchmark6502::klaus_benchmark_result run_klaus_nmos_standard(const int measured_runs)
{
    return run_klaus(klaus_model::nmos, klaus_test::standard, measured_runs);
}

benchmark6502::klaus_benchmark_result run_klaus_wdc_65c02_standard(const int measured_runs)
{
    return run_klaus(klaus_model::wdc, klaus_test::standard, measured_runs);
}

benchmark6502::klaus_benchmark_result run_klaus_wdc_65c02_extended(const int measured_runs)
{
    return run_klaus(klaus_model::wdc, klaus_test::extended, measured_runs);
}

benchmark6502::klaus_benchmark_result run_klaus_rockwell_65c02_standard(const int measured_runs)
{
    return run_klaus(klaus_model::rockwell, klaus_test::standard, measured_runs);
}

benchmark6502::klaus_benchmark_result run_klaus_rockwell_65c02_extended(const int measured_runs)
{
    return run_klaus(klaus_model::rockwell, klaus_test::extended, measured_runs);
}

} // namespace vremu6502_toolbox
