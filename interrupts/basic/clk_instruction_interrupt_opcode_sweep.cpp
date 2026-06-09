#include "common/instruction_interrupt_opcode_sweep.hpp"

#include <clk_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_opcode_sweep(
        &clk_toolbox::make_clk_p6502_nmos_instruction_cpu);
}
