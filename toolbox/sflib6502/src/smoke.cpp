#include <sflib6502_toolbox.hpp>

extern "C" {
#include <lib6502.h>
}

#include <cstdint>

namespace sflib6502_toolbox {

benchmark6502::smoke_result run_smoke_test()
{
    M6502_Registers registers{};
    M6502_Memory memory{};
    M6502_Callbacks callbacks{};

    for (std::uint32_t i = 0; i < 65536; ++i) {
        memory[i] = 0xEA; // NOP
    }

    memory[0x8000] = 0xA2; // LDX #$42
    memory[0x8001] = 0x42;
    memory[0xFFFC] = 0x00;
    memory[0xFFFD] = 0x80;

    M6502* cpu = M6502_new(&registers, memory, &callbacks);
    if (cpu == nullptr) {
        return {false, "M6502_new failed"};
    }

    M6502_reset(cpu);
    M6502_step(cpu);

    const bool ok = (registers.x == 0x42) && (registers.pc == 0x8002);
    M6502_delete(cpu);

    if (!ok) {
        return {false, "LDX immediate did not set X to 0x42"};
    }

    return {true, ""};
}

} // namespace sflib6502_toolbox
