#include <chris_pikul_mos6502_toolbox.hpp>

#include <cstdio>

int main()
{
    const benchmark6502::smoke_result result = chris_pikul_mos6502_toolbox::run_smoke_test();
    if (!result.passed) {
        std::fprintf(stderr, "chris-pikul/mos6502 smoke test failed: %s\n", result.message);
        return 1;
    }

    std::puts("chris-pikul/mos6502 smoke test passed");
    return 0;
}
