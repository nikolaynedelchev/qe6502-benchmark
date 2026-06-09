#pragma once

#include <fceux_toolbox.hpp>

#include "common/instruction_interrupt_group1.hpp"
#include "common/instruction_interrupt_group2.hpp"
#include "common/instruction_interrupt_group3.hpp"
#include "common/instruction_interrupt_opcode_sweep.hpp"

namespace fceux_toolbox {

inline int run_fceux_instruction_interrupt_basic_tests()
{
    const benchmark6502::interrupts::basic::InstructionCpuFactory factory =
        &make_fceux_nes_instruction_cpu;

    int result = 0;
    result |= benchmark6502::interrupts::basic::run_instruction_interrupt_group1(factory);
    result |= benchmark6502::interrupts::basic::run_instruction_interrupt_group2(factory);
    result |= benchmark6502::interrupts::basic::run_instruction_interrupt_group3(factory);
    result |= benchmark6502::interrupts::basic::run_instruction_interrupt_opcode_sweep(factory);
    return result;
}

} // namespace fceux_toolbox
