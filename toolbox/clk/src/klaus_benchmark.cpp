#include <clk_toolbox.hpp>

#include <Processors/6502/6502.hpp>

#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>

static const std::uint8_t s_klaus_nmos_rom[0x10000] =
#include "cores/qe6502/harness/klaus2m5/6502_functional_test.hex"

namespace {

constexpr std::uint16_t klaus_start_address = 0x0400u;
constexpr std::uint16_t klaus_success_address = 0x3469u;
constexpr std::uint64_t max_cycles = 200000000ull;
constexpr int run_slice_cycles = 10000;

class klaus_bus : public CPU::MOS6502::BusHandler {
public:
    using AddressType = std::uint16_t;

    std::array<std::uint8_t, 65536> memory{};
    std::uint64_t cycles = 0;
    bool success_reached = false;
    std::uint64_t success_cycles = 0;

    Cycles perform_bus_operation(const CPU::MOS6502::BusOperation operation,
                                 const AddressType address,
                                 std::uint8_t* const value)
    {
        ++cycles;
        if (!success_reached && address == klaus_success_address && CPU::MOS6502Esque::is_read(operation)) {
            success_reached = true;
            success_cycles = cycles - 1u;
        }
        if (CPU::MOS6502Esque::is_read(operation)) {
            *value = memory[address];
        } else if (CPU::MOS6502Esque::is_write(operation)) {
            memory[address] = *value;
        }
        return Cycles(1);
    }
};

using processor_type = CPU::MOS6502::Processor<CPU::MOS6502::P6502, klaus_bus, false>;

struct run_result {
    bool passed;
    std::uint64_t cycles;
};

run_result run_klaus_once()
{
    klaus_bus bus;
    std::memcpy(bus.memory.data(), s_klaus_nmos_rom, bus.memory.size());

    processor_type cpu(bus);
    cpu.set_power_on(false);
    cpu.set_reset_line(false);
    cpu.set_value_of(CPU::MOS6502::Register::ProgramCounter, klaus_start_address);
    cpu.set_value_of(CPU::MOS6502::Register::StackPointer, 0xfdu);
    cpu.set_value_of(CPU::MOS6502::Register::Flags, CPU::MOS6502::Flag::Always);
    cpu.restart_operation_fetch();

    while (bus.cycles < max_cycles) {
        if (bus.success_reached || cpu.value_of(CPU::MOS6502::Register::ProgramCounter) == klaus_success_address) {
            return {true, bus.success_reached ? bus.success_cycles : bus.cycles};
        }
        if (cpu.is_jammed()) {
            return {false, bus.cycles};
        }
        cpu.run_for(Cycles(run_slice_cycles));
    }

    return {false, bus.cycles};
}

} // namespace

namespace clk_toolbox {

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

} // namespace clk_toolbox
