#include <applewin_toolbox.hpp>

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>

namespace applewin_toolbox {
namespace {

class AppleWinInstructionCpu final : public benchmark6502::IInstructionCpu {
public:
    AppleWinInstructionCpu()
    {
        attach_memory(memory_.data(), static_cast<std::uint32_t>(memory_.size()));
        reset();
    }

    std::string_view core_name() const override { return "applewin"; }
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
        attach_memory(memory_.data(), static_cast<std::uint32_t>(memory_.size()));
        instruction_count_ = 0;
        clear_scheduled_irq_deassert();
        clear_scheduled_nmi_deassert();
        clear_interrupt_lines();
        reset_from_vector(CpuMode::nmos6502);
    }

    void step_instruction() override
    {
        execute_one_instruction();

        ++instruction_count_;
        apply_scheduled_interrupt_deasserts();
    }

    void assert_irq_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            clear_irq_line();
            clear_scheduled_irq_deassert();
            return;
        }

        set_irq_line(true);
        irq_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_irq_deassert_at_ = true;
    }

    void assert_nmi_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            clear_nmi_pending();
            clear_scheduled_nmi_deassert();
            return;
        }

        pulse_nmi();
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
            pc(),
            a(),
            x(),
            y(),
            static_cast<std::uint8_t>(sp()),
            p(),
        };
    }

private:
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
            clear_irq_line();
            clear_scheduled_irq_deassert();
        }

        if (has_nmi_deassert_at_ && instruction_count_ >= nmi_deassert_at_) {
            clear_nmi_pending();
            clear_scheduled_nmi_deassert();
        }
    }

    std::array<std::uint8_t, 65536> memory_{};
    std::uint64_t instruction_count_ = 0;
    std::uint64_t irq_deassert_at_ = 0;
    std::uint64_t nmi_deassert_at_ = 0;
    bool has_irq_deassert_at_ = false;
    bool has_nmi_deassert_at_ = false;
};

} // namespace

std::unique_ptr<benchmark6502::IInstructionCpu> make_applewin_nmos_instruction_cpu()
{
    return std::make_unique<AppleWinInstructionCpu>();
}

} // namespace applewin_toolbox
