#include <fake65c02_toolbox.hpp>

#include <iomanip>
#include <iostream>

int main()
{
    const benchmark6502::klaus_benchmark_result result = fake65c02_toolbox::run_klaus_65c02_extended(5);
    if (!result.passed) {
        std::cerr << "fake65c02 Klaus2m5 65C02 extended benchmark failed\n";
        return 1;
    }

    std::cout << "fake65c02 Klaus2m5 65C02 extended: PASS\n"
              << "runs: " << result.measured_runs << " measured (+1 cold ignored)\n"
              << "cycles: " << result.bus_ticks << '\n'
              << "seconds: " << std::fixed << std::setprecision(6) << result.seconds << '\n'
              << "MHz: " << std::fixed << std::setprecision(2) << result.mhz() << '\n';

    return 0;
}
