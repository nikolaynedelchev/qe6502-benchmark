#include <clk_toolbox.hpp>

#include <cpu6502_bridge/cpu.hpp>
#include <Processors/6502/6502.hpp>

#include <array>
#include <cstdint>
#include <memory>

namespace clk_toolbox {
namespace {

class ClkBus final : public CPU::MOS6502::BusHandler {
public:
    using AddressType = std::uint16_t;

    std::array<std::uint8_t, 65536> memory{};

    Cycles perform_bus_operation(const CPU::MOS6502::BusOperation operation,
                                 const AddressType address,
                                 std::uint8_t* const value)
    {
        last_operation_ = operation;
        last_address_ = address;

        if (CPU::MOS6502Esque::is_write(operation)) {
            last_data_ = *value;
            memory[address] = *value;
        } else if (CPU::MOS6502Esque::is_read(operation)) {
            *value = memory[address];
            last_data_ = *value;
        } else {
            last_data_ = memory[address];
        }

        return Cycles(1);
    }

    std::uint16_t last_address() const noexcept { return last_address_; }
    std::uint8_t last_data() const noexcept { return last_data_; }
    CPU::MOS6502::BusOperation last_operation() const noexcept { return last_operation_; }

    void set_last_read_data(const std::uint8_t data) noexcept
    {
        if (!CPU::MOS6502Esque::is_write(last_operation_)) {
            memory[last_address_] = data;
            last_data_ = data;
        }
    }

private:
    CPU::MOS6502::BusOperation last_operation_ = CPU::MOS6502::BusOperation::Read;
    std::uint16_t last_address_ = 0u;
    std::uint8_t last_data_ = 0u;
};

using ClkP6502 = CPU::MOS6502::Processor<CPU::MOS6502::P6502, ClkBus, false>;

class ClkP6502Cpu final : public cpu6502_bridge::ICpu {
public:
    ClkP6502Cpu()
    {
        restart();
    }

    const char* get_name() const noexcept override { return "clk P6502"; }

    void restart() noexcept override
    {
        cpu_ = std::make_unique<ClkP6502>(bus_);
        irq_asserted_ = false;
        nmi_asserted_ = false;
    }

    void irq(const bool assert_irq) noexcept override
    {
        irq_asserted_ = assert_irq;
        cpu_->set_irq_line(assert_irq);
    }

    bool is_irq_asserted() const noexcept override { return irq_asserted_; }

    void nmi(const bool assert_nmi) noexcept override
    {
        nmi_asserted_ = assert_nmi;
        cpu_->set_nmi_line(assert_nmi);
    }

    bool is_nmi_asserted() const noexcept override { return nmi_asserted_; }

    void set_bus_data(const std::uint8_t data) noexcept override
    {
        bus_.set_last_read_data(data);
    }

    void step() noexcept override
    {
        cpu_->run_for(Cycles(1));
    }

    std::uint16_t bus_address() const noexcept override { return bus_.last_address(); }
    std::uint8_t bus_data() const noexcept override { return bus_.last_data(); }
    std::uint8_t* memory() noexcept override { return bus_.memory.data(); }

    bool is_write() const noexcept override
    {
        return CPU::MOS6502Esque::is_write(bus_.last_operation());
    }

    bool is_opcode_fetch() const noexcept override
    {
        return bus_.last_operation() == CPU::MOS6502::BusOperation::ReadOpcode;
    }

    std::uint16_t pc() const noexcept override
    {
        return static_cast<std::uint16_t>(cpu_->value_of(CPU::MOS6502::Register::ProgramCounter));
    }

    std::uint8_t s() const noexcept override
    {
        return static_cast<std::uint8_t>(cpu_->value_of(CPU::MOS6502::Register::StackPointer));
    }

    std::uint8_t a() const noexcept override
    {
        return static_cast<std::uint8_t>(cpu_->value_of(CPU::MOS6502::Register::A));
    }

    std::uint8_t x() const noexcept override
    {
        return static_cast<std::uint8_t>(cpu_->value_of(CPU::MOS6502::Register::X));
    }

    std::uint8_t y() const noexcept override
    {
        return static_cast<std::uint8_t>(cpu_->value_of(CPU::MOS6502::Register::Y));
    }

    std::uint8_t p() const noexcept override
    {
        return static_cast<std::uint8_t>(cpu_->value_of(CPU::MOS6502::Register::Flags));
    }

private:
    ClkBus bus_{};
    std::unique_ptr<ClkP6502> cpu_{};
    bool irq_asserted_ = false;
    bool nmi_asserted_ = false;
};

} // namespace

std::unique_ptr<cpu6502_bridge::ICpu> make_clk_p6502_nmos_cpu()
{
    return std::make_unique<ClkP6502Cpu>();
}

} // namespace clk_toolbox
