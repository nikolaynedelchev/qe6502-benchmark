#include <floooh_chips_toolbox.hpp>

#include <iostream>

int main()
{
    const benchmark6502::smoke_result result = floooh_chips_toolbox::run_smoke_test();
    if (!result.passed) {
        std::cerr << "floooh/chips m6502 smoke test failed: " << result.message << '\n';
        return 1;
    }

    std::cout << "floooh/chips m6502 smoke test passed\n";
    return 0;
}
