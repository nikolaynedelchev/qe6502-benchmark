#include <altirrasdl_toolbox.hpp>
#include <iostream>
int main() {
    const auto result = altirrasdl_toolbox::run_smoke_test();
    if (!result.passed) {
        std::cerr << "altirrasdl smoke test failed: " << result.message << '\n';
        return 1;
    }
    std::cout << "altirrasdl smoke test passed\n";
    return 0;
}
