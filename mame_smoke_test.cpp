#include "benchmark6502/mame/mame6502_cpu.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>

int main()
{
    benchmark6502::mame::Mame6502Cpu cpu;
    cpu.clear_memory();

    constexpr std::uint16_t start = 0x0200;
    const std::array<std::uint8_t, 4> program = {0xa9, 0x42, 0xaa, 0xea};
    cpu.load_program(start, program.data(), program.size());
    cpu.set_reset_vector(start);
    cpu.reset_from_vector();

    const unsigned executed = cpu.execute(6);
    if(executed != 6) {
        std::fprintf(stderr, "expected 6 cycles, got %u\n", executed);
        return EXIT_FAILURE;
    }
    if(cpu.a() != 0x42 || cpu.x() != 0x42 || cpu.pc() != 0x0204) {
        std::fprintf(stderr, "unexpected state: A=%02x X=%02x PC=%04x\n", cpu.a(), cpu.x(), cpu.pc());
        return EXIT_FAILURE;
    }
    if(cpu.is_jammed()) {
        std::fprintf(stderr, "CPU jammed unexpectedly\n");
        return EXIT_FAILURE;
    }
    if(cpu.read_count() == 0 || cpu.write_count() != 0) {
        std::fprintf(stderr, "unexpected memory counts: reads=%u writes=%u\n", cpu.read_count(), cpu.write_count());
        return EXIT_FAILURE;
    }

    std::puts("mame smoke test passed");
    return EXIT_SUCCESS;
}
