#include <gianlucag_mos6502_toolbox.hpp>

#include <mos6502.h>

#include <array>
#include <cstdint>

namespace {

std::array<std::uint8_t, 65536> memory{};

std::uint8_t read_byte(std::uint16_t address)
{
    return memory[address];
}

void write_byte(std::uint16_t address, std::uint8_t value)
{
    memory[address] = value;
}

} // namespace

namespace gianlucag_mos6502_toolbox {

benchmark6502::smoke_result run_smoke_test()
{
    memory.fill(0xEA);
    memory[0x8000] = 0xA2; // LDX #$42
    memory[0x8001] = 0x42;
    memory[0xFFFC] = 0x00;
    memory[0xFFFD] = 0x80;

    mos6502 cpu(read_byte, write_byte);
    cpu.Reset();

    std::uint64_t cycles = 0;
    cpu.Run(1, cycles, mos6502::INST_COUNT);

    if (cpu.GetX() != 0x42) {
        return {false, "LDX immediate did not set X to 0x42"};
    }
    if (cycles != 2) {
        return {false, "LDX immediate did not report 2 cycles"};
    }

    return {true, ""};
}

} // namespace gianlucag_mos6502_toolbox
