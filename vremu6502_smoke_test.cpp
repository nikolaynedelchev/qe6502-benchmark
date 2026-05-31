#include <vremu6502_toolbox.hpp>

#include <iostream>

int main()
{
    const auto result = vremu6502_toolbox::run_smoke_test();
    if (!result.passed) {
        std::cerr << "vrEmu6502 smoke test failed: " << result.message << '\n';
        return 1;
    }

    std::cout << "vrEmu6502 smoke test passed\n";
    return 0;
}
