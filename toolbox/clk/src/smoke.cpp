#include <clk_toolbox.hpp>

#include <Processors/6502/6502.hpp>

#include <array>
#include <cstdint>

namespace {

class smoke_bus : public CPU::MOS6502::BusHandler {
public:
    using AddressType = std::uint16_t;

    std::array<std::uint8_t, 65536> memory{};
    std::uint64_t cycles = 0;

    Cycles perform_bus_operation(const CPU::MOS6502::BusOperation operation,
                                 const AddressType address,
                                 std::uint8_t* const value)
    {
        ++cycles;
        if (CPU::MOS6502Esque::is_read(operation)) {
            *value = memory[address];
        } else if (CPU::MOS6502Esque::is_write(operation)) {
            memory[address] = *value;
        }
        return Cycles(1);
    }
};

using processor_type = CPU::MOS6502::Processor<CPU::MOS6502::P6502, smoke_bus, false>;

} // namespace

namespace clk_toolbox {

benchmark6502::smoke_result run_smoke_test()
{
    smoke_bus bus;

    constexpr std::uint16_t program_address = 0x0200u;
    bus.memory[program_address + 0u] = 0xa9u; // LDA #$42
    bus.memory[program_address + 1u] = 0x42u;
    bus.memory[program_address + 2u] = 0xaau; // TAX
    bus.memory[program_address + 3u] = 0xeau; // NOP

    processor_type cpu(bus);
    cpu.set_power_on(false);
    cpu.set_reset_line(false);
    cpu.set_value_of(CPU::MOS6502::Register::ProgramCounter, program_address);
    cpu.set_value_of(CPU::MOS6502::Register::StackPointer, 0xfdu);
    cpu.set_value_of(CPU::MOS6502::Register::Flags, CPU::MOS6502::Flag::Always);
    cpu.restart_operation_fetch();

    cpu.run_for(Cycles(6));

    if (cpu.value_of(CPU::MOS6502::Register::A) != 0x42u) {
        return {false, "LDA immediate did not set A to 0x42"};
    }
    if (cpu.value_of(CPU::MOS6502::Register::X) != 0x42u) {
        return {false, "TAX did not copy A to X"};
    }
    if (cpu.value_of(CPU::MOS6502::Register::ProgramCounter) != program_address + 4u) {
        return {false, "unexpected PC after LDA/TAX/NOP smoke program"};
    }
    if (bus.cycles != 6u) {
        return {false, "unexpected cycle count for LDA/TAX/NOP smoke program"};
    }
    if (cpu.is_jammed()) {
        return {false, "CPU jammed during smoke program"};
    }

    return {true, "passed"};
}

} // namespace clk_toolbox
