#pragma once

#include <benchmark6502/results.hpp>
#include <benchmark6502/singlestep.hpp>

#include <memory>

namespace cpu6502_bridge {
class ICpu;
}

namespace clk_toolbox {

benchmark6502::smoke_result run_smoke_test();
benchmark6502::klaus_benchmark_result run_klaus_nmos_standard(int measured_runs = 5);
benchmark6502::klaus_benchmark_result run_klaus_wdc_65c02_standard(int measured_runs = 5);
benchmark6502::klaus_benchmark_result run_klaus_wdc_65c02_extended(int measured_runs = 5);
benchmark6502::klaus_benchmark_result run_klaus_rockwell_65c02_standard(int measured_runs = 5);
benchmark6502::klaus_benchmark_result run_klaus_rockwell_65c02_extended(int measured_runs = 5);
benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus);
benchmark6502::singlestep_result run_singlestep_wdc65c02(const benchmark6502::singlestep_corpus& corpus);
benchmark6502::singlestep_result run_singlestep_rockwell65c02(const benchmark6502::singlestep_corpus& corpus);
benchmark6502::singlestep_result run_singlestep_synertek65c02(const benchmark6502::singlestep_corpus& corpus);
std::unique_ptr<cpu6502_bridge::ICpu> make_clk_p6502_nmos_cpu();

} // namespace clk_toolbox
