#include "common/instruction_interrupt_opcode_sweep.hpp"

#include <sflib6502_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_opcode_sweep(
        &sflib6502_toolbox::make_sflib6502_nmos_instruction_cpu);
}
