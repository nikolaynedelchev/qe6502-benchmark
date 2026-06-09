#include "common/instruction_interrupt_group1.hpp"

#include <applewin_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_group1(
        &applewin_toolbox::make_applewin_nmos_instruction_cpu);
}
