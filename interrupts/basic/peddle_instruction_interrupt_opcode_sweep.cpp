#include "common/instruction_interrupt_opcode_sweep.hpp"

#include <peddle_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_opcode_sweep(
        &peddle_toolbox::make_peddle_nmos_instruction_cpu);
}
