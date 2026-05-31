#pragma once

#include <cstdint>

namespace benchmark6502 {

struct smoke_result {
    bool passed = false;
    const char* message = "";
};

struct klaus_benchmark_result {
    bool passed = false;
    int measured_runs = 0;
    std::uint64_t bus_ticks = 0;
    double seconds = 0.0;

    double mhz() const
    {
        return seconds > 0.0 ? (static_cast<double>(bus_ticks) / seconds) / 1000000.0 : 0.0;
    }
};

} // namespace benchmark6502
