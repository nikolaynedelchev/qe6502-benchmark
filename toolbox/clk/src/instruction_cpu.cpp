#include <clk_toolbox.hpp>

#include <Processors/6502/6502.hpp>

#include <array>
#include <cstdint>
#include <memory>

namespace clk_toolbox {
namespace {

class ClkP6502InstructionCpu;

class ClkInstructionBus final : public CPU::MOS6502::BusHandler {
public:
    using AddressType = std::uint16_t;

    explicit ClkInstructionBus(ClkP6502InstructionCpu& owner) noexcept
        : owner_(owner)
    {
    }

    Cycles perform_bus_operation(CPU::MOS6502::BusOperation operation,
                                 AddressType address,
                                 std::uint8_t* value);

    std::array<std::uint8_t, 65536> memory{};
    CPU::MOS6502::BusOperation last_operation = CPU::MOS6502::BusOperation::Read;
    std::uint16_t last_address = 0;
    std::uint8_t last_data = 0;

private:
    ClkP6502InstructionCpu& owner_;
};

using ClkP6502 = CPU::MOS6502::Processor<CPU::MOS6502::P6502, ClkInstructionBus, false>;

class ClkP6502InstructionCpu final : public benchmark6502::IInstructionCpu {
public:
    ClkP6502InstructionCpu()
        : bus_(*this)
    {
        reset();
    }

    std::string_view core_name() const override { return "CLK"; }
    std::string_view model_name() const override { return "P6502"; }

    void clear_memory(std::uint8_t fill) override { bus_.memory.fill(fill); }

    void load(std::span<const asm6502::mem_value> program) override
    {
        for (const auto& [address, value] : program) {
            bus_.memory[address] = value;
        }
    }

    void reset() override
    {
        instruction_count_ = 0;
        clear_scheduled_irq_deassert();
        clear_scheduled_nmi_deassert();
        irq_asserted_ = false;
        nmi_asserted_ = false;
        at_opcode_fetch_ = false;
        cpu_ = std::make_unique<ClkP6502>(bus_);
        run_until_opcode_fetch(128u);
    }

    void step_instruction() override
    {
        at_opcode_fetch_ = false;
        run_until_opcode_fetch(256u);
        ++instruction_count_;
    }

    void assert_irq_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            set_irq(false);
            clear_scheduled_irq_deassert();
            return;
        }

        set_irq(true);
        const std::uint64_t line_visibility_slop = (state().p & 0x04u) == 0u ? 1u : 0u;
        irq_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count) + line_visibility_slop;
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

    std::uint8_t read(std::uint16_t address) const override { return bus_.memory[address]; }
    void write(std::uint16_t address, std::uint8_t value) override { bus_.memory[address] = value; }

    benchmark6502::instruction_cpu_state state() const override
    {
        return benchmark6502::instruction_cpu_state{
            visible_pc(),
            static_cast<std::uint8_t>(cpu_->value_of(CPU::MOS6502::Register::A)),
            static_cast<std::uint8_t>(cpu_->value_of(CPU::MOS6502::Register::X)),
            static_cast<std::uint8_t>(cpu_->value_of(CPU::MOS6502::Register::Y)),
            static_cast<std::uint8_t>(cpu_->value_of(CPU::MOS6502::Register::StackPointer)),
            static_cast<std::uint8_t>(cpu_->value_of(CPU::MOS6502::Register::Flags)),
        };
    }

    void before_bus_operation(CPU::MOS6502::BusOperation operation)
    {
        if (operation != CPU::MOS6502::BusOperation::ReadOpcode) {
            return;
        }

        const auto completed_count = instruction_count_ + 1u;
        if (has_irq_deassert_at_ && completed_count >= irq_deassert_at_) {
            set_irq(false);
            clear_scheduled_irq_deassert();
        }
        if (has_nmi_deassert_at_ && completed_count >= nmi_deassert_at_) {
            set_nmi(false);
            clear_scheduled_nmi_deassert();
        }
    }

    void after_bus_operation(CPU::MOS6502::BusOperation operation)
    {
        at_opcode_fetch_ = operation == CPU::MOS6502::BusOperation::ReadOpcode;
    }

private:
    std::uint16_t visible_pc() const
    {
        if (at_opcode_fetch_) {
            return bus_.last_address;
        }
        return static_cast<std::uint16_t>(cpu_->value_of(CPU::MOS6502::Register::ProgramCounter));
    }

    void run_until_opcode_fetch(unsigned max_cycles)
    {
        for (unsigned cycle = 0; cycle < max_cycles; ++cycle) {
            if (at_opcode_fetch_) {
                return;
            }
            cpu_->run_for(Cycles(1));
        }
    }

    void set_irq(bool asserted)
    {
        irq_asserted_ = asserted;
        cpu_->set_irq_line(asserted);
    }

    void set_nmi(bool asserted)
    {
        nmi_asserted_ = asserted;
        cpu_->set_nmi_line(asserted);
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

    ClkInstructionBus bus_;
    std::unique_ptr<ClkP6502> cpu_{};
    std::uint64_t instruction_count_ = 0;
    std::uint64_t irq_deassert_at_ = 0;
    std::uint64_t nmi_deassert_at_ = 0;
    bool has_irq_deassert_at_ = false;
    bool has_nmi_deassert_at_ = false;
    bool irq_asserted_ = false;
    bool nmi_asserted_ = false;
    bool at_opcode_fetch_ = false;
};

Cycles ClkInstructionBus::perform_bus_operation(CPU::MOS6502::BusOperation operation,
                                                AddressType address,
                                                std::uint8_t* value)
{
    owner_.before_bus_operation(operation);

    last_operation = operation;
    last_address = address;

    if (CPU::MOS6502Esque::is_write(operation)) {
        last_data = *value;
        memory[address] = *value;
    } else if (CPU::MOS6502Esque::is_read(operation)) {
        *value = memory[address];
        last_data = *value;
    } else {
        last_data = memory[address];
    }

    owner_.after_bus_operation(operation);
    return Cycles(1);
}

} // namespace

std::unique_ptr<benchmark6502::IInstructionCpu> make_clk_p6502_nmos_instruction_cpu()
{
    return std::make_unique<ClkP6502InstructionCpu>();
}

} // namespace clk_toolbox
