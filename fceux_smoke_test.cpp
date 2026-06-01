#include <fceux_toolbox.hpp>

#include <iostream>

int main()
{
    const auto result = fceux_toolbox::run_smoke_test();
    if (!result.passed) {
        std::cerr << "fceux smoke test failed: " << result.message << '\n';
        return 1;
    }

    std::cout << "fceux smoke test passed\n";
    return 0;
}
