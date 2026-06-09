#include <sflib65c02_toolbox.hpp>

extern "C" {
#include <lib6502.h>
}

#include <cstdint>
#include <cstring>
#include <memory>

namespace sflib65c02_toolbox {
namespace {

constexpr std::uint8_t flag_i = 0x04u;
constexpr std::uint8_t flag_b = 0x10u;
constexpr std::uint8_t flag_u = 0x20u;

class Sflib65c02InstructionCpu final : public benchmark6502::IInstructionCpu {
public:
    Sflib65c02InstructionCpu()
    {
        cpu_ = M6502_new(&registers_, memory_, &callbacks_);
        reset();
    }

    ~Sflib65c02InstructionCpu() override
    {
        if (cpu_ != nullptr) M6502_delete(cpu_);
    }

    Sflib65c02InstructionCpu(const Sflib65c02InstructionCpu&) = delete;
    Sflib65c02InstructionCpu& operator=(const Sflib65c02InstructionCpu&) = delete;

    std::string_view core_name() const override { return "sflib65c02"; }
    std::string_view model_name() const override { return "65C02"; }

    void clear_memory(std::uint8_t fill) override { std::memset(memory_, fill, sizeof(memory_)); }

    void load(std::span<const asm6502::mem_value> program) override
    {
        for (const auto& [address, value] : program) memory_[address] = value;
    }

    void reset() override
    {
        instruction_count_ = 0;
        clear_scheduled_irq_deassert();
        clear_scheduled_nmi_deassert();
        irq_asserted_ = false;
        nmi_pending_ = false;
        registers_ = {};
        registers_.s = 0xfd;
        registers_.p = flag_u;
        if (cpu_ != nullptr) {
            M6502_reset(cpu_);
            registers_.p |= flag_i | flag_u;
        }
    }

    void step_instruction() override
    {
        if (nmi_pending_) {
            enter_interrupt(0xfffau, false);
            nmi_pending_ = false;
        } else if (irq_asserted_ && (registers_.p & flag_i) == 0u) {
            enter_interrupt(0xfffeu, false);
        } else {
            M6502_step(cpu_);
        }
        ++instruction_count_;
        apply_scheduled_interrupt_deasserts();
    }

    void assert_irq_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            irq_asserted_ = false;
            clear_scheduled_irq_deassert();
            return;
        }
        irq_asserted_ = true;
        irq_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_irq_deassert_at_ = true;
    }

    void assert_nmi_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            nmi_pending_ = false;
            clear_scheduled_nmi_deassert();
            return;
        }
        nmi_pending_ = true;
        nmi_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_nmi_deassert_at_ = true;
    }

    std::uint8_t read(std::uint16_t address) const override { return memory_[address]; }
    void write(std::uint16_t address, std::uint8_t value) override { memory_[address] = value; }

    benchmark6502::instruction_cpu_state state() const override
    {
        return benchmark6502::instruction_cpu_state{
            registers_.pc, registers_.a, registers_.x, registers_.y, registers_.s,
            static_cast<std::uint8_t>(registers_.p | flag_u)};
    }

private:
    void enter_interrupt(std::uint16_t vector, bool break_bit)
    {
        memory_[0x0100u + registers_.s--] = static_cast<std::uint8_t>(registers_.pc >> 8u);
        memory_[0x0100u + registers_.s--] = static_cast<std::uint8_t>(registers_.pc & 0xffu);
        const std::uint8_t pushed_p = static_cast<std::uint8_t>(((registers_.p | flag_u) & ~flag_b) | (break_bit ? flag_b : 0u));
        memory_[0x0100u + registers_.s--] = pushed_p;
        registers_.p = static_cast<std::uint8_t>((registers_.p & ~flag_b) | flag_i | flag_u);
        registers_.pc = static_cast<std::uint16_t>(memory_[vector] | (static_cast<std::uint16_t>(memory_[vector + 1u]) << 8u));
    }

    void clear_scheduled_irq_deassert() { has_irq_deassert_at_ = false; irq_deassert_at_ = 0; }
    void clear_scheduled_nmi_deassert() { has_nmi_deassert_at_ = false; nmi_deassert_at_ = 0; }

    void apply_scheduled_interrupt_deasserts()
    {
        if (has_irq_deassert_at_ && instruction_count_ >= irq_deassert_at_) {
            irq_asserted_ = false;
            clear_scheduled_irq_deassert();
        }
        if (has_nmi_deassert_at_ && instruction_count_ >= nmi_deassert_at_) {
            nmi_pending_ = false;
            clear_scheduled_nmi_deassert();
        }
    }

    M6502_Registers registers_{};
    M6502_Memory memory_{};
    M6502_Callbacks callbacks_{};
    M6502* cpu_ = nullptr;
    std::uint64_t instruction_count_ = 0;
    std::uint64_t irq_deassert_at_ = 0;
    std::uint64_t nmi_deassert_at_ = 0;
    bool has_irq_deassert_at_ = false;
    bool has_nmi_deassert_at_ = false;
    bool irq_asserted_ = false;
    bool nmi_pending_ = false;
};

} // namespace

std::unique_ptr<benchmark6502::IInstructionCpu> make_sflib65c02_instruction_cpu()
{
    return std::make_unique<Sflib65c02InstructionCpu>();
}

} // namespace sflib65c02_toolbox
