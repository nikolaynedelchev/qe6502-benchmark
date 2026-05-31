#include <olcNES_toolbox.hpp>

#include <iostream>

int main()
{
    const auto result = olcNES_toolbox::run_smoke_test();
    if (!result.passed) {
        std::cerr << "olcNES smoke test failed: " << result.message << '\n';
        return 1;
    }

    std::cout << "olcNES smoke test passed\n";
    return 0;
}
