#include <applewin_toolbox.hpp>

#include <array>
#include <cstdint>

namespace applewin_toolbox {

benchmark6502::smoke_result run_smoke_test()
{
    std::array<std::uint8_t, 65536> ram{};
    attach_memory(ram.data(), static_cast<std::uint32_t>(ram.size()));

    constexpr std::uint16_t program_address = 0x0200u;
    ram[program_address + 0u] = 0xa9u; // LDA #$42
    ram[program_address + 1u] = 0x42u;
    ram[program_address + 2u] = 0xaau; // TAX
    ram[program_address + 3u] = 0xeau; // NOP

    reset_cpu(program_address);
    const auto cycles = execute(6);
    const auto s = state();

    if (cycles != 6u) {
        return {false, "unexpected cycle count for LDA/TAX/NOP smoke program"};
    }
    if (s.a != 0x42u) {
        return {false, "LDA immediate did not set A to 0x42"};
    }
    if (s.x != 0x42u) {
        return {false, "TAX did not copy A to X"};
    }
    if (s.pc != program_address + 4u) {
        return {false, "unexpected PC after LDA/TAX/NOP smoke program"};
    }
    if (s.jammed) {
        return {false, "CPU jammed during smoke program"};
    }

    return {true, "passed"};
}

} // namespace applewin_toolbox
