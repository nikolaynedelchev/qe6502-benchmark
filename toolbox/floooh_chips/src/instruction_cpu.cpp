#include <floooh_chips_toolbox.hpp>

#include <cpu6502_bridge/cpu.hpp>

#include <cstdint>
#include <memory>

namespace floooh_chips_toolbox {
namespace {

class FlooohChipsInstructionCpu final : public benchmark6502::IInstructionCpu {
public:
    FlooohChipsInstructionCpu()
        : cpu_(make_floooh_chips_nmos_cpu())
    {
        reset();
    }

    std::string_view core_name() const override { return "floooh/chips"; }
    std::string_view model_name() const override { return "nmos"; }

    void clear_memory(std::uint8_t fill) override
    {
        cpu_->clear_memory(fill);
    }

    void load(std::span<const asm6502::mem_value> program) override
    {
        auto* const memory = cpu_->memory();
        for (const auto& [address, value] : program) {
            memory[address] = value;
        }
    }

    void reset() override
    {
        instruction_count_ = 0;
        clear_scheduled_irq_deassert();
        clear_scheduled_nmi_deassert();
        cpu_->restart_to_start_fetch();
        cpu_->irq(false);
        cpu_->nmi(false);
    }

    void step_instruction() override
    {
        if (!cpu_->is_opcode_fetch()) {
            step_until_opcode_fetch();
        }

        cpu_->step();
        step_until_opcode_fetch();

        ++instruction_count_;
        apply_scheduled_interrupt_deasserts();
    }

    void assert_irq_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            cpu_->irq(false);
            clear_scheduled_irq_deassert();
            return;
        }

        cpu_->irq(true);
        irq_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_irq_deassert_at_ = true;
    }

    void assert_nmi_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            cpu_->nmi(false);
            clear_scheduled_nmi_deassert();
            return;
        }

        cpu_->nmi(true);
        nmi_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_nmi_deassert_at_ = true;
    }

    std::uint8_t read(std::uint16_t address) const override
    {
        return cpu_->memory()[address];
    }

    void write(std::uint16_t address, std::uint8_t value) override
    {
        cpu_->memory()[address] = value;
    }

    benchmark6502::instruction_cpu_state state() const override
    {
        return benchmark6502::instruction_cpu_state{
            visible_pc(),
            cpu_->a(),
            cpu_->x(),
            cpu_->y(),
            cpu_->s(),
            cpu_->p(),
        };
    }

private:
    std::uint16_t visible_pc() const
    {
        if (cpu_->is_opcode_fetch()) {
            return cpu_->bus_address();
        }
        return cpu_->pc();
    }

    void step_until_opcode_fetch()
    {
        constexpr unsigned max_cycles = 64u;
        for (unsigned cycle = 0; cycle < max_cycles; ++cycle) {
            if (cpu_->is_opcode_fetch()) {
                return;
            }
            cpu_->step();
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
            cpu_->irq(false);
            clear_scheduled_irq_deassert();
        }

        if (has_nmi_deassert_at_ && instruction_count_ >= nmi_deassert_at_) {
            cpu_->nmi(false);
            clear_scheduled_nmi_deassert();
        }
    }

    std::unique_ptr<cpu6502_bridge::ICpu> cpu_;
    std::uint64_t instruction_count_ = 0;
    std::uint64_t irq_deassert_at_ = 0;
    std::uint64_t nmi_deassert_at_ = 0;
    bool has_irq_deassert_at_ = false;
    bool has_nmi_deassert_at_ = false;
};

} // namespace

std::unique_ptr<benchmark6502::IInstructionCpu> make_floooh_chips_nmos_instruction_cpu()
{
    return std::make_unique<FlooohChipsInstructionCpu>();
}

} // namespace floooh_chips_toolbox
