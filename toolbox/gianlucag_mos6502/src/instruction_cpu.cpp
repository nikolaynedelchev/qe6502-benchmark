#include <gianlucag_mos6502_toolbox.hpp>

#include <mos6502.h>

#include <array>
#include <cstdint>
#include <memory>

namespace gianlucag_mos6502_toolbox {
namespace {

constexpr std::uint8_t flag_i = 0x04u;
constexpr std::uint8_t flag_u = 0x20u;

class GianlucagMos6502InstructionCpu final : public benchmark6502::IInstructionCpu {
public:
    GianlucagMos6502InstructionCpu()
        : cpu_(read_byte, write_byte)
    {
        current_ = this;
        reset();
    }

    std::string_view core_name() const override { return "gianlucag_mos6502"; }
    std::string_view model_name() const override { return "NMOS"; }

    void clear_memory(std::uint8_t fill) override { memory_.fill(fill); }

    void load(std::span<const asm6502::mem_value> program) override
    {
        for (const auto& [address, value] : program) memory_[address] = value;
    }

    void reset() override
    {
        current_ = this;
        instruction_count_ = 0;
        clear_scheduled_irq_deassert();
        clear_scheduled_nmi_deassert();
        irq_asserted_ = false;
        nmi_asserted_ = false;
        cpu_.IRQ(true);      // inactive, active-low IRQ input
        cpu_.NMI(true);      // inactive high; a high-to-low transition asserts NMI
        cpu_.SetResetS(0xfdu);
        cpu_.SetResetP(flag_i | flag_u);
        cpu_.SetResetA(0u);
        cpu_.SetResetX(0u);
        cpu_.SetResetY(0u);
        cpu_.Reset();
        cpu_.SetP(static_cast<std::uint8_t>(cpu_.GetP() | flag_i | flag_u));
    }

    void step_instruction() override
    {
        current_ = this;
        sync_irq_line();
        sync_nmi_line();

        std::uint64_t cycles = 0;
        cpu_.Run(1, cycles, mos6502::INST_COUNT);

        ++instruction_count_;
        apply_scheduled_interrupt_deasserts();
        sync_irq_line();
        sync_nmi_line();
    }

    void assert_irq_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            irq_asserted_ = false;
            clear_scheduled_irq_deassert();
            sync_irq_line();
            return;
        }
        irq_asserted_ = true;
        irq_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_irq_deassert_at_ = true;
        sync_irq_line();
    }

    void assert_nmi_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            nmi_asserted_ = false;
            clear_scheduled_nmi_deassert();
            sync_nmi_line();
            return;
        }
        nmi_asserted_ = true;
        nmi_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_nmi_deassert_at_ = true;
        sync_nmi_line();
    }

    std::uint8_t read(std::uint16_t address) const override { return memory_[address]; }
    void write(std::uint16_t address, std::uint8_t value) override { memory_[address] = value; }

    benchmark6502::instruction_cpu_state state() const override
    {
        return benchmark6502::instruction_cpu_state{
            cpu_.GetPC(), cpu_.GetA(), cpu_.GetX(), cpu_.GetY(), cpu_.GetS(),
            static_cast<std::uint8_t>(cpu_.GetP() | flag_u)};
    }

private:
    static std::uint8_t read_byte(std::uint16_t address)
    {
        return current_->memory_[address];
    }

    static void write_byte(std::uint16_t address, std::uint8_t value)
    {
        current_->memory_[address] = value;
    }

    void sync_irq_line()
    {
        cpu_.IRQ(!irq_asserted_); // core IRQ input is active low
    }

    void sync_nmi_line()
    {
        cpu_.NMI(!nmi_asserted_); // core NMI input is edge triggered, high-to-low
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
            nmi_asserted_ = false;
            clear_scheduled_nmi_deassert();
        }
    }

    std::array<std::uint8_t, 65536> memory_{};
    mutable mos6502 cpu_;
    std::uint64_t instruction_count_ = 0;
    std::uint64_t irq_deassert_at_ = 0;
    std::uint64_t nmi_deassert_at_ = 0;
    bool has_irq_deassert_at_ = false;
    bool has_nmi_deassert_at_ = false;
    bool irq_asserted_ = false;
    bool nmi_asserted_ = false;

    static inline GianlucagMos6502InstructionCpu* current_ = nullptr;
};

} // namespace

std::unique_ptr<benchmark6502::IInstructionCpu> make_gianlucag_mos6502_nmos_instruction_cpu()
{
    return std::make_unique<GianlucagMos6502InstructionCpu>();
}

} // namespace gianlucag_mos6502_toolbox
