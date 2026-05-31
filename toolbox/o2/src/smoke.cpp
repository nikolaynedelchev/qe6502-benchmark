#include <o2_toolbox.hpp>

#include <O2.hpp>

#include <array>
#include <cstdint>

namespace o2_toolbox {

benchmark6502::smoke_result run_smoke_test()
{
    std::array<std::uint8_t, 0x10000> memory{};
    memory.fill(0xeau);
    memory[0x8000] = 0xa2u; // LDX #$42
    memory[0x8001] = 0x42u;
    memory[0xfffc] = 0x00u;
    memory[0xfffd] = 0x80u;

    O2::CPU cpu(
        [&memory](const std::uint16_t address) { return memory[address]; },
        [&memory](const std::uint16_t address, const std::uint8_t value) { memory[address] = value; });

    cpu.reset();
    cpu.step();

    if (cpu.X != 0x42u) {
        return {false, "LDX immediate did not set X to 0x42"};
    }
    if (cpu.instruction_cycles != 2u) {
        return {false, "LDX immediate did not report 2 cycles"};
    }

    return {true, "passed"};
}

} // namespace o2_toolbox
