#include "common/instruction_interrupt_group2.hpp"

#include <floooh_chips_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_group2(
        &floooh_chips_toolbox::make_floooh_chips_nmos_instruction_cpu);
}
