#include "common/instruction_interrupt_group3.hpp"

#include <fake6502_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_group3(
        &fake6502_toolbox::make_fake6502_nmos_instruction_cpu);
}
