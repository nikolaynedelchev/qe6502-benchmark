#include <chris_pikul_mos6502_toolbox.hpp>
#include <fake6502_toolbox.hpp>
#include <floooh_chips_toolbox.hpp>
#include <gianlucag_mos6502_toolbox.hpp>
#include <o2_toolbox.hpp>
#include <olcNES_toolbox.hpp>
#include <peddle_toolbox.hpp>
#include <qe6502_toolbox.hpp>
#include <sflib6502_toolbox.hpp>
#include <vremu6502_toolbox.hpp>

#include <benchmark6502/results.hpp>
#include <benchmark6502/singlestep.hpp>

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
    std::cout << std::left << std::setw(28) << name
              << " smoke             " << (result.passed ? "PASS" : "FAIL");
    if (!result.passed && result.message != nullptr && result.message[0] != '\0') {
        std::cout << "  " << result.message;
    }
    std::cout << '\n';
}

void print_klaus_result(const char* const name, const benchmark6502::klaus_benchmark_result& result, test_summary& summary)
{
    summary.record(result.passed);
    std::cout << std::left << std::setw(28) << name
              << " Klaus NMOS        " << (result.passed ? "PASS" : "FAIL");
    if (result.passed) {
        std::cout << "  runs=" << result.measured_runs
                  << "  cycles=" << result.bus_ticks
                  << "  seconds=" << std::fixed << std::setprecision(6) << result.seconds
                  << "  MHz=" << std::fixed << std::setprecision(2) << result.mhz();
    }
    std::cout << '\n';
}

} // namespace

int main(int argc, char** argv)
{
    constexpr int measured_runs = 5;
    test_summary summary;

    print_smoke_result("qe6502", qe6502_toolbox::run_smoke_test(), summary);
    print_klaus_result("qe6502", qe6502_toolbox::run_klaus_nmos_standard(measured_runs), summary);

    print_smoke_result("floooh/chips", floooh_chips_toolbox::run_smoke_test(), summary);
    print_klaus_result("floooh/chips", floooh_chips_toolbox::run_klaus_nmos_standard(measured_runs), summary);

    print_smoke_result("gianlucag/mos6502", gianlucag_mos6502_toolbox::run_smoke_test(), summary);
    print_klaus_result("gianlucag/mos6502", gianlucag_mos6502_toolbox::run_klaus_nmos_standard(measured_runs), summary);

    print_smoke_result("fake6502", fake6502_toolbox::run_smoke_test(), summary);
    print_klaus_result("fake6502", fake6502_toolbox::run_klaus_nmos_standard(measured_runs), summary);

    print_smoke_result("vrEmu6502", vremu6502_toolbox::run_smoke_test(), summary);
    print_klaus_result("vrEmu6502", vremu6502_toolbox::run_klaus_nmos_standard(measured_runs), summary);

    print_smoke_result("O2", o2_toolbox::run_smoke_test(), summary);
    print_klaus_result("O2", o2_toolbox::run_klaus_nmos_standard(measured_runs), summary);

    print_smoke_result("Peddle", peddle_toolbox::run_smoke_test(), summary);
    print_klaus_result("Peddle", peddle_toolbox::run_klaus_nmos_standard(measured_runs), summary);

    print_smoke_result("sflib6502", sflib6502_toolbox::run_smoke_test(), summary);
    print_klaus_result("sflib6502", sflib6502_toolbox::run_klaus_nmos_standard(measured_runs), summary);

    print_smoke_result("olcNES", olcNES_toolbox::run_smoke_test(), summary);
    print_klaus_result("olcNES", olcNES_toolbox::run_klaus_nmos_standard(measured_runs), summary);

    print_smoke_result("chris-pikul/mos6502", chris_pikul_mos6502_toolbox::run_smoke_test(), summary);
    print_klaus_result("chris-pikul/mos6502", chris_pikul_mos6502_toolbox::run_klaus_nmos_standard(measured_runs), summary);

    try {
        const benchmark6502::singlestep_corpus_options corpus_options = benchmark6502::parse_singlestep_corpus_options(argc, argv);
        const benchmark6502::singlestep_corpus corpus = benchmark6502::load_singlestep_corpus(benchmark6502::singlestep_model::nmos6502, corpus_options);
        std::cout << "\nSingleStep NMOS corpus loaded once: " << corpus.model_path
                  << "  cases=" << corpus.total_cases() << "\n\n";

        const benchmark6502::singlestep_result qe6502_singlestep = qe6502_toolbox::run_singlestep_nmos(corpus);
        benchmark6502::print_singlestep_result(corpus, qe6502_singlestep);
        if (qe6502_singlestep.harness_error) {
            summary.record(false);
        }
    } catch (const std::exception& e) {
        std::cout << "\nSingleStep NMOS harness error: " << e.what() << "\n";
        summary.record(false);
    }

    std::cout << "\nall_6502_tests summary: " << summary.passed << " passed, " << summary.failed << " failed\n";
    return summary.failed == 0 ? 0 : 1;
}
