#pragma once

#include <sflib65c02_toolbox.hpp>

#include "common/instruction_interrupt_group1.hpp"
#include "common/instruction_interrupt_group2.hpp"
#include "common/instruction_interrupt_group3.hpp"
#include "common/instruction_interrupt_opcode_sweep.hpp"

namespace sflib65c02_toolbox {

inline int run_sflib65c02_instruction_interrupt_basic_tests()
{
    const benchmark6502::interrupts::basic::InstructionCpuFactory factory =
        &make_sflib65c02_instruction_cpu;

    int result = 0;
    result |= benchmark6502::interrupts::basic::run_instruction_interrupt_group1(factory);
    result |= benchmark6502::interrupts::basic::run_instruction_interrupt_group2(factory);
    result |= benchmark6502::interrupts::basic::run_instruction_interrupt_group3(factory);
    result |= benchmark6502::interrupts::basic::run_instruction_interrupt_opcode_sweep(factory);
    return result;
}

} // namespace sflib65c02_toolbox
