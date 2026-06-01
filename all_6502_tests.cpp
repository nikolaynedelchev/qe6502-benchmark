#include <altirrasdl_toolbox.hpp>
#include <applewin_toolbox.hpp>
#include <ares_toolbox.hpp>
#include <chris_pikul_mos6502_toolbox.hpp>
#include <clk_toolbox.hpp>
#include <fake6502_toolbox.hpp>
#include <fceux_toolbox.hpp>
#include <floooh_chips_toolbox.hpp>
#include <gianlucag_mos6502_toolbox.hpp>
#include <mame_toolbox.hpp>
#include <o2_toolbox.hpp>
#include <olcNES_toolbox.hpp>
#include <peddle_toolbox.hpp>
#include <qe6502_toolbox.hpp>
#include <sflib6502_toolbox.hpp>
#include <vremu6502_toolbox.hpp>

#include <benchmark6502/results.hpp>
#include <benchmark6502/singlestep.hpp>

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

namespace {


struct all_6502_options {
    bool run_smoke = true;
    bool run_klaus = true;
    bool run_singlestep = true;
};

void print_usage(const char* const program)
{
    std::cout << "Usage: " << program << " [options]\n"
              << "\n"
              << "Options:\n"
              << "  --singlestep-only       Skip smoke and Klaus/performance tests; run NMOS SingleStep only.\n"
              << "  --skip-klaus            Skip Klaus/performance tests.\n"
              << "  --skip-smoke            Skip smoke tests.\n"
              << "  --skip-singlestep       Skip NMOS SingleStep tests.\n"
              << "  --singlestep-dir <dir>  Use an explicit SingleStepTests corpus root.\n"
              << "  --path <dir>            Alias for --singlestep-dir.\n"
              << "  --help                  Show this help.\n";
}

all_6502_options parse_all_6502_options(const int argc, char** argv)
{
    all_6502_options options;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--singlestep-only") {
            options.run_smoke = false;
            options.run_klaus = false;
            options.run_singlestep = true;
        } else if (arg == "--skip-klaus" || arg == "--no-klaus" || arg == "--skip-performance") {
            options.run_klaus = false;
        } else if (arg == "--skip-smoke" || arg == "--no-smoke") {
            options.run_smoke = false;
        } else if (arg == "--skip-singlestep" || arg == "--no-singlestep") {
            options.run_singlestep = false;
        } else if (arg == "--help" || arg == "-h") {
            print_usage(argv[0]);
            std::exit(0);
        } else if ((arg == "--singlestep-dir" || arg == "--path") && i + 1 < argc) {
            ++i;
        }
    }
    return options;
}

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

void print_klaus_nes_result(const char* const name, const benchmark6502::klaus_benchmark_result& result, test_summary& summary)
{
    summary.record(result.passed);
    std::cout << std::left << std::setw(28) << name
              << " Klaus NES         " << (result.passed ? "PASS" : "FAIL");
    if (result.passed) {
        std::cout << "  runs=" << result.measured_runs
                  << "  cycles=" << result.bus_ticks
                  << "  seconds=" << std::fixed << std::setprecision(6) << result.seconds
                  << "  MHz=" << std::fixed << std::setprecision(2) << result.mhz();
    }
    std::cout << '\n';
}

void write_singlestep_log(const benchmark6502::singlestep_corpus& corpus,
                          const benchmark6502::singlestep_result& result,
                          test_summary& summary)
{
    try {
        const std::string log_path = benchmark6502::write_singlestep_detail_log(corpus, result);
        std::cout << "Wrote SingleStep detail log: " << log_path << "\n";
    } catch (const std::exception& e) {
        std::cout << "SingleStep detail log error: " << e.what() << "\n";
        summary.record(false);
    }
}

} // namespace

