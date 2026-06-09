#include "common/instruction_interrupt_group3.hpp"

#include <clk_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_group3(
        &clk_toolbox::make_clk_p6502_nmos_instruction_cpu);
}
