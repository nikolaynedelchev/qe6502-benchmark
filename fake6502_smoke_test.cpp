#include <fake6502_toolbox.hpp>

#include <iostream>

int main()
{
    const auto result = fake6502_toolbox::run_smoke_test();
    if (!result.passed) {
        std::cerr << "fake6502 smoke test failed: " << result.message << '\n';
        return 1;
    }

    std::cout << "fake6502 smoke test passed\n";
    return 0;
}
