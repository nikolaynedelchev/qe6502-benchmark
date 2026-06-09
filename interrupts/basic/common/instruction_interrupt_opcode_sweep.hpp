#pragma once

#include "instruction_interrupt_group1.hpp"

namespace benchmark6502::interrupts::basic {

int run_instruction_interrupt_opcode_sweep(const InstructionCpuFactory& factory);

} // namespace benchmark6502::interrupts::basic
