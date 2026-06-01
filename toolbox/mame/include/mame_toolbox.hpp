#pragma once

#include <benchmark6502/mame/mame6502_cpu.hpp>
#include <benchmark6502/results.hpp>
#include <benchmark6502/singlestep.hpp>

namespace mame_toolbox {

benchmark6502::smoke_result run_smoke_test();
benchmark6502::klaus_benchmark_result run_klaus_nmos_standard(int measured_runs = 5);
benchmark6502::klaus_benchmark_result run_klaus_wdc_65c02_standard(int measured_runs = 5);
benchmark6502::klaus_benchmark_result run_klaus_wdc_65c02_extended(int measured_runs = 5);
benchmark6502::klaus_benchmark_result run_klaus_rockwell_65c02_standard(int measured_runs = 5);
benchmark6502::klaus_benchmark_result run_klaus_rockwell_65c02_extended(int measured_runs = 5);
benchmark6502::klaus_benchmark_result run_klaus_wdc65c02s_standard(int measured_runs = 5);
benchmark6502::klaus_benchmark_result run_klaus_wdc65c02s_extended(int measured_runs = 5);
benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus);
benchmark6502::singlestep_result run_singlestep_wdc65c02(const benchmark6502::singlestep_corpus& corpus);
benchmark6502::singlestep_result run_singlestep_rockwell65c02(const benchmark6502::singlestep_corpus& corpus);
benchmark6502::singlestep_result run_singlestep_wdc65c02s(const benchmark6502::singlestep_corpus& corpus);

} // namespace mame_toolbox
