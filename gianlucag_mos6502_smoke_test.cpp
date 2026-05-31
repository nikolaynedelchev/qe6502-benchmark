#include <gianlucag_mos6502_toolbox.hpp>

#include <iostream>

int main()
{
    const benchmark6502::smoke_result result = gianlucag_mos6502_toolbox::run_smoke_test();
    if (!result.passed) {
        std::cerr << "gianlucag/mos6502 smoke test failed: " << result.message << '\n';
        return 1;
    }

    std::cout << "gianlucag/mos6502 smoke test passed\n";
    return 0;
}
