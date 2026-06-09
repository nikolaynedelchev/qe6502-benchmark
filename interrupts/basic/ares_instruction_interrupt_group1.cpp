#include "common/instruction_interrupt_group1.hpp"

#include <ares_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_group1(
        &ares_toolbox::make_ares_nmos_instruction_cpu);
}
