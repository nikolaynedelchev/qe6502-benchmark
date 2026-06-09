#include "common/instruction_interrupt_group1.hpp"

#include <gianlucag_mos6502_toolbox.hpp>

int main()
{
    const benchmark6502::interrupts::basic::InstructionCpuFactory factory =
        &gianlucag_mos6502_toolbox::make_gianlucag_mos6502_nmos_instruction_cpu;
    return benchmark6502::interrupts::basic::run_instruction_interrupt_group1(factory);
}
