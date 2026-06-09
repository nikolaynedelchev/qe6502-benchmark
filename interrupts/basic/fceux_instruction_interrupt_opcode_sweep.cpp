#include "common/instruction_interrupt_opcode_sweep.hpp"

#include <fceux_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_opcode_sweep(
        &fceux_toolbox::make_fceux_nes_instruction_cpu);
}
