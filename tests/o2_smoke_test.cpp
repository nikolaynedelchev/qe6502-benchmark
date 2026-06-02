#include <o2_toolbox.hpp>

#include <iostream>

int main()
{
    const auto result = o2_toolbox::run_smoke_test();
    if (!result.passed) {
        std::cerr << "O2 smoke test failed: " << result.message << '\n';
        return 1;
    }

    std::cout << "O2 smoke test passed\n";
    return 0;
}
