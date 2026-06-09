#include "common/instruction_interrupt_group3.hpp"

#include <olcNES_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_group3(
        &olcNES_toolbox::make_olcNES_nmos_instruction_cpu);
}
