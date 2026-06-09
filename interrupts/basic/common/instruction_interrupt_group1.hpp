#pragma once

#include <benchmark6502/instruction_cpu.hpp>

#include <memory>

namespace benchmark6502::interrupts::basic {

using InstructionCpuFactory = std::unique_ptr<IInstructionCpu> (*)();

int run_instruction_interrupt_group1(const InstructionCpuFactory& factory);

} // namespace benchmark6502::interrupts::basic
