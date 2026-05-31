#include <qe6502_toolbox.hpp>

#include <qe6502/cpu.hpp>

namespace qe6502_toolbox {

benchmark6502::smoke_result run_smoke_test()
{
    qe6502::cpu cpu(qe6502::model::nmos);
    cpu.jump_to(0x8000u);

    const qe6502_tick_t& opcode_tick = cpu.raw_tick();
    if (opcode_tick.address != 0x8000u || !cpu.is_opcode_fetch()) {
        return {false, "expected opcode fetch at $8000"};
    }

    const qe6502_tick_t& operand_tick = cpu.tick(0xa2u); // LDX #imm
    if (operand_tick.address != 0x8001u || cpu.is_write()) {
        return {false, "expected operand read at $8001"};
    }

    const qe6502_tick_t& next_opcode_tick = cpu.tick(0x42u);
    if (cpu.x() != 0x42u || next_opcode_tick.address != 0x8002u || !cpu.is_opcode_fetch()) {
        return {false, "LDX immediate did not complete"};
    }

    return {true, "passed"};
}

} // namespace qe6502_toolbox
