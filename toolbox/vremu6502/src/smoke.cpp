#include <vremu6502_toolbox.hpp>

#include <vrEmu6502.h>

#include <array>
#include <cstdint>

namespace {

std::array<std::uint8_t, 0x10000>* g_memory = nullptr;

std::uint8_t memory_read(const std::uint16_t address, const bool /*is_debug*/)
{
    return (*g_memory)[address];
}

void memory_write(const std::uint16_t address, const std::uint8_t value)
{
    (*g_memory)[address] = value;
}

} // namespace

namespace vremu6502_toolbox {

benchmark6502::smoke_result run_smoke_test()
{
    std::array<std::uint8_t, 0x10000> memory{};
    memory.fill(0xeau);
    memory[0x8000] = 0xa2u; // LDX #$42
    memory[0x8001] = 0x42u;
    memory[0xfffc] = 0x00u;
    memory[0xfffd] = 0x80u;

    g_memory = &memory;
    VrEmu6502* cpu = vrEmu6502New(CPU_6502U, memory_read, memory_write);
    if (cpu == nullptr) {
        g_memory = nullptr;
        return {false, "could not create vrEmu6502 CPU"};
    }

    vrEmu6502Reset(cpu);
    const std::uint8_t cycles = vrEmu6502InstCycle(cpu);
    const std::uint8_t x = vrEmu6502GetX(cpu);
    vrEmu6502Destroy(cpu);
    g_memory = nullptr;

    if (x != 0x42u) {
        return {false, "LDX immediate did not set X to 0x42"};
    }
    if (cycles != 2u) {
        return {false, "LDX immediate did not report 2 cycles"};
    }

    return {true, "passed"};
}

} // namespace vremu6502_toolbox
