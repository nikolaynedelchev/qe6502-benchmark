#pragma once

#include <floooh_chips_toolbox.hpp>

#include "common/instruction_interrupt_group1.hpp"
#include "common/instruction_interrupt_group2.hpp"
#include "common/instruction_interrupt_group3.hpp"
#include "common/instruction_interrupt_opcode_sweep.hpp"

namespace floooh_chips_toolbox {

inline int run_floooh_chips_instruction_interrupt_basic_tests()
{
    const benchmark6502::interrupts::basic::InstructionCpuFactory factory =
        &make_floooh_chips_nmos_instruction_cpu;

    int result = 0;
    result |= benchmark6502::interrupts::basic::run_instruction_interrupt_group1(factory);
    result |= benchmark6502::interrupts::basic::run_instruction_interrupt_group2(factory);
    result |= benchmark6502::interrupts::basic::run_instruction_interrupt_group3(factory);
    result |= benchmark6502::interrupts::basic::run_instruction_interrupt_opcode_sweep(factory);
    return result;
}

} // namespace floooh_chips_toolbox
