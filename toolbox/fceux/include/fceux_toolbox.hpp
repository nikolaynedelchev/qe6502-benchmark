#pragma once

#include <benchmark6502/results.hpp>

namespace fceux_toolbox {

benchmark6502::smoke_result run_smoke_test();
benchmark6502::klaus_benchmark_result run_klaus_nes_standard(int measured_runs = 5);

} // namespace fceux_toolbox
