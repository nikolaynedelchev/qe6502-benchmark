#pragma once

#include <benchmark6502/instruction_cpu.hpp>
#include <benchmark6502/results.hpp>
#include <benchmark6502/singlestep.hpp>

#include <memory>

namespace cpu6502_bridge {
class ICpu;
}

namespace o2_toolbox {

benchmark6502::smoke_result run_smoke_test();
benchmark6502::klaus_benchmark_result run_klaus_nmos_standard(int measured_runs = 5);
benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus);
std::unique_ptr<benchmark6502::IInstructionCpu> make_o2_nmos_instruction_cpu();
std::unique_ptr<cpu6502_bridge::ICpu> make_o2_nmos_cpu();

} // namespace o2_toolbox
