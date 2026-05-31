#pragma once

#include <benchmark6502/results.hpp>
#include <benchmark6502/singlestep.hpp>

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

} // namespace vremu6502_toolbox
