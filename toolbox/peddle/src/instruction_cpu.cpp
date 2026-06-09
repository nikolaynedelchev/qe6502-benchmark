#include <peddle_toolbox.hpp>

#include <Peddle.h>

#include <array>
#include <cstdint>
#include <memory>

namespace peddle_toolbox {
namespace {

class PeddleInstructionCpu final : public peddle::Peddle, public benchmark6502::IInstructionCpu {
public:
    PeddleInstructionCpu()
    {
        setModel(peddle::MOS_6502);
        memory_.fill(0u);
        reset();
    }

    std::string_view core_name() const override { return "Peddle"; }
    std::string_view model_name() const override { return "nmos"; }

    void clear_memory(std::uint8_t fill) override { memory_.fill(fill); }

    void load(std::span<const asm6502::mem_value> program) override
    {
        for (const auto& [address, value] : program) {
            memory_[address] = value;
        }
    }

    void reset() override
    {
        instruction_count_ = 0;
        clear_scheduled_irq_deassert();
        clear_scheduled_nmi_deassert();
        irq_asserted_ = false;
        nmi_asserted_ = false;
        peddle::Peddle::reset();
        releaseIrqLine(interrupt_source_);
        releaseNmiLine(interrupt_source_);
    }

    void step_instruction() override
    {
        executeInstruction();
        ++instruction_count_;
        apply_scheduled_interrupt_deasserts();
    }

    void assert_irq_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            set_irq(false);
            clear_scheduled_irq_deassert();
            return;
        }

        set_irq(true);
        irq_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_irq_deassert_at_ = true;
    }

    void assert_nmi_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            set_nmi(false);
            clear_scheduled_nmi_deassert();
            return;
        }

        set_nmi(true);
        nmi_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_nmi_deassert_at_ = true;
    }

    std::uint8_t read(std::uint16_t address) const override { return memory_[address]; }
    void write(std::uint16_t address, std::uint8_t value) override { memory_[address] = value; }

    benchmark6502::instruction_cpu_state state() const override
    {
        return benchmark6502::instruction_cpu_state{
            reg.pc,
            reg.a,
            reg.x,
            reg.y,
            static_cast<std::uint8_t>(reg.sp),
            getP(),
        };
    }

    peddle::u8 read(const peddle::u16 address) override
    {
        return memory_[address];
    }


    peddle::u8 readDasm(const peddle::u16 address) const override
    {
        return memory_[address];
    }

private:
    static constexpr peddle::IntSource interrupt_source_ = 1;

    void set_irq(bool asserted)
    {
        if (asserted == irq_asserted_) {
            return;
        }
        irq_asserted_ = asserted;
        if (asserted) {
            pullDownIrqLine(interrupt_source_);
        } else {
            releaseIrqLine(interrupt_source_);
        }
    }

    void set_nmi(bool asserted)
    {
        if (asserted == nmi_asserted_) {
            return;
        }
        nmi_asserted_ = asserted;
        if (asserted) {
            pullDownNmiLine(interrupt_source_);
        } else {
            releaseNmiLine(interrupt_source_);
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
            set_irq(false);
            clear_scheduled_irq_deassert();
        }

        if (has_nmi_deassert_at_ && instruction_count_ >= nmi_deassert_at_) {
            set_nmi(false);
            clear_scheduled_nmi_deassert();
        }
    }

    std::array<std::uint8_t, 65536> memory_{};
    std::uint64_t instruction_count_ = 0;
    std::uint64_t irq_deassert_at_ = 0;
    std::uint64_t nmi_deassert_at_ = 0;
    bool has_irq_deassert_at_ = false;
    bool has_nmi_deassert_at_ = false;
    bool irq_asserted_ = false;
    bool nmi_asserted_ = false;
};

} // namespace

std::unique_ptr<benchmark6502::IInstructionCpu> make_peddle_nmos_instruction_cpu()
{
    return std::make_unique<PeddleInstructionCpu>();
}

} // namespace peddle_toolbox
