#pragma once

#include <benchmark6502/instruction_cpu.hpp>
#include <benchmark6502/results.hpp>
#include <benchmark6502/singlestep.hpp>

#include <memory>

namespace vremu6502_toolbox {

enum class klaus_model {
    nmos,
    wdc,
    rockwell,
};

enum class klaus_test {
    standard,
    extended,
};

benchmark6502::smoke_result run_smoke_test();
benchmark6502::klaus_benchmark_result run_klaus(klaus_model model, klaus_test test, int measured_runs = 5);
benchmark6502::klaus_benchmark_result run_klaus_nmos_standard(int measured_runs = 5);
benchmark6502::klaus_benchmark_result run_klaus_wdc_65c02_standard(int measured_runs = 5);
benchmark6502::klaus_benchmark_result run_klaus_wdc_65c02_extended(int measured_runs = 5);
benchmark6502::klaus_benchmark_result run_klaus_rockwell_65c02_standard(int measured_runs = 5);
benchmark6502::klaus_benchmark_result run_klaus_rockwell_65c02_extended(int measured_runs = 5);
benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus);
benchmark6502::singlestep_result run_singlestep_cpu65c02(const benchmark6502::singlestep_corpus& corpus);
benchmark6502::singlestep_result run_singlestep_wdc65c02(const benchmark6502::singlestep_corpus& corpus);
benchmark6502::singlestep_result run_singlestep_rockwell65c02(const benchmark6502::singlestep_corpus& corpus);
benchmark6502::singlestep_result run_singlestep_synertek65c02(const benchmark6502::singlestep_corpus& corpus);
std::unique_ptr<benchmark6502::IInstructionCpu> make_vremu6502_nmos_instruction_cpu();

} // namespace vremu6502_toolbox
