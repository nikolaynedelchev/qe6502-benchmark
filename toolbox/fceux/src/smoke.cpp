#include <fceux_toolbox.hpp>

#include <fceu.h>
#include <x6502.h>

#include <array>
#include <cstdint>

namespace {

std::array<std::uint8_t, 65536> memory{};
std::uint64_t read_count = 0;
std::uint64_t write_count = 0;

uint8 read_byte(uint32 address)
{
    ++read_count;
    return memory[static_cast<std::uint16_t>(address)];
}

void write_byte(uint32 address, uint8 value)
{
    ++write_count;
    memory[static_cast<std::uint16_t>(address)] = value;
}

void install_handlers()
{
    for (auto& read_handler : ARead) {
        read_handler = read_byte;
    }
    for (auto& write_handler : BWrite) {
        write_handler = write_byte;
    }
    RAM = memory.data();
}

} // namespace

namespace fceux_toolbox {

benchmark6502::smoke_result run_smoke_test()
{
    memory.fill(0u);
    read_count = 0;
    write_count = 0;
    install_handlers();

    constexpr std::uint16_t program_address = 0x0200u;
    memory[program_address + 0u] = 0xa9u; // LDA #$42
    memory[program_address + 1u] = 0x42u;
    memory[program_address + 2u] = 0xaau; // TAX
    memory[program_address + 3u] = 0xeau; // NOP
    memory[0xfffcu] = static_cast<std::uint8_t>(program_address & 0xffu);
    memory[0xfffdu] = static_cast<std::uint8_t>(program_address >> 8u);

    X6502_Init();
    X6502_Power();
    X6502_Run(18);

    if (X.A != 0x42u) {
        return {false, "LDA immediate did not set A to 0x42"};
    }
    if (X.X != 0x42u) {
        return {false, "TAX did not copy A to X"};
    }
    if (X.PC != program_address + 4u) {
        return {false, "unexpected PC after LDA/TAX/NOP smoke program"};
    }
    if (X.jammed) {
        return {false, "CPU jammed during smoke program"};
    }
    if (read_count == 0u) {
        return {false, "no host-visible reads were observed"};
    }
    if (write_count != 0u) {
        return {false, "unexpected host-visible writes during read-only smoke program"};
    }

    return {true, "passed"};
}

} // namespace fceux_toolbox
