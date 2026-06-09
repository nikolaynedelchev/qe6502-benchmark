#include "common/instruction_interrupt_group1.hpp"

#include <qe6502_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_group1(
        &qe6502_toolbox::make_qe6502_nmos_instruction_cpu);
}
