#include "common/instruction_interrupt_group1.hpp"

#include <fceux_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_group1(
        &fceux_toolbox::make_fceux_nes_instruction_cpu);
}
