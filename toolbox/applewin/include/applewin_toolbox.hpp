#pragma once

#include <applewin_toolbox/applewin_cpu.hpp>
#include <benchmark6502/results.hpp>

namespace applewin_toolbox {

benchmark6502::smoke_result run_smoke_test();
benchmark6502::klaus_benchmark_result run_klaus_nmos_standard(int measured_runs = 5);

} // namespace applewin_toolbox
