#include <chris_pikul_mos6502_toolbox.hpp>

#include <bus.h>
#include <cpu.h>
#include <memory.h>

#include <cstdint>
#include <memory>

namespace chris_pikul_mos6502_toolbox {
namespace {

constexpr std::uint8_t flag_i = 0x04u;
constexpr std::uint8_t flag_u = 0x20u;

class AdapterCpu final : public mos6502::CPU {
public:
    using mos6502::CPU::CPU;

    void enter_interrupt(std::uint16_t vector, bool break_bit)
    {
        PushToStack(GET_HIGH_BYTE(m_PC));
        PushToStack(GET_LOW_BYTE(m_PC));
        const std::uint8_t pushed_p = static_cast<std::uint8_t>((m_ProcStatus.value | flag_u) & ~0x10u);
        PushToStack(break_bit ? static_cast<std::uint8_t>(pushed_p | 0x10u) : pushed_p);
        m_ProcStatus.value = static_cast<std::uint8_t>((m_ProcStatus.value & ~0x10u) | flag_i | flag_u);
        m_PC = ReadWord(vector);
        m_CyclesRem += 7;
    }
};

class ChrisPikulInstructionCpu final : public benchmark6502::IInstructionCpu {
public:
    ChrisPikulInstructionCpu()
        : memory_(std::static_pointer_cast<mos6502::Memory>(mos6502::Memory::Make()))
        , bus_(mos6502::Bus::Make(memory_))
        , cpu_(bus_)
    {
        reset();
    }

    std::string_view core_name() const override { return "chris_pikul_mos6502"; }
    std::string_view model_name() const override { return "NMOS"; }

    void clear_memory(std::uint8_t fill) override { memory_->Clear(fill); }

    void load(std::span<const asm6502::mem_value> program) override
    {
        for (const auto& [address, value] : program) {
            memory_->WriteByte(address, value);
        }
    }

    void reset() override
    {
        instruction_count_ = 0;
        clear_scheduled_irq_deassert();
        clear_scheduled_nmi_deassert();
        irq_asserted_ = false;
        nmi_pending_ = false;
        cpu_.Reset();
        cpu_.SetStatusFlag(mos6502::CPU::StatusFlag::INTERRUPT, true);
        cpu_.SetStatusFlag(mos6502::CPU::StatusFlag::UNUSED, true);
    }

    void step_instruction() override
    {
        if (nmi_pending_) {
            cpu_.enter_interrupt(0xfffau, false);
            nmi_pending_ = false;
            drain_current_operation();
        } else if (irq_asserted_ && (cpu_.GetStatus().value & flag_i) == 0u) {
            cpu_.enter_interrupt(0xfffeu, false);
            drain_current_operation();
        } else {
            run_one_instruction();
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

    std::uint8_t read(std::uint16_t address) const override { return memory_->ReadByte(address); }
    void write(std::uint16_t address, std::uint8_t value) override { memory_->WriteByte(address, value); }

    benchmark6502::instruction_cpu_state state() const override
    {
        return benchmark6502::instruction_cpu_state{
            cpu_.GetProgramCounter(), cpu_.GetAccumulator(), cpu_.GetX(), cpu_.GetY(),
            cpu_.GetStackPointer(), static_cast<std::uint8_t>(cpu_.GetStatus().value | flag_u)};
    }

private:
    void run_one_instruction()
    {
        bool more = false;
        do {
            more = cpu_.Tick();
        } while (more);
    }

    void drain_current_operation()
    {
        while (cpu_.Tick()) {
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

    std::shared_ptr<mos6502::Memory> memory_;
    mos6502::ioptr bus_;
    AdapterCpu cpu_;
    std::uint64_t instruction_count_ = 0;
    std::uint64_t irq_deassert_at_ = 0;
    std::uint64_t nmi_deassert_at_ = 0;
    bool has_irq_deassert_at_ = false;
    bool has_nmi_deassert_at_ = false;
    bool irq_asserted_ = false;
    bool nmi_pending_ = false;
};

} // namespace

std::unique_ptr<benchmark6502::IInstructionCpu> make_chris_pikul_mos6502_nmos_instruction_cpu()
{
    return std::make_unique<ChrisPikulInstructionCpu>();
}

} // namespace chris_pikul_mos6502_toolbox
