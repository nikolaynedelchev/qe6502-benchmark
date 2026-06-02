#include <altirrasdl_toolbox.hpp>

#include <cstdlib>
#include <iomanip>
#include <iostream>

int main(int argc, char** argv)
{
    const int measured_runs = argc > 1 ? std::atoi(argv[1]) : 5;
    const auto result = altirrasdl_toolbox::run_klaus_nmos_standard(measured_runs);
    if (!result.passed) {
        std::cerr << "altirrasdl Klaus NMOS benchmark failed\n";
        return 1;
    }

    std::cout << "altirrasdl Klaus NMOS benchmark passed\n"
              << "runs: " << result.measured_runs << '\n'
              << "cycles: " << result.bus_ticks << '\n'
              << "seconds: " << std::fixed << std::setprecision(6) << result.seconds << '\n'
              << "MHz: " << std::fixed << std::setprecision(2) << result.mhz() << '\n';
    return 0;
}
