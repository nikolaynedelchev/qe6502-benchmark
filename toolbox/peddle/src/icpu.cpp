#include <peddle_toolbox.hpp>

#include <cpu6502_bridge/cpu.hpp>
#include <Peddle.h>

#include <array>
#include <cstdint>
#include <memory>

namespace peddle_toolbox {
namespace {

class PeddleCpu final : public peddle::Peddle, public cpu6502_bridge::ICpu {
public:
    PeddleCpu()
    {
        setModel(peddle::MOS_6502);
        memory_.fill(0u);
        restart();
    }

    const char* get_name() const noexcept override { return "Peddle NMOS"; }

    void restart() noexcept override
    {
        last_address_ = 0u;
        last_data_ = 0u;
        last_is_write_ = false;
        last_is_opcode_fetch_ = false;
        irq_asserted_ = false;
        nmi_asserted_ = false;
        opcode_fetch_cycle_ = false;
        reset();
    }


    unsigned restart_to_start_fetch(unsigned /*max_steps*/ = 64) override
    {
        const auto start_address = static_cast<std::uint16_t>(
            memory_[0xfffcu] | (static_cast<std::uint16_t>(memory_[0xfffdu]) << 8u));
        restart();
        last_address_ = start_address;
        last_data_ = memory_[start_address];
        last_is_write_ = false;
        last_is_opcode_fetch_ = true;
        return 0u;
    }

    void irq(const bool assert_irq) noexcept override
    {
        if (assert_irq == irq_asserted_) {
            return;
        }
        irq_asserted_ = assert_irq;
        if (assert_irq) {
            pullDownIrqLine(interrupt_source_);
        } else {
            releaseIrqLine(interrupt_source_);
        }
    }

    bool is_irq_asserted() const noexcept override { return irq_asserted_; }

    void nmi(const bool assert_nmi) noexcept override
    {
        if (assert_nmi == nmi_asserted_) {
            return;
        }
        nmi_asserted_ = assert_nmi;
        if (assert_nmi) {
            pullDownNmiLine(interrupt_source_);
        } else {
            releaseNmiLine(interrupt_source_);
        }
    }

    bool is_nmi_asserted() const noexcept override { return nmi_asserted_; }

    void set_bus_data(const std::uint8_t data) noexcept override
    {
        if (!last_is_write_) {
            memory_[last_address_] = data;
            last_data_ = data;
        }
    }

    void step() noexcept override
    {
        opcode_fetch_cycle_ = inFetchPhase() && !doNmi && !doIrq;
        execute();
        opcode_fetch_cycle_ = false;
    }

    std::uint16_t bus_address() const noexcept override { return last_address_; }
    std::uint8_t bus_data() const noexcept override { return last_data_; }
    std::uint8_t* memory() noexcept override { return memory_.data(); }
    bool is_write() const noexcept override { return last_is_write_; }
    bool is_opcode_fetch() const noexcept override { return last_is_opcode_fetch_; }

    std::uint16_t pc() const noexcept override { return reg.pc; }
    std::uint8_t s() const noexcept override { return static_cast<std::uint8_t>(reg.sp); }
    std::uint8_t a() const noexcept override { return reg.a; }
    std::uint8_t x() const noexcept override { return reg.x; }
    std::uint8_t y() const noexcept override { return reg.y; }
    std::uint8_t p() const noexcept override { return getP(); }

    peddle::u8 read(const peddle::u16 address) override
    {
        last_address_ = address;
        last_data_ = memory_[address];
        last_is_write_ = false;
        last_is_opcode_fetch_ = opcode_fetch_cycle_;
        return last_data_;
    }

    void write(const peddle::u16 address, const peddle::u8 value) override
    {
        memory_[address] = value;
        last_address_ = address;
        last_data_ = value;
        last_is_write_ = true;
        last_is_opcode_fetch_ = false;
    }

    peddle::u8 readDasm(const peddle::u16 address) const override
    {
        return memory_[address];
    }

private:
    static constexpr peddle::IntSource interrupt_source_ = 1;

    std::array<std::uint8_t, 65536> memory_{};
    std::uint16_t last_address_ = 0u;
    std::uint8_t last_data_ = 0u;
    bool last_is_write_ = false;
    bool last_is_opcode_fetch_ = false;
    bool irq_asserted_ = false;
    bool nmi_asserted_ = false;
    bool opcode_fetch_cycle_ = false;
};

} // namespace

std::unique_ptr<cpu6502_bridge::ICpu> make_peddle_nmos_cpu()
{
    return std::make_unique<PeddleCpu>();
}

} // namespace peddle_toolbox
