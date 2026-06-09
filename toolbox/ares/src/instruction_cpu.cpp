#include <ares_toolbox.hpp>

#include <ares/ares.hpp>
#include <component/processor/mos6502/mos6502.hpp>

#include <array>
#include <cstdint>
#include <memory>

namespace ares_toolbox {
namespace {

constexpr std::uint8_t flag_i = 0x04u;
constexpr std::uint8_t flag_u = 0x20u;

class AresInstructionCpu final : public benchmark6502::IInstructionCpu, private ares::MOS6502 {
public:
    AresInstructionCpu() { reset(); }

    std::string_view core_name() const override { return "ares"; }
    std::string_view model_name() const override { return "MOS6502 NMOS/BCD"; }

    void clear_memory(std::uint8_t fill) override { memory_.fill(fill); }

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
        power();
        BCD = 1;
        MOS6502::reset();
    }

    void step_instruction() override
    {
        if (nmi_pending_) {
            MOS6502::interrupt();
        } else if (irq_asserted_ && static_cast<std::uint8_t>(P & flag_i) == 0u) {
            MOS6502::interrupt();
        } else {
            MOS6502::instruction();
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
            static_cast<std::uint16_t>(PC), static_cast<std::uint8_t>(A), static_cast<std::uint8_t>(X),
            static_cast<std::uint8_t>(Y), static_cast<std::uint8_t>(S),
            static_cast<std::uint8_t>(static_cast<std::uint8_t>(P) | flag_u)};
    }

private:
    auto read(n16 address) -> n8 override { return memory_[static_cast<std::uint16_t>(address)]; }
    auto write(n16 address, n8 data) -> void override { memory_[static_cast<std::uint16_t>(address)] = static_cast<std::uint8_t>(data); }
    auto nmi(n16& vector) -> void override
    {
        if (nmi_pending_) {
            vector = 0xfffa;
            nmi_pending_ = false;
        }
    }
    auto cancelNmi() -> void override { nmi_pending_ = false; }
    auto irqPending() -> bool override { return irq_asserted_ && static_cast<std::uint8_t>(P & flag_i) == 0u; }

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

    std::array<std::uint8_t, 65536> memory_{};
    std::uint64_t instruction_count_ = 0;
    std::uint64_t irq_deassert_at_ = 0;
    std::uint64_t nmi_deassert_at_ = 0;
    bool has_irq_deassert_at_ = false;
    bool has_nmi_deassert_at_ = false;
    bool irq_asserted_ = false;
    bool nmi_pending_ = false;
};

} // namespace

std::unique_ptr<benchmark6502::IInstructionCpu> make_ares_nmos_instruction_cpu()
{
    return std::make_unique<AresInstructionCpu>();
}

} // namespace ares_toolbox
