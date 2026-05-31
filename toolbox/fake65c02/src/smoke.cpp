#include <fake65c02_toolbox.hpp>

extern "C" int fake65c02_smoke_run(unsigned int* cycles_out);

namespace fake65c02_toolbox {

benchmark6502::smoke_result run_smoke_test()
{
    unsigned int cycles = 0;
    const int status = fake65c02_smoke_run(&cycles);

    if (status == 1) {
        return {false, "STZ absolute did not clear memory"};
    }
    if (status == 2) {
        return {false, "BRA did not branch over the skipped LDA"};
    }
    if (status != 0) {
        return {false, "unexpected fake65c02 smoke status"};
    }

    return {true, "passed"};
}

} // namespace fake65c02_toolbox
