#include <sflib6502_toolbox.hpp>

#include <iostream>

int main()
{
    const auto result = sflib6502_toolbox::run_smoke_test();
    if (!result.passed) {
        std::cerr << "sflib6502 smoke test failed: " << result.message << '\n';
        return 1;
    }

    std::cout << "sflib6502 smoke test passed\n";
    return 0;
}
