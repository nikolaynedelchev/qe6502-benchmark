#include <fake65c02_toolbox.hpp>
#include <qe6502_toolbox.hpp>
#include <sflib65c02_toolbox.hpp>
#include <vremu6502_toolbox.hpp>

#include <benchmark6502/results.hpp>
#include <benchmark6502/singlestep.hpp>

#include <cstdlib>
#include <exception>
#include <iomanip>
#include <iostream>
#include <string>

namespace {

struct all_65c02_options {
    bool run_smoke = true;
    bool run_klaus = true;
    bool run_singlestep = true;
};

void print_usage(const char* const program)
{
    std::cout << "Usage: " << program << " [options]\n"
              << "\n"
              << "Options:\n"
              << "  --singlestep-only       Skip smoke and Klaus/performance tests; run 65C02 SingleStep only.\n"
              << "  --singlestep-dir <dir>  Use an explicit SingleStepTests corpus root.\n"
              << "  --path <dir>            Alias for --singlestep-dir.\n"
              << "  --help                  Show this help.\n";
}

all_65c02_options parse_all_65c02_options(const int argc, char** argv)
{
    all_65c02_options options;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--singlestep-only") {
            options.run_smoke = false;
            options.run_klaus = false;
            options.run_singlestep = true;
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
    int harness_errors = 0;

    void record(const bool ok)
    {
        if (ok) {
            ++passed;
        } else {
            ++failed;
        }
    }

    void record_harness_error()
    {
        ++harness_errors;
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

void write_singlestep_log(const benchmark6502::singlestep_corpus& corpus,
                          const benchmark6502::singlestep_result& result,
                          test_summary& summary)
{
    try {
        const std::string log_path = benchmark6502::write_singlestep_detail_log(corpus, result);
        std::cout << "  detail log: " << log_path << "\n";
    } catch (const std::exception& e) {
        std::cout << "  detail log error: " << e.what() << "\n";
        summary.record_harness_error();
    }
}

void run_65c02_singlestep_one(const benchmark6502::singlestep_corpus& corpus,
                                  benchmark6502::singlestep_result (*runner)(const benchmark6502::singlestep_corpus&),
                                  const char* const label,
                                  test_summary& summary)
{
    std::cout << "\nStarting " << label << "...\n";
    const benchmark6502::singlestep_result result = runner(corpus);
    benchmark6502::print_singlestep_result(corpus, result);
    write_singlestep_log(corpus, result, summary);
    if (result.harness_error) {
        summary.record_harness_error();
    }
}

} // namespace

int main(int argc, char** argv)
{
    constexpr int measured_runs = 5;
    test_summary summary;

    const all_65c02_options options = parse_all_65c02_options(argc, argv);

    if (!options.run_smoke) {
        std::cout << "Skipping smoke tests.\n";
    }
    if (!options.run_klaus) {
        std::cout << "Skipping Klaus/performance tests.\n";
    }

    if (options.run_smoke) {
        print_smoke_result("fake65c02", fake65c02_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("fake65c02", "Klaus 65C02 ext", fake65c02_toolbox::run_klaus_65c02_extended(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("vrEmu6502", vremu6502_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("vrEmu6502 WDC 65C02", "Klaus standard", vremu6502_toolbox::run_klaus_wdc_65c02_standard(measured_runs), summary);
        print_klaus_result("vrEmu6502 WDC 65C02", "Klaus extended", vremu6502_toolbox::run_klaus_wdc_65c02_extended(measured_runs), summary);
        print_klaus_result("vrEmu6502 RW 65C02", "Klaus standard", vremu6502_toolbox::run_klaus_rockwell_65c02_standard(measured_runs), summary);
        print_klaus_result("vrEmu6502 RW 65C02", "Klaus extended", vremu6502_toolbox::run_klaus_rockwell_65c02_extended(measured_runs), summary);
    }

    if (options.run_smoke) {
        print_smoke_result("sflib65c02", sflib65c02_toolbox::run_smoke_test(), summary);
    }
    if (options.run_klaus) {
        print_klaus_result("sflib65c02", "Klaus 65C02 ext", sflib65c02_toolbox::run_klaus_65c02_extended(measured_runs), summary);
    }

    if (options.run_klaus) {
        print_klaus_result("qe6502 WDC 65C02", "Klaus standard", qe6502_toolbox::run_klaus_wdc_65c02_standard(measured_runs), summary);
        print_klaus_result("qe6502 WDC 65C02", "Klaus extended", qe6502_toolbox::run_klaus_wdc_65c02_extended(measured_runs), summary);
        print_klaus_result("qe6502 RW 65C02", "Klaus standard", qe6502_toolbox::run_klaus_rockwell_65c02_standard(measured_runs), summary);
        print_klaus_result("qe6502 RW 65C02", "Klaus extended", qe6502_toolbox::run_klaus_rockwell_65c02_extended(measured_runs), summary);
    }

    if (options.run_singlestep) {
        try {
            const benchmark6502::singlestep_corpus_options corpus_options = benchmark6502::parse_singlestep_corpus_options(argc, argv);

            std::cout << "\nLoading SingleStep WDC 65C02 corpus...\n";
            const benchmark6502::singlestep_corpus wdc_corpus = benchmark6502::load_singlestep_corpus(benchmark6502::singlestep_model::wdc65c02, corpus_options);
            std::cout << "Loaded SingleStep WDC 65C02 corpus: " << wdc_corpus.model_path
                      << "  cases=" << wdc_corpus.total_cases() << "\n";
            run_65c02_singlestep_one(wdc_corpus,
                                          qe6502_toolbox::run_singlestep_wdc65c02,
                                          "qe6502 WDC 65C02 SingleStep",
                                          summary);
            run_65c02_singlestep_one(wdc_corpus,
                                          vremu6502_toolbox::run_singlestep_wdc65c02,
                                          "vrEmu6502 WDC 65C02 SingleStep",
                                          summary);
            run_65c02_singlestep_one(wdc_corpus,
                                          fake65c02_toolbox::run_singlestep_wdc65c02,
                                          "fake65c02 WDC 65C02 SingleStep",
                                          summary);
            run_65c02_singlestep_one(wdc_corpus,
                                          sflib65c02_toolbox::run_singlestep_wdc65c02,
                                          "sflib65c02 WDC 65C02 SingleStep",
                                          summary);

            std::cout << "\nLoading SingleStep Rockwell 65C02 corpus...\n";
            const benchmark6502::singlestep_corpus rockwell_corpus = benchmark6502::load_singlestep_corpus(benchmark6502::singlestep_model::rockwell65c02, corpus_options);
            std::cout << "Loaded SingleStep Rockwell 65C02 corpus: " << rockwell_corpus.model_path
                      << "  cases=" << rockwell_corpus.total_cases() << "\n";
            run_65c02_singlestep_one(rockwell_corpus,
                                          qe6502_toolbox::run_singlestep_rockwell65c02,
                                          "qe6502 Rockwell 65C02 SingleStep",
                                          summary);
            run_65c02_singlestep_one(rockwell_corpus,
                                          vremu6502_toolbox::run_singlestep_rockwell65c02,
                                          "vrEmu6502 Rockwell 65C02 SingleStep",
                                          summary);
            run_65c02_singlestep_one(rockwell_corpus,
                                          fake65c02_toolbox::run_singlestep_rockwell65c02,
                                          "fake65c02 Rockwell 65C02 SingleStep",
                                          summary);
            run_65c02_singlestep_one(rockwell_corpus,
                                          sflib65c02_toolbox::run_singlestep_rockwell65c02,
                                          "sflib65c02 Rockwell 65C02 SingleStep",
                                          summary);

            std::cout << "\nLoading SingleStep Synertek 65C02 corpus...\n";
            const benchmark6502::singlestep_corpus synertek_corpus = benchmark6502::load_singlestep_corpus(benchmark6502::singlestep_model::synertek65c02, corpus_options);
            std::cout << "Loaded SingleStep Synertek 65C02 corpus: " << synertek_corpus.model_path
                      << "  cases=" << synertek_corpus.total_cases() << "\n";
            run_65c02_singlestep_one(synertek_corpus,
                                          qe6502_toolbox::run_singlestep_synertek65c02,
                                          "qe6502 Synertek/ST 65C02 SingleStep",
                                          summary);
            run_65c02_singlestep_one(synertek_corpus,
                                          vremu6502_toolbox::run_singlestep_synertek65c02,
                                          "vrEmu6502 Synertek/ST 65C02 SingleStep",
                                          summary);
            run_65c02_singlestep_one(synertek_corpus,
                                          fake65c02_toolbox::run_singlestep_synertek65c02,
                                          "fake65c02 Synertek/ST 65C02 SingleStep",
                                          summary);
            run_65c02_singlestep_one(synertek_corpus,
                                          sflib65c02_toolbox::run_singlestep_synertek65c02,
                                          "sflib65c02 Synertek/ST 65C02 SingleStep",
                                          summary);
        } catch (const std::exception& e) {
            std::cout << "\nSingleStep 65C02 harness error: " << e.what() << "\n";
            summary.record_harness_error();
        }
    }

    std::cout << "\nall_65c02_tests summary: " << summary.passed << " passed, " << summary.failed << " failed";
    if (summary.harness_errors != 0) {
        std::cout << ", " << summary.harness_errors << " harness errors";
    }
    std::cout << "\n";
    return summary.harness_errors == 0 ? 0 : 1;
}