int main(int argc, char** argv)
{
    constexpr int measured_runs = 5;
    test_summary summary;

    const all_6502_options options = parse_all_6502_options(argc, argv);

    if (!options.run_smoke) {
        std::cout << "Skipping smoke tests.\n";
    }
    if (!options.run_klaus) {
        std::cout << "Skipping Klaus/performance tests.\n";
    }
    if (!options.run_singlestep) {
        std::cout << "Skipping NMOS SingleStep tests.\n";
    }

    if (options.run_smoke) {
        print_smoke_result("qe6502", qe6502_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("qe6502", qe6502_toolbox::run_klaus_nmos_standard(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("floooh/chips", floooh_chips_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("floooh/chips", floooh_chips_toolbox::run_klaus_nmos_standard(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("gianlucag/mos6502", gianlucag_mos6502_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("gianlucag/mos6502", gianlucag_mos6502_toolbox::run_klaus_nmos_standard(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("fake6502", fake6502_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("fake6502", fake6502_toolbox::run_klaus_nmos_standard(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("vrEmu6502", vremu6502_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("vrEmu6502", vremu6502_toolbox::run_klaus_nmos_standard(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("O2", o2_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("O2", o2_toolbox::run_klaus_nmos_standard(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("Peddle", peddle_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("Peddle", peddle_toolbox::run_klaus_nmos_standard(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("sflib6502", sflib6502_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("sflib6502", sflib6502_toolbox::run_klaus_nmos_standard(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("olcNES", olcNES_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("olcNES", olcNES_toolbox::run_klaus_nmos_standard(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("chris-pikul/mos6502", chris_pikul_mos6502_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("chris-pikul/mos6502", chris_pikul_mos6502_toolbox::run_klaus_nmos_standard(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("fceux", fceux_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_nes_result("fceux", fceux_toolbox::run_klaus_nes_standard(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("clk", clk_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("clk", clk_toolbox::run_klaus_nmos_standard(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("ares", ares_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("ares", ares_toolbox::run_klaus_nmos_standard(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("altirrasdl", altirrasdl_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("altirrasdl", altirrasdl_toolbox::run_klaus_nmos_standard(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("applewin", applewin_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("applewin", applewin_toolbox::run_klaus_nmos_standard(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("mame", mame_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("mame", mame_toolbox::run_klaus_nmos_standard(measured_runs), summary);
    }

    if (options.run_singlestep) {
        try {
            const benchmark6502::singlestep_corpus_options corpus_options = benchmark6502::parse_singlestep_corpus_options(argc, argv);
            std::cout << "\nLoading SingleStep NMOS corpus...\n";
            const benchmark6502::singlestep_corpus corpus = benchmark6502::load_singlestep_corpus(benchmark6502::singlestep_model::nmos6502, corpus_options);
            std::cout << "Loaded SingleStep NMOS corpus: " << corpus.model_path
                      << "  cases=" << corpus.total_cases() << "\n\n";

            std::cout << "Starting qe6502 NMOS SingleStep...\n";
            const benchmark6502::singlestep_result qe6502_singlestep = qe6502_toolbox::run_singlestep_nmos(corpus);
            benchmark6502::print_singlestep_result(corpus, qe6502_singlestep);
            write_singlestep_log(corpus, qe6502_singlestep, summary);
            if (qe6502_singlestep.harness_error) {
                summary.record(false);
            }

            std::cout << "\nStarting floooh/chips NMOS SingleStep...\n";
            const benchmark6502::singlestep_result floooh_singlestep = floooh_chips_toolbox::run_singlestep_nmos(corpus);
            benchmark6502::print_singlestep_result(corpus, floooh_singlestep);
            write_singlestep_log(corpus, floooh_singlestep, summary);
            if (floooh_singlestep.harness_error) {
                summary.record(false);
            }

            std::cout << "\nStarting vrEmu6502 NMOS SingleStep...\n";
            const benchmark6502::singlestep_result vremu6502_singlestep = vremu6502_toolbox::run_singlestep_nmos(corpus);
            benchmark6502::print_singlestep_result(corpus, vremu6502_singlestep);
            write_singlestep_log(corpus, vremu6502_singlestep, summary);
            if (vremu6502_singlestep.harness_error) {
                summary.record(false);
            }

            std::cout << "\nStarting O2 NMOS SingleStep...\n";
            const benchmark6502::singlestep_result o2_singlestep = o2_toolbox::run_singlestep_nmos(corpus);
            benchmark6502::print_singlestep_result(corpus, o2_singlestep);
            write_singlestep_log(corpus, o2_singlestep, summary);
            if (o2_singlestep.harness_error) {
                summary.record(false);
            }

            std::cout << "\nStarting Peddle NMOS SingleStep...\n";
            const benchmark6502::singlestep_result peddle_singlestep = peddle_toolbox::run_singlestep_nmos(corpus);
            benchmark6502::print_singlestep_result(corpus, peddle_singlestep);
            write_singlestep_log(corpus, peddle_singlestep, summary);
            if (peddle_singlestep.harness_error) {
                summary.record(false);
            }

            std::cout << "\nStarting fake6502 NMOS SingleStep...\n";
            const benchmark6502::singlestep_result fake6502_singlestep = fake6502_toolbox::run_singlestep_nmos(corpus);
            benchmark6502::print_singlestep_result(corpus, fake6502_singlestep);
            write_singlestep_log(corpus, fake6502_singlestep, summary);
            if (fake6502_singlestep.harness_error) {
                summary.record(false);
            }


            std::cout << "\nStarting gianlucag/mos6502 NMOS SingleStep...\n";
            const benchmark6502::singlestep_result gianlucag_singlestep = gianlucag_mos6502_toolbox::run_singlestep_nmos(corpus);
            benchmark6502::print_singlestep_result(corpus, gianlucag_singlestep);
            write_singlestep_log(corpus, gianlucag_singlestep, summary);
            if (gianlucag_singlestep.harness_error) {
                summary.record(false);
            }

            std::cout << "\nStarting sflib6502 NMOS SingleStep...\n";
            const benchmark6502::singlestep_result sflib6502_singlestep = sflib6502_toolbox::run_singlestep_nmos(corpus);
            benchmark6502::print_singlestep_result(corpus, sflib6502_singlestep);
            write_singlestep_log(corpus, sflib6502_singlestep, summary);
            if (sflib6502_singlestep.harness_error) {
                summary.record(false);
            }

            std::cout << "\nStarting olcNES NMOS SingleStep...\n";
            const benchmark6502::singlestep_result olcNES_singlestep = olcNES_toolbox::run_singlestep_nmos(corpus);
            benchmark6502::print_singlestep_result(corpus, olcNES_singlestep);
            write_singlestep_log(corpus, olcNES_singlestep, summary);
            if (olcNES_singlestep.harness_error) {
                summary.record(false);
            }

            std::cout << "\nStarting chris-pikul/mos6502 NMOS SingleStep...\n";
            const benchmark6502::singlestep_result chris_pikul_singlestep = chris_pikul_mos6502_toolbox::run_singlestep_nmos(corpus);
            benchmark6502::print_singlestep_result(corpus, chris_pikul_singlestep);
            write_singlestep_log(corpus, chris_pikul_singlestep, summary);
            if (chris_pikul_singlestep.harness_error) {
                summary.record(false);
            }


            std::cout << "\nStarting clk NMOS SingleStep...\n";
            const benchmark6502::singlestep_result clk_singlestep = clk_toolbox::run_singlestep_nmos(corpus);
            benchmark6502::print_singlestep_result(corpus, clk_singlestep);
            write_singlestep_log(corpus, clk_singlestep, summary);
            if (clk_singlestep.harness_error) {
                summary.record(false);
            }

            std::cout << "\nStarting ares NMOS SingleStep...\n";
            const benchmark6502::singlestep_result ares_singlestep = ares_toolbox::run_singlestep_nmos(corpus);
            benchmark6502::print_singlestep_result(corpus, ares_singlestep);
            write_singlestep_log(corpus, ares_singlestep, summary);
            if (ares_singlestep.harness_error) {
                summary.record(false);
            }

            std::cout << "\nStarting altirrasdl NMOS SingleStep...\n";
            const benchmark6502::singlestep_result altirrasdl_singlestep = altirrasdl_toolbox::run_singlestep_nmos(corpus);
            benchmark6502::print_singlestep_result(corpus, altirrasdl_singlestep);
            write_singlestep_log(corpus, altirrasdl_singlestep, summary);
            if (altirrasdl_singlestep.harness_error) {
                summary.record(false);
            }

            std::cout << "\nStarting applewin NMOS SingleStep...\n";
            const benchmark6502::singlestep_result applewin_singlestep = applewin_toolbox::run_singlestep_nmos(corpus);
            benchmark6502::print_singlestep_result(corpus, applewin_singlestep);
            write_singlestep_log(corpus, applewin_singlestep, summary);
            if (applewin_singlestep.harness_error) {
                summary.record(false);
            }

            std::cout << "\nStarting mame NMOS SingleStep...\n";
            const benchmark6502::singlestep_result mame_singlestep = mame_toolbox::run_singlestep_nmos(corpus);
            benchmark6502::print_singlestep_result(corpus, mame_singlestep);
            write_singlestep_log(corpus, mame_singlestep, summary);
            if (mame_singlestep.harness_error) {
                summary.record(false);
            }
        } catch (const std::exception& e) {
            std::cout << "\nSingleStep NMOS harness error: " << e.what() << "\n";
            summary.record(false);
        }
    }

    std::cout << "\nall_6502_tests summary: " << summary.passed << " passed, " << summary.failed << " failed\n";
    return summary.failed == 0 ? 0 : 1;
}
