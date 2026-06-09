#include <mame_toolbox.hpp>

#include <cstdint>
#include <memory>

namespace mame_toolbox {
namespace {

class MameInstructionCpu final : public benchmark6502::IInstructionCpu {
public:
    MameInstructionCpu()
        : cpu_(benchmark6502::mame::CpuMode::nmos6502)
    {
        reset();
    }

    std::string_view core_name() const override { return "mame"; }
    std::string_view model_name() const override { return "nmos"; }

    void clear_memory(std::uint8_t fill) override
    {
        cpu_.clear_memory(fill);
    }

    void load(std::span<const asm6502::mem_value> program) override
    {
        for (const auto& [address, value] : program) {
            cpu_.write_memory(address, value);
        }
    }

    void reset() override
    {
        instruction_count_ = 0;
        clear_scheduled_irq_deassert();
        clear_scheduled_nmi_deassert();
        cpu_.clear_irq_line();
        cpu_.set_nmi_line(false);
        cpu_.reset_from_vector();
        cpu_.set_pc_to_opcode_boundary(reset_vector_target());
    }

    void step_instruction() override
    {
        step_from_opcode_boundary();

        ++instruction_count_;
        apply_scheduled_interrupt_deasserts();
    }

    void assert_irq_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            cpu_.clear_irq_line();
            clear_scheduled_irq_deassert();
            return;
        }

        cpu_.set_irq_line(true);
        irq_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_irq_deassert_at_ = true;
    }

    void assert_nmi_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            cpu_.set_nmi_line(false);
            clear_scheduled_nmi_deassert();
            return;
        }

        cpu_.set_nmi_line(true);
        nmi_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_nmi_deassert_at_ = true;
    }

    std::uint8_t read(std::uint16_t address) const override
    {
        return cpu_.read_memory(address);
    }

    void write(std::uint16_t address, std::uint8_t value) override
    {
        cpu_.write_memory(address, value);
    }

    benchmark6502::instruction_cpu_state state() const override
    {
        return benchmark6502::instruction_cpu_state{
            cpu_.pc(),
            cpu_.a(),
            cpu_.x(),
            cpu_.y(),
            cpu_.sp(),
            cpu_.p(),
        };
    }

private:
    std::uint16_t reset_vector_target() const
    {
        return static_cast<std::uint16_t>(cpu_.read_memory(0xfffcu))
            | static_cast<std::uint16_t>(static_cast<std::uint16_t>(cpu_.read_memory(0xfffdu)) << 8u);
    }

    void step_from_opcode_boundary()
    {
        cpu_.execute_one_instruction_from_opcode_boundary();
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
            cpu_.clear_irq_line();
            clear_scheduled_irq_deassert();
        }

        if (has_nmi_deassert_at_ && instruction_count_ >= nmi_deassert_at_) {
            cpu_.set_nmi_line(false);
            clear_scheduled_nmi_deassert();
        }
    }

    benchmark6502::mame::Mame6502Cpu cpu_;
    std::uint64_t instruction_count_ = 0;
    std::uint64_t irq_deassert_at_ = 0;
    std::uint64_t nmi_deassert_at_ = 0;
    bool has_irq_deassert_at_ = false;
    bool has_nmi_deassert_at_ = false;
};

} // namespace

std::unique_ptr<benchmark6502::IInstructionCpu> make_mame_nmos_instruction_cpu()
{
    return std::make_unique<MameInstructionCpu>();
}

} // namespace mame_toolbox
