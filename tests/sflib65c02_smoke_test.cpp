#include <sflib65c02_toolbox.hpp>

#include <iostream>

int main()
{
    const auto result = sflib65c02_toolbox::run_smoke_test();
    if (!result.passed) {
        std::cerr << "sflib65c02 smoke test failed: " << result.message << '\n';
        return 1;
    }

    std::cout << "sflib65c02 smoke test passed\n";
    return 0;
}
