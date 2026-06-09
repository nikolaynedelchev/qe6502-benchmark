#include "common/instruction_interrupt_group2.hpp"

#include <vremu6502_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_group2(
        &vremu6502_toolbox::make_vremu6502_nmos_instruction_cpu);
}
