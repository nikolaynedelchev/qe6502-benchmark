#include <sflib65c02_toolbox.hpp>

extern "C" {
#include <lib6502.h>
}

#include <cstddef>
#include <cstdint>

namespace {

int client_read(M6502* cpu, const std::uint16_t address, std::uint8_t)
{
    return cpu->memory[address];
}

int client_write(M6502* cpu, const std::uint16_t address, const std::uint8_t data)
{
    cpu->memory[address] = data;
    return data;
}

void install_client_memory_callbacks(M6502* cpu)
{
    for (std::size_t address = 0; address < 0x10000u; ++address) {
        M6502_setCallback(cpu, read, address, client_read);
        M6502_setCallback(cpu, write, address, client_write);
    }
}

} // namespace

namespace sflib65c02_toolbox {

benchmark6502::smoke_result run_smoke_test()
{
    M6502_Registers registers{};
    M6502_Memory memory{};
    M6502_Callbacks callbacks{};

    for (std::uint32_t i = 0; i < 65536; ++i) {
        memory[i] = 0xEA; // NOP
    }

    memory[0x8000] = 0xA9; // LDA #$77
    memory[0x8001] = 0x77;
    memory[0x8002] = 0x8D; // STA $2000
    memory[0x8003] = 0x00;
    memory[0x8004] = 0x20;
    memory[0x8005] = 0x9C; // STZ $2000 (65C02)
    memory[0x8006] = 0x00;
    memory[0x8007] = 0x20;
    memory[0x8008] = 0x80; // BRA +2 (65C02)
    memory[0x8009] = 0x02;
    memory[0x800A] = 0xA9; // skipped: LDA #$01
    memory[0x800B] = 0x01;
    memory[0x800C] = 0xA9; // LDA #$42
    memory[0x800D] = 0x42;
    memory[0xFFFC] = 0x00;
    memory[0xFFFD] = 0x80;

    M6502* cpu = M6502_new(&registers, memory, &callbacks);
    if (cpu == nullptr) {
        return {false, "M6502_new failed"};
    }
    install_client_memory_callbacks(cpu);

    M6502_reset(cpu);
    for (int i = 0; i < 5; ++i) {
        M6502_step(cpu);
    }

    const bool ok = (memory[0x2000] == 0x00) && (registers.a == 0x42) && (registers.pc == 0x800E);
    M6502_delete(cpu);

    if (!ok) {
        return {false, "65C02 STZ/BRA smoke program failed"};
    }

    return {true, ""};
}

} // namespace sflib65c02_toolbox
