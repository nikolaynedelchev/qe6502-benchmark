#include <floooh_chips_toolbox.hpp>

#include "chips/m6502.h"

#include <cstdint>

namespace {

std::uint8_t read_memory(const std::uint16_t address)
{
    switch (address) {
        case 0x8000u: return 0xa2u; // LDX #imm
        case 0x8001u: return 0x42u;
        default: return 0xeau;      // NOP
    }
}

} // namespace

namespace floooh_chips_toolbox {

benchmark6502::smoke_result run_smoke_test()
{
    m6502_t cpu{};
    m6502_desc_t desc{};
    static_cast<void>(m6502_init(&cpu, &desc));

    m6502_set_pc(&cpu, 0x8000u);

    uint64_t pins = M6502_MAKE_PINS(M6502_RW | M6502_SYNC, 0x8000u, 0u);

    for (int tick = 0; tick < 8; ++tick) {
        if (pins & M6502_RW) {
            M6502_SET_DATA(pins, read_memory(M6502_GET_ADDR(pins)));
        }
        pins = m6502_tick(&cpu, pins);

        if (m6502_x(&cpu) == 0x42u) {
            return {true, "passed"};
        }
    }

    return {false, "LDX immediate did not complete"};
}

} // namespace floooh_chips_toolbox
