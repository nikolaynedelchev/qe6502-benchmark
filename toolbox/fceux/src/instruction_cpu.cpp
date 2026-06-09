#include <fceux_toolbox.hpp>

#include <fceu.h>
#include <x6502.h>

#include <array>
#include <cstdint>
#include <memory>

namespace fceux_toolbox {
namespace {

constexpr std::uint8_t flag_i = 0x04u;
constexpr std::uint8_t flag_u = 0x20u;
constexpr int irq_line_mask = FCEU_IQEXT;

class FceuxInstructionCpu final : public benchmark6502::IInstructionCpu {
public:
    FceuxInstructionCpu()
    {
        install_handlers();
        reset();
    }

    std::string_view core_name() const override { return "fceux"; }
    std::string_view model_name() const override { return "NES/Ricoh 2A03"; }

    void clear_memory(std::uint8_t fill) override { memory_.fill(fill); }

    void load(std::span<const asm6502::mem_value> program) override
    {
        for (const auto& [address, value] : program) memory_[address] = value;
    }

    void reset() override
    {
        install_handlers();
        instruction_count_ = 0;
        clear_scheduled_irq_deassert();
        clear_scheduled_nmi_deassert();
        irq_asserted_ = false;
        nmi_pending_ = false;

        X6502_Init();
        X6502_Power();
        X.IRQlow = 0;
        X.PC = static_cast<std::uint16_t>(memory_[0xfffcu] | (static_cast<std::uint16_t>(memory_[0xfffdu]) << 8u));
        X.P = flag_i;
        X.mooPI = X.P;
        X.S = 0xfd;
        X.jammed = 0;
        X.count = 0;
        X.tcount = 0;
        timestamp = 0;
        soundtimestamp = 0;
    }

    void step_instruction() override
    {
        install_handlers();
        sync_irq_line();
        if (nmi_pending_) {
            TriggerNMI();
        }

        X.count = 1;
        X6502_Run(0);
        X.count = 0;
        X.mooPI = X.P;

        nmi_pending_ = false;
        ++instruction_count_;
        apply_scheduled_interrupt_deasserts();
        sync_irq_line();
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
            X.PC, X.A, X.X, X.Y, X.S, static_cast<std::uint8_t>(X.P | flag_u)};
    }

private:
    void install_handlers()
    {
        current_ = this;
        for (auto& read_handler : ARead) read_handler = read_byte;
        for (auto& write_handler : BWrite) write_handler = write_byte;
        RAM = memory_.data();
    }

    static uint8 read_byte(uint32 address)
    {
        return current_->memory_[static_cast<std::uint16_t>(address)];
    }

    static void write_byte(uint32 address, uint8 value)
    {
        current_->memory_[static_cast<std::uint16_t>(address)] = value;
    }

    void sync_irq_line()
    {
        if (irq_asserted_) {
            X6502_IRQBegin(irq_line_mask);
        } else {
            X6502_IRQEnd(irq_line_mask);
        }
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

    std::array<std::uint8_t, 65536> memory_{};
    std::uint64_t instruction_count_ = 0;
    std::uint64_t irq_deassert_at_ = 0;
    std::uint64_t nmi_deassert_at_ = 0;
    bool has_irq_deassert_at_ = false;
    bool has_nmi_deassert_at_ = false;
    bool irq_asserted_ = false;
    bool nmi_pending_ = false;

    static inline FceuxInstructionCpu* current_ = nullptr;
};

} // namespace

std::unique_ptr<benchmark6502::IInstructionCpu> make_fceux_nes_instruction_cpu()
{
    return std::make_unique<FceuxInstructionCpu>();
}

} // namespace fceux_toolbox
