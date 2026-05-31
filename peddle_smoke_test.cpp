#include <peddle_toolbox.hpp>

#include <iostream>

int main()
{
    const auto result = peddle_toolbox::run_smoke_test();
    if (!result.passed) {
        std::cerr << "Peddle smoke test failed: " << result.message << '\n';
        return 1;
    }

    std::cout << "Peddle smoke test passed\n";
    return 0;
}
