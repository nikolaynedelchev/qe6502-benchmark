#include <cstdio>
#include "applewin_toolbox/applewin_cpu.hpp"

int main() {
    applewin_toolbox::reset_memory();
    auto* mem = applewin_toolbox::memory();
    mem[0x0200] = 0xa9; // LDA #$42
    mem[0x0201] = 0x42;
    mem[0x0202] = 0xaa; // TAX
    mem[0x0203] = 0xea; // NOP

    applewin_toolbox::reset_cpu(0x0200);
    const auto cycles = applewin_toolbox::execute(6);
    const auto s = applewin_toolbox::state();

    if (cycles != 6 || s.a != 0x42 || s.x != 0x42 || s.pc != 0x0204 || s.jammed) {
        std::fprintf(stderr,
            "applewin smoke failed: cycles=%u A=%02x X=%02x PC=%04x jammed=%d\n",
            cycles, s.a, s.x, s.pc, s.jammed ? 1 : 0);
        return 1;
    }

    std::puts("applewin smoke test passed");
    return 0;
}
