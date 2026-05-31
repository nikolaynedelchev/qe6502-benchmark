#include <peddle_toolbox.hpp>

#include <Peddle.h>

#include <array>
#include <cstdint>

namespace peddle_toolbox {
namespace {

class SmokeCpu final : public peddle::Peddle {
public:
    std::array<std::uint8_t, 0x10000> memory{};

    SmokeCpu()
    {
        memory.fill(0xeau);
    }

    peddle::u8 read(const peddle::u16 address) override
    {
        return memory[address];
    }

    void write(const peddle::u16 address, const peddle::u8 value) override
    {
        memory[address] = value;
    }

    peddle::u8 readDasm(const peddle::u16 address) const override
    {
        return memory[address];
    }
};

} // namespace

benchmark6502::smoke_result run_smoke_test()
{
    SmokeCpu cpu;
    cpu.setModel(peddle::MOS_6502);

    cpu.memory[0x8000] = 0xa2u; // LDX #$42
    cpu.memory[0x8001] = 0x42u;
    cpu.memory[0xfffc] = 0x00u;
    cpu.memory[0xfffd] = 0x80u;

    cpu.reset();
    cpu.executeInstruction();

    if (cpu.reg.x != 0x42u) {
        return {false, "LDX immediate did not set X to 0x42"};
    }
    if (cpu.reg.pc != 0x8002u) {
        return {false, "LDX immediate did not advance PC to 0x8002"};
    }

    return {true, "passed"};
}

} // namespace peddle_toolbox
