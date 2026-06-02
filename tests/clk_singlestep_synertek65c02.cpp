#include <clk_toolbox.hpp>

#include <benchmark6502/build_info.hpp>
#include <benchmark6502/singlestep.hpp>

#include <exception>
#include <iostream>

int main(int argc, char** argv)
{
    try {
        benchmark6502::print_current_build_runtime_info();
        const benchmark6502::singlestep_corpus_options options = benchmark6502::parse_singlestep_corpus_options(argc, argv);
        std::cout << "Loading SingleStep clk Synertek/ST 65C02 corpus...\n";
        const benchmark6502::singlestep_corpus corpus = benchmark6502::load_singlestep_corpus(benchmark6502::singlestep_model::synertek65c02, options);
        std::cout << "Loaded SingleStep clk Synertek/ST 65C02 corpus: " << corpus.model_path
                  << "  cases=" << corpus.total_cases() << "\n";
        std::cout << "Starting clk Synertek/ST 65C02 SingleStep...\n";
        const benchmark6502::singlestep_result result = clk_toolbox::run_singlestep_synertek65c02(corpus);
        benchmark6502::print_singlestep_result(corpus, result);
        const std::string log_path = benchmark6502::write_singlestep_detail_log(corpus, result);
        std::cout << "Wrote SingleStep detail log: " << log_path << "\n";
        return result.harness_error ? 1 : 0;
    } catch (const std::exception& e) {
        std::cerr << "SingleStep error: " << e.what() << '\n';
        return 1;
    }
}
