#include "common/instruction_interrupt_opcode_sweep.hpp"

#include <chris_pikul_mos6502_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_opcode_sweep(
        &chris_pikul_mos6502_toolbox::make_chris_pikul_mos6502_nmos_instruction_cpu);
}
