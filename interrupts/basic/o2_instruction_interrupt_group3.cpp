#include "common/instruction_interrupt_group3.hpp"

#include <o2_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_group3(
        &o2_toolbox::make_o2_nmos_instruction_cpu);
}
