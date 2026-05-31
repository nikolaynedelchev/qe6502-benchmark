#include <fake65c02_toolbox.hpp>
#include <qe6502_toolbox.hpp>
#include <sflib65c02_toolbox.hpp>

#include <benchmark6502/results.hpp>

#include <iomanip>
#include <iostream>

namespace {

struct test_summary {
    int passed = 0;
    int failed = 0;

    void record(const bool ok)
    {
        if (ok) {
            ++passed;
        } else {
            ++failed;
        }
    }
};

void print_smoke_result(const char* const name, const benchmark6502::smoke_result& result, test_summary& summary)
{
    summary.record(result.passed);
    std::cout << std::left << std::setw(30) << name
              << " smoke              " << (result.passed ? "PASS" : "FAIL");
    if (!result.passed && result.message != nullptr && result.message[0] != '\0') {
        std::cout << "  " << result.message;
    }
    std::cout << '\n';
}

void print_klaus_result(const char* const name,
                        const char* const test_name,
                        const benchmark6502::klaus_benchmark_result& result,
                        test_summary& summary)
{
    summary.record(result.passed);
    std::cout << std::left << std::setw(30) << name
              << ' ' << std::left << std::setw(18) << test_name
              << (result.passed ? "PASS" : "FAIL");
    if (result.passed) {
        std::cout << "  runs=" << result.measured_runs
                  << "  cycles=" << result.bus_ticks
                  << "  seconds=" << std::fixed << std::setprecision(6) << result.seconds
                  << "  MHz=" << std::fixed << std::setprecision(2) << result.mhz();
    }
    std::cout << '\n';
}

} // namespace

int main()
{
    constexpr int measured_runs = 5;
    test_summary summary;

    print_smoke_result("fake65c02", fake65c02_toolbox::run_smoke_test(), summary);
    print_klaus_result("fake65c02", "Klaus 65C02 ext", fake65c02_toolbox::run_klaus_65c02_extended(measured_runs), summary);

    print_smoke_result("sflib65c02", sflib65c02_toolbox::run_smoke_test(), summary);
    print_klaus_result("sflib65c02", "Klaus 65C02 ext", sflib65c02_toolbox::run_klaus_65c02_extended(measured_runs), summary);

    print_klaus_result("qe6502 WDC 65C02", "Klaus standard", qe6502_toolbox::run_klaus_wdc_65c02_standard(measured_runs), summary);
    print_klaus_result("qe6502 WDC 65C02", "Klaus extended", qe6502_toolbox::run_klaus_wdc_65c02_extended(measured_runs), summary);
    print_klaus_result("qe6502 RW 65C02", "Klaus standard", qe6502_toolbox::run_klaus_rockwell_65c02_standard(measured_runs), summary);
    print_klaus_result("qe6502 RW 65C02", "Klaus extended", qe6502_toolbox::run_klaus_rockwell_65c02_extended(measured_runs), summary);

    std::cout << "\nall_65c02_tests summary: " << summary.passed << " passed, " << summary.failed << " failed\n";
    return summary.failed == 0 ? 0 : 1;
}
