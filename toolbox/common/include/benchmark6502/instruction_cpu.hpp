#pragma once

#include <asm6502/asm6502.h>

#include <cstdint>
#include <span>
#include <string_view>

namespace benchmark6502 {

struct instruction_cpu_state {
    std::uint16_t pc = 0;
    std::uint8_t a = 0;
    std::uint8_t x = 0;
    std::uint8_t y = 0;
    std::uint8_t s = 0;
    std::uint8_t p = 0;
};

class IInstructionCpu {
public:
    virtual ~IInstructionCpu() = default;

    virtual std::string_view core_name() const = 0;
    virtual std::string_view model_name() const = 0;

    virtual void clear_memory(std::uint8_t fill) = 0;
    virtual void load(std::span<const asm6502::mem_value> program) = 0;
    virtual void reset() = 0;

    virtual void step_instruction() = 0;

    virtual void irq(bool asserted) = 0;
    virtual void nmi(bool asserted) = 0;

    virtual std::uint8_t read(std::uint16_t address) const = 0;
    virtual void write(std::uint16_t address, std::uint8_t value) = 0;

    virtual instruction_cpu_state state() const = 0;
};

} // namespace benchmark6502
