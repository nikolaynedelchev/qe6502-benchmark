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


    {
        benchmark6502::mame::Mame6502Cpu cmos_cpu(benchmark6502::mame::CpuMode::wdc65c02);
        cmos_cpu.clear_memory();
        constexpr std::uint16_t cmos_start = 0x0300;
        const std::array<std::uint8_t, 4> cmos_program = {0x64, 0x20, 0xea, 0xea}; // STZ $20; NOP; NOP
        cmos_cpu.write_memory(0x0020, 0xff);
        cmos_cpu.load_program(cmos_start, cmos_program.data(), cmos_program.size());
        cmos_cpu.set_reset_vector(cmos_start);
        cmos_cpu.reset_from_vector();
        const unsigned cmos_executed = cmos_cpu.execute(5);
        if(cmos_executed != 5 || cmos_cpu.read_memory(0x0020) != 0x00 || cmos_cpu.pc() != 0x0303) {
            std::fprintf(stderr, "unexpected WDC 65C02 state: cycles=%u mem20=%02x PC=%04x\n",
                cmos_executed, cmos_cpu.read_memory(0x0020), cmos_cpu.pc());
            return EXIT_FAILURE;
        }
    }

    {
        benchmark6502::mame::Mame6502Cpu rockwell_cpu(benchmark6502::mame::CpuMode::rockwell65c02);
        rockwell_cpu.clear_memory();
        constexpr std::uint16_t rockwell_start = 0x0400;
        const std::array<std::uint8_t, 3> rockwell_program = {0x07, 0x30, 0xea}; // RMB0 $30; NOP
        rockwell_cpu.write_memory(0x0030, 0xff);
        rockwell_cpu.load_program(rockwell_start, rockwell_program.data(), rockwell_program.size());
        rockwell_cpu.set_reset_vector(rockwell_start);
        rockwell_cpu.reset_from_vector();
        const unsigned rockwell_executed = rockwell_cpu.execute(7);
        if(rockwell_executed != 7 || rockwell_cpu.read_memory(0x0030) != 0xfe || rockwell_cpu.pc() != 0x0403) {
            std::fprintf(stderr, "unexpected Rockwell 65C02 state: cycles=%u mem30=%02x PC=%04x\n",
                rockwell_executed, rockwell_cpu.read_memory(0x0030), rockwell_cpu.pc());
            return EXIT_FAILURE;
        }
    }

    {
        benchmark6502::mame::Mame6502Cpu w65s_cpu(benchmark6502::mame::CpuMode::wdc65c02s);
        w65s_cpu.clear_memory();
        constexpr std::uint16_t w65s_start = 0x0500;
        const std::array<std::uint8_t, 5> w65s_program = {0x64, 0x40, 0x07, 0x40, 0xea}; // STZ $40; RMB0 $40; NOP
        w65s_cpu.write_memory(0x0040, 0xff);
        w65s_cpu.load_program(w65s_start, w65s_program.data(), w65s_program.size());
        w65s_cpu.set_reset_vector(w65s_start);
        w65s_cpu.reset_from_vector();
        const unsigned w65s_executed = w65s_cpu.execute(10);
        if(w65s_executed != 10 || w65s_cpu.read_memory(0x0040) != 0x00 || w65s_cpu.pc() != 0x0505) {
            std::fprintf(stderr, "unexpected W65C02S state: cycles=%u mem40=%02x PC=%04x\n",
                w65s_executed, w65s_cpu.read_memory(0x0040), w65s_cpu.pc());
            return EXIT_FAILURE;
        }
    }

    std::puts("mame smoke test passed");
    return EXIT_SUCCESS;
}
