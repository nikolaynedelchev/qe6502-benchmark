#include <o2_toolbox.hpp>

#include <iomanip>
#include <iostream>

int main()
{
    const benchmark6502::klaus_benchmark_result result = o2_toolbox::run_klaus_nmos_standard(5);
    if (!result.passed) {
        std::cerr << "O2 Klaus2m5 NMOS benchmark failed\n";
        return 1;
    }

    std::cout << "O2 Klaus2m5 NMOS standard: PASS\n"
              << "runs: " << result.measured_runs << " measured (+1 cold ignored)\n"
              << "cycles: " << result.bus_ticks << '\n'
              << "seconds: " << std::fixed << std::setprecision(6) << result.seconds << '\n'
              << "MHz: " << std::fixed << std::setprecision(2) << result.mhz() << '\n';

    return 0;
}
