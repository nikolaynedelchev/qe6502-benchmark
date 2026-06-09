#include <olcNES_toolbox.hpp>

#include <Bus.h>

#include <cstdint>
#include <memory>

namespace olcNES_toolbox {
namespace {

constexpr std::uint8_t flag_i = 0x04u;
constexpr std::uint8_t flag_u = 0x20u;

class OlcNesInstructionCpu final : public benchmark6502::IInstructionCpu {
public:
    OlcNesInstructionCpu() { reset(); }

    std::string_view core_name() const override { return "olcNES"; }
    std::string_view model_name() const override { return "NMOS/NES-style"; }

    void clear_memory(std::uint8_t fill) override { bus_.ram.fill(fill); }

    void load(std::span<const asm6502::mem_value> program) override
    {
        for (const auto& [address, value] : program) bus_.ram[address] = value;
    }

    void reset() override
    {
        instruction_count_ = 0;
        clear_scheduled_irq_deassert();
        clear_scheduled_nmi_deassert();
        irq_asserted_ = false;
        nmi_pending_ = false;
        bus_.cpu.reset();
        while (!bus_.cpu.complete()) bus_.cpu.clock();
        bus_.cpu.status |= flag_i | flag_u;
    }

    void step_instruction() override
    {
        if (nmi_pending_) {
            enter_interrupt(0xfffau, false);
            nmi_pending_ = false;
            drain_current_operation();
        } else if (irq_asserted_ && (bus_.cpu.status & flag_i) == 0u) {
            enter_interrupt(0xfffeu, false);
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

    std::uint8_t read(std::uint16_t address) const override { return bus_.ram[address]; }
    void write(std::uint16_t address, std::uint8_t value) override { bus_.ram[address] = value; }

    benchmark6502::instruction_cpu_state state() const override
    {
        return benchmark6502::instruction_cpu_state{
            bus_.cpu.pc, bus_.cpu.a, bus_.cpu.x, bus_.cpu.y, bus_.cpu.stkp,
            static_cast<std::uint8_t>(bus_.cpu.status | flag_u)};
    }

private:
    void run_one_instruction()
    {
        do { bus_.cpu.clock(); } while (!bus_.cpu.complete());
    }

    void drain_current_operation()
    {
        while (!bus_.cpu.complete()) bus_.cpu.clock();
    }

    void enter_interrupt(std::uint16_t vector, bool break_bit)
    {
        const std::uint8_t old_status = bus_.cpu.status;
        if (vector == 0xfffau) {
            bus_.cpu.nmi();
        } else {
            bus_.cpu.irq();
        }
        const std::uint8_t pushed_p = static_cast<std::uint8_t>(((old_status | flag_u) & ~0x10u) | (break_bit ? 0x10u : 0u));
        bus_.write(static_cast<std::uint16_t>(0x0100u + bus_.cpu.stkp + 1u), pushed_p);
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

    Bus bus_{};
    std::uint64_t instruction_count_ = 0;
    std::uint64_t irq_deassert_at_ = 0;
    std::uint64_t nmi_deassert_at_ = 0;
    bool has_irq_deassert_at_ = false;
    bool has_nmi_deassert_at_ = false;
    bool irq_asserted_ = false;
    bool nmi_pending_ = false;
};

} // namespace

std::unique_ptr<benchmark6502::IInstructionCpu> make_olcNES_nmos_instruction_cpu()
{
    return std::make_unique<OlcNesInstructionCpu>();
}

} // namespace olcNES_toolbox
