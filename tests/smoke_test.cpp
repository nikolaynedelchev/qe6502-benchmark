#include <qe6502_toolbox.hpp>

#include <iostream>

int main()
{
    const benchmark6502::smoke_result result = qe6502_toolbox::run_smoke_test();
    if (!result.passed) {
        std::cerr << "qe6502 C++ smoke test failed: " << result.message << '\n';
        return 1;
    }

    std::cout << "qe6502 C++ smoke test passed\n";
    return 0;
}
