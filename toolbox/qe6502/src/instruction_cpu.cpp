#include <qe6502_toolbox.hpp>

#include <qe6502/qe6502.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>

namespace qe6502_toolbox {
namespace {

class Qe6502InstructionCpu final : public benchmark6502::IInstructionCpu {
public:
    Qe6502InstructionCpu()
    {
        cpu_.model = qe6502_model_nmos;
        reset();
    }

    std::string_view core_name() const override { return "qe6502"; }
    std::string_view model_name() const override { return "nmos"; }

    void clear_memory(std::uint8_t fill) override
    {
        memory_.fill(fill);
    }

    void load(std::span<const asm6502::mem_value> program) override
    {
        for (const auto& [address, value] : program) {
            memory_[address] = value;
        }
    }

    void reset() override
    {
        const std::uint16_t start_address = static_cast<std::uint16_t>(memory_[0xfffcu])
            | static_cast<std::uint16_t>(static_cast<std::uint16_t>(memory_[0xfffdu]) << 8u);

        instruction_count_ = 0;
        clear_scheduled_irq_deassert();
        clear_scheduled_nmi_deassert();
        qe6502_irq_assert(&cpu_, 0u);
        qe6502_nmi_assert(&cpu_, 0u);

        tick_ = qe6502_restart(&cpu_);

        constexpr unsigned max_reset_cycles = 64u;
        for (unsigned cycle = 0; cycle < max_reset_cycles; ++cycle) {
            if ((tick_.status & qe6502_status_opcode_fetch) != 0u
                && !is_write()
                && tick_.address == start_address) {
                return;
            }
            step_cycle();
        }
    }

    void step_instruction() override
    {
        if ((tick_.status & qe6502_status_opcode_fetch) == 0u) {
            step_until_opcode_fetch();
        }

        step_cycle();
        step_until_opcode_fetch();

        ++instruction_count_;
        apply_scheduled_interrupt_deasserts();
    }

    void assert_irq_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            qe6502_irq_assert(&cpu_, 0u);
            clear_scheduled_irq_deassert();
            return;
        }

        qe6502_irq_assert(&cpu_, 1u);
        irq_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_irq_deassert_at_ = true;
    }

    void assert_nmi_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            qe6502_nmi_assert(&cpu_, 0u);
            clear_scheduled_nmi_deassert();
            return;
        }

        qe6502_nmi_assert(&cpu_, 1u);
        nmi_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_nmi_deassert_at_ = true;
    }

    std::uint8_t read(std::uint16_t address) const override
    {
        return memory_[address];
    }

    void write(std::uint16_t address, std::uint8_t value) override
    {
        memory_[address] = value;
    }

    benchmark6502::instruction_cpu_state state() const override
    {
        return benchmark6502::instruction_cpu_state{
            visible_pc(),
            cpu_.A,
            cpu_.X,
            cpu_.Y,
            cpu_.S,
            cpu_.P,
        };
    }

private:
    std::uint16_t visible_pc() const
    {
        if ((tick_.status & qe6502_status_opcode_fetch) != 0u) {
            return tick_.address;
        }
        return cpu_.PC;
    }

    bool is_write() const
    {
        return (tick_.status & qe6502_status_writing) != 0u;
    }

    void step_cycle()
    {
        const std::uint16_t address = tick_.address;
        const std::uint8_t input = memory_[address];

        if (is_write()) {
            memory_[address] = tick_.bus;
        }

        tick_ = qe6502_tick(&cpu_, input);
    }

    void step_until_opcode_fetch()
    {
        constexpr unsigned max_cycles = 32u;
        for (unsigned cycle = 0; cycle < max_cycles; ++cycle) {
            if ((tick_.status & qe6502_status_opcode_fetch) != 0u) {
                return;
            }
            if ((tick_.status & qe6502_status_cpu_jammed) != 0u) {
                return;
            }
            step_cycle();
        }
    }

    void clear_scheduled_irq_deassert()
    {
        has_irq_deassert_at_ = false;
        irq_deassert_at_ = 0;
    }

    void clear_scheduled_nmi_deassert()
    {
        has_nmi_deassert_at_ = false;
        nmi_deassert_at_ = 0;
    }

    void apply_scheduled_interrupt_deasserts()
    {
        if (has_irq_deassert_at_ && instruction_count_ >= irq_deassert_at_) {
            qe6502_irq_assert(&cpu_, 0u);
            clear_scheduled_irq_deassert();
        }

        if (has_nmi_deassert_at_ && instruction_count_ >= nmi_deassert_at_) {
            qe6502_nmi_assert(&cpu_, 0u);
            clear_scheduled_nmi_deassert();
        }
    }

    qe6502_t cpu_{};
    qe6502_tick_t tick_{};
    std::array<std::uint8_t, 65536> memory_{};
    std::uint64_t instruction_count_ = 0;
    std::uint64_t irq_deassert_at_ = 0;
    std::uint64_t nmi_deassert_at_ = 0;
    bool has_irq_deassert_at_ = false;
    bool has_nmi_deassert_at_ = false;
};

} // namespace

std::unique_ptr<benchmark6502::IInstructionCpu> make_qe6502_nmos_instruction_cpu()
{
    return std::make_unique<Qe6502InstructionCpu>();
}

} // namespace qe6502_toolbox
