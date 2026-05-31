#include <qe6502_toolbox.hpp>

#include <benchmark6502/singlestep.hpp>

#include <exception>
#include <iostream>

int main(int argc, char** argv)
{
    try {
        const benchmark6502::singlestep_corpus_options options = benchmark6502::parse_singlestep_corpus_options(argc, argv);
        const benchmark6502::singlestep_corpus corpus = benchmark6502::load_singlestep_corpus(benchmark6502::singlestep_model::nmos6502, options);
        const benchmark6502::singlestep_result result = qe6502_toolbox::run_singlestep_nmos(corpus);
        benchmark6502::print_singlestep_result(corpus, result);
        return result.harness_error ? 1 : 0;
    } catch (const std::exception& e) {
        std::cerr << "SingleStep error: " << e.what() << '\n';
        return 1;
    }
}
