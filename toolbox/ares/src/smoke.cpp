#include <ares_toolbox.hpp>

#include <ares/ares.hpp>
#include <component/processor/mos6502/mos6502.hpp>

#include <array>
#include <cstdint>

namespace {

class smoke_cpu final : public ares::MOS6502 {
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

} // namespace

namespace ares_toolbox {

benchmark6502::smoke_result run_smoke_test()
{
    smoke_cpu cpu;

    constexpr std::uint16_t program_address = 0x0200u;
    cpu.memory[program_address + 0u] = 0xa9u; // LDA #$42
    cpu.memory[program_address + 1u] = 0x42u;
    cpu.memory[program_address + 2u] = 0xaau; // TAX
    cpu.memory[program_address + 3u] = 0xeau; // NOP

    cpu.power();
    cpu.PC = program_address;
    cpu.S = 0xfdu;
    cpu.P = 0x24u;

    cpu.instruction();
    cpu.instruction();
    cpu.instruction();

    if (static_cast<std::uint8_t>(cpu.A) != 0x42u) {
        return {false, "LDA immediate did not set A to 0x42"};
    }
    if (static_cast<std::uint8_t>(cpu.X) != 0x42u) {
        return {false, "TAX did not copy A to X"};
    }
    if (static_cast<std::uint16_t>(cpu.PC) != program_address + 4u) {
        return {false, "unexpected PC after LDA/TAX/NOP smoke program"};
    }
    if (cpu.bus_cycles != 6u) {
        return {false, "unexpected bus cycle count for LDA/TAX/NOP smoke program"};
    }

    return {true, "passed"};
}

} // namespace ares_toolbox
