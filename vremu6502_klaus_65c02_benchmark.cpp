#include <vremu6502_toolbox.hpp>

#include <iomanip>
#include <iostream>

namespace {

int print_result(const char* const name, const benchmark6502::klaus_benchmark_result& result)
{
    if (!result.passed) {
        std::cerr << name << " failed\n";
        return 1;
    }

    std::cout << name << ": PASS\n"
              << "runs: " << result.measured_runs << " measured (+1 cold ignored)\n"
              << "cycles: " << result.bus_ticks << '\n'
              << "seconds: " << std::fixed << std::setprecision(6) << result.seconds << '\n'
              << "MHz: " << std::fixed << std::setprecision(2) << result.mhz() << "\n\n";
    return 0;
}

} // namespace

int main()
{
    int failed = 0;
    failed += print_result("vrEmu6502 WDC 65C02 Klaus standard", vremu6502_toolbox::run_klaus_wdc_65c02_standard(5));
    failed += print_result("vrEmu6502 WDC 65C02 Klaus extended", vremu6502_toolbox::run_klaus_wdc_65c02_extended(5));
    failed += print_result("vrEmu6502 Rockwell 65C02 Klaus standard", vremu6502_toolbox::run_klaus_rockwell_65c02_standard(5));
    failed += print_result("vrEmu6502 Rockwell 65C02 Klaus extended", vremu6502_toolbox::run_klaus_rockwell_65c02_extended(5));
    return failed == 0 ? 0 : 1;
}
