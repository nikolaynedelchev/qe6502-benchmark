#pragma once

#include <benchmark6502/results.hpp>

namespace sflib65c02_toolbox {

benchmark6502::smoke_result run_smoke_test();
benchmark6502::klaus_benchmark_result run_klaus_65c02_extended(int measured_runs);

} // namespace sflib65c02_toolbox
