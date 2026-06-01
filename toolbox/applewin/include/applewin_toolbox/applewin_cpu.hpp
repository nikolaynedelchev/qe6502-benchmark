#pragma once

#include <cstdint>

namespace applewin_toolbox {

struct State {
    std::uint8_t a;
    std::uint8_t x;
    std::uint8_t y;
    std::uint8_t p;
    std::uint16_t pc;
    std::uint16_t sp;
    bool jammed;
};

void reset_memory();
std::uint8_t* memory();
void reset_cpu(std::uint16_t pc, bool cmos = false);
std::uint32_t execute(std::uint32_t cycles);
State state();

} // namespace applewin_toolbox
