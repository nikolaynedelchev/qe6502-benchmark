#include <clk_toolbox.hpp>

#include <iostream>

int main()
{
    const auto result = clk_toolbox::run_smoke_test();
    if (!result.passed) {
        std::cerr << "clk smoke test failed: " << result.message << '\n';
        return 1;
    }

    std::cout << "clk smoke test passed\n";
    return 0;
}
