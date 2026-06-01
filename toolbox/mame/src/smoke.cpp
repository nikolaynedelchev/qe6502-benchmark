#include <mame_toolbox.hpp>

#include <array>

namespace mame_toolbox {

benchmark6502::smoke_result run_smoke_test()
{
    benchmark6502::mame::Mame6502Cpu cpu;
    cpu.clear_memory();

    constexpr std::uint16_t program_address = 0x0200u;
    const std::array<std::uint8_t, 4> program = {0xa9u, 0x42u, 0xaau, 0xeau};
    cpu.load_program(program_address, program.data(), program.size());
    cpu.set_reset_vector(program_address);
    cpu.reset_from_vector();

    const unsigned cycles = cpu.execute(6);
    if (cycles != 6u) {
        return {false, "unexpected cycle count for LDA/TAX/NOP smoke program"};
    }
    if (cpu.a() != 0x42u) {
        return {false, "LDA immediate did not set A to 0x42"};
    }
    if (cpu.x() != 0x42u) {
        return {false, "TAX did not copy A to X"};
    }
    if (cpu.pc() != program_address + 4u) {
        return {false, "unexpected PC after LDA/TAX/NOP smoke program"};
    }
    if (cpu.is_jammed()) {
        return {false, "CPU jammed during smoke program"};
    }
    if (cpu.read_count() == 0u) {
        return {false, "no host-visible reads were observed"};
    }
    if (cpu.write_count() != 0u) {
        return {false, "unexpected host-visible writes during read-only smoke program"};
    }

    return {true, "passed"};
}

} // namespace mame_toolbox
