#include <fake65c02_toolbox.hpp>

#include <iostream>

int main()
{
    const auto result = fake65c02_toolbox::run_smoke_test();
    if (!result.passed) {
        std::cerr << "fake65c02 smoke test failed: " << result.message << '\n';
        return 1;
    }

    std::cout << "fake65c02 smoke test passed\n";
    return 0;
}
