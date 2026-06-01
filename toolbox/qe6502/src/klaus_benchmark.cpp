#include <qe6502_toolbox.hpp>

#include <qe6502/cpu.hpp>

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

qe6502::model to_qe6502_model(const qe6502_toolbox::klaus_model model)
{
    switch (model) {
    case qe6502_toolbox::klaus_model::nmos:
        return qe6502::model::nmos;
    case qe6502_toolbox::klaus_model::wdc:
        return qe6502::model::wdc;
    case qe6502_toolbox::klaus_model::rockwell:
        return qe6502::model::rw;
    case qe6502_toolbox::klaus_model::synertek:
        return qe6502::model::st;
    }

    return qe6502::model::nmos;
}

struct run_result {
    bool passed;
    std::uint64_t bus_ticks;
};

run_result run_klaus_once(const qe6502_toolbox::klaus_model model, const qe6502_toolbox::klaus_test test)
{
    if (test == qe6502_toolbox::klaus_test::extended &&
        model != qe6502_toolbox::klaus_model::wdc &&
        model != qe6502_toolbox::klaus_model::rockwell) {
        return {false, 0};
    }

    std::array<std::uint8_t, 0x10000> memory{};
    std::uint16_t success_address = klaus_nmos_success_address;

    if (test == qe6502_toolbox::klaus_test::extended) {
        std::memcpy(memory.data(), s_klaus_65c02_extended_rom, memory.size());
        success_address = klaus_65c02_extended_success_address;
    } else {
        std::memcpy(memory.data(), s_klaus_nmos_rom, memory.size());
        success_address = klaus_nmos_success_address;
    }

    qe6502::cpu cpu(to_qe6502_model(model));
    cpu.restart();
    cpu.jump_to(klaus_start_address);

    std::uint64_t bus_ticks = 0;

    for (;;) {
        const std::uint16_t address = cpu.bus_address();

        if (address == success_address) {
            return {true, bus_ticks};
        }

        std::uint8_t data;
        if (cpu.is_write()) {
            data = cpu.bus_data();
            memory[address] = data;
        } else {
            data = memory[address];
        }

        cpu.tick(data);
        ++bus_ticks;
    }
}

} // namespace

namespace qe6502_toolbox {

benchmark6502::klaus_benchmark_result run_klaus(const klaus_model model,
                                                 const klaus_test test,
                                                 const int measured_runs)
{
    const run_result cold = run_klaus_once(model, test);
    if (!cold.passed) {
        return {false, measured_runs, 0, 0.0};
    }

    std::uint64_t total_bus_ticks = 0;
    const auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < measured_runs; ++i) {
        const run_result result = run_klaus_once(model, test);
        if (!result.passed) {
            return {false, measured_runs, total_bus_ticks, 0.0};
        }
        total_bus_ticks += result.bus_ticks;
    }

    const auto stop = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed = stop - start;

    return {true, measured_runs, total_bus_ticks, elapsed.count()};
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

} // namespace qe6502_toolbox
