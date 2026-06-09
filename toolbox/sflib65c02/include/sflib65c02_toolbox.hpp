#pragma once

#include <memory>

#include <benchmark6502/results.hpp>
#include <benchmark6502/instruction_cpu.hpp>
#include <benchmark6502/singlestep.hpp>

namespace sflib65c02_toolbox {

std::unique_ptr<benchmark6502::IInstructionCpu> make_sflib65c02_instruction_cpu();
benchmark6502::smoke_result run_smoke_test();
benchmark6502::klaus_benchmark_result run_klaus_65c02_extended(int measured_runs);

benchmark6502::singlestep_result run_singlestep_wdc65c02(const benchmark6502::singlestep_corpus& corpus);
benchmark6502::singlestep_result run_singlestep_rockwell65c02(const benchmark6502::singlestep_corpus& corpus);
benchmark6502::singlestep_result run_singlestep_synertek65c02(const benchmark6502::singlestep_corpus& corpus);

} // namespace sflib65c02_toolbox
