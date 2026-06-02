#include <ares_toolbox.hpp>

#include <iostream>

int main()
{
    const auto result = ares_toolbox::run_smoke_test();
    if (!result.passed) {
        std::cerr << "ares smoke test failed: " << result.message << '\n';
        return 1;
    }

    std::cout << "ares smoke test passed\n";
    return 0;
}
