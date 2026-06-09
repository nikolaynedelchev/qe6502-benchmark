#include "common/instruction_interrupt_group2.hpp"

#include <sflib65c02_toolbox.hpp>

int main()
{
    return benchmark6502::interrupts::basic::run_instruction_interrupt_group2(
        &sflib65c02_toolbox::make_sflib65c02_instruction_cpu);
}
