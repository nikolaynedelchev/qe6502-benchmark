#pragma once

#include <memory>

#include <benchmark6502/results.hpp>
#include <benchmark6502/instruction_cpu.hpp>
#include <benchmark6502/singlestep.hpp>

namespace ares_toolbox {

std::unique_ptr<benchmark6502::IInstructionCpu> make_ares_nmos_instruction_cpu();
benchmark6502::smoke_result run_smoke_test();
benchmark6502::klaus_benchmark_result run_klaus_nmos_standard(int measured_runs = 5);
benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus);

} // namespace ares_toolbox
