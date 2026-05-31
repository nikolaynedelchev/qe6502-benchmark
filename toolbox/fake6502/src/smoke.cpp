#include <fake6502_toolbox.hpp>

extern "C" int fake6502_smoke_run(unsigned int* cycles_out);

namespace fake6502_toolbox {

benchmark6502::smoke_result run_smoke_test()
{
    unsigned int cycles = 0;
    const int status = fake6502_smoke_run(&cycles);

    if (status == 1) {
        return {false, "LDX immediate did not set X to 0x42"};
    }
    if (status == 2) {
        return {false, "LDX immediate did not report 2 cycles"};
    }
    if (status != 0) {
        return {false, "unexpected fake6502 smoke status"};
    }

    return {true, "passed"};
}

} // namespace fake6502_toolbox
