#include <chris_pikul_mos6502_toolbox.hpp>

#include <bus.h>
#include <cpu.h>
#include <memory.h>

#include <memory>

#include <iostream>
#include <sstream>

namespace chris_pikul_mos6502_toolbox {

benchmark6502::smoke_result run_smoke_test()
{
    const auto memory = std::make_shared<mos6502::Memory>();
    const auto bus = std::make_shared<mos6502::Bus>(memory);
    mos6502::CPU cpu(bus);

    memory->Clear(0xEA);
    memory->WriteByte(0xFFFC, 0x00);
    memory->WriteByte(0xFFFD, 0x80);
    memory->WriteByte(0x8000, 0xA2); // LDX #$42
    memory->WriteByte(0x8001, 0x42);

    std::ostringstream sink;
    std::streambuf* const old_cout = std::cout.rdbuf(sink.rdbuf());

    cpu.Reset();
    for (int i = 0; i < 16 && cpu.GetX() != 0x42; ++i) {
        cpu.Tick();
    }

    std::cout.rdbuf(old_cout);

    if (cpu.GetX() != 0x42) {
        return {false, "LDX immediate did not set X to 0x42"};
    }
    if (cpu.GetProgramCounter() != 0x8002) {
        return {false, "LDX immediate did not advance PC to $8002"};
    }

    return {true, "passed"};
}

} // namespace chris_pikul_mos6502_toolbox
