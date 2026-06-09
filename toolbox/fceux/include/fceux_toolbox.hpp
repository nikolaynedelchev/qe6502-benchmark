#pragma once

#include <memory>

#include <benchmark6502/results.hpp>
#include <benchmark6502/instruction_cpu.hpp>

namespace fceux_toolbox {

std::unique_ptr<benchmark6502::IInstructionCpu> make_fceux_nes_instruction_cpu();
benchmark6502::smoke_result run_smoke_test();
benchmark6502::klaus_benchmark_result run_klaus_nes_standard(int measured_runs = 5);

} // namespace fceux_toolbox
