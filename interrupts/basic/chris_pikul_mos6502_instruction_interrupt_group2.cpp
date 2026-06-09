#include "common/instruction_interrupt_group2.hpp"

#include <chris_pikul_mos6502_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_group2(
        &chris_pikul_mos6502_toolbox::make_chris_pikul_mos6502_nmos_instruction_cpu);
}
