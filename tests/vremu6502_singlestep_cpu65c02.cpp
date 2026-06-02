#include <vremu6502_toolbox.hpp>

#include <benchmark6502/singlestep.hpp>

#include <exception>
#include <iostream>

namespace {

bool run_one(const benchmark6502::singlestep_model model,
             const benchmark6502::singlestep_corpus_options& options)
{
    try {
        const benchmark6502::singlestep_corpus corpus = benchmark6502::load_singlestep_corpus(model, options);
        const benchmark6502::singlestep_result result = vremu6502_toolbox::run_singlestep_cpu65c02(corpus);
        benchmark6502::print_singlestep_result(corpus, result);
        const std::string detail_log = benchmark6502::write_singlestep_detail_log(corpus, result);
        std::cout << "  detail log: " << detail_log << "\n\n";
        return !result.harness_error;
    } catch (const std::exception& ex) {
        std::cerr << "vrEmu6502 CPU_65C02 SingleStep failed for "
                  << benchmark6502::singlestep_model_name(model) << ": " << ex.what() << "\n";
        return false;
    }
}

} // namespace

int main(int argc, char** argv)
{
    const benchmark6502::singlestep_corpus_options options =
        benchmark6502::parse_singlestep_corpus_options(argc, argv);

    bool ok = true;
    ok = run_one(benchmark6502::singlestep_model::wdc65c02, options) && ok;
    ok = run_one(benchmark6502::singlestep_model::rockwell65c02, options) && ok;
    ok = run_one(benchmark6502::singlestep_model::synertek65c02, options) && ok;

    return ok ? 0 : 1;
}
