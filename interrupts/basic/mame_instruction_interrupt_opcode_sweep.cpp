#include "common/instruction_interrupt_opcode_sweep.hpp"

#include <mame_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_opcode_sweep(
        &mame_toolbox::make_mame_nmos_instruction_cpu);
}
