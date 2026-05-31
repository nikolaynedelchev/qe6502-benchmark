#pragma once

#include <benchmark6502/results.hpp>

namespace floooh_chips_toolbox {

benchmark6502::smoke_result run_smoke_test();
benchmark6502::klaus_benchmark_result run_klaus_nmos_standard(int measured_runs = 5);

} // namespace floooh_chips_toolbox
