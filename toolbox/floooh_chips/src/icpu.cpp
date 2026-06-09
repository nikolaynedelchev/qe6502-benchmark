#include <floooh_chips_toolbox.hpp>

#include <cpu6502_bridge/cpu.hpp>

#include "chips/m6502.h"

#include <array>
#include <cstdint>
#include <memory>

namespace floooh_chips_toolbox {
namespace {

class FlooohChipsCpu final : public cpu6502_bridge::ICpu {
public:
    FlooohChipsCpu() noexcept
    {
        restart();
    }

    const char* get_name() const noexcept override { return "floooh/chips m6502"; }

    void restart() noexcept override
    {
        m6502_desc_t desc{};
        pins_ = m6502_init(&cpu_, &desc);
        irq_asserted_ = false;
        nmi_asserted_ = false;
    }

    void irq(bool assert_irq) noexcept override
    {
        irq_asserted_ = assert_irq;
        if (assert_irq) {
            pins_ |= M6502_IRQ;
        } else {
            pins_ &= ~M6502_IRQ;
        }
    }

    bool is_irq_asserted() const noexcept override
    {
        return irq_asserted_;
    }

    void nmi(bool assert_nmi) noexcept override
    {
        nmi_asserted_ = assert_nmi;
        if (assert_nmi) {
            pins_ |= M6502_NMI;
        } else {
            pins_ &= ~M6502_NMI;
        }
    }

    bool is_nmi_asserted() const noexcept override
    {
        return nmi_asserted_;
    }

    void set_bus_data(std::uint8_t data) noexcept override
    {
        if (!is_write()) {
            memory_[bus_address()] = data;
            M6502_SET_DATA(pins_, data);
        }
    }

    void step() noexcept override
    {
        const std::uint16_t address = bus_address();
        if (is_write()) {
            memory_[address] = M6502_GET_DATA(pins_);
        } else {
            M6502_SET_DATA(pins_, memory_[address]);
        }

        pins_ = m6502_tick(&cpu_, pins_);
        if (irq_asserted_) {
            pins_ |= M6502_IRQ;
        } else {
            pins_ &= ~M6502_IRQ;
        }
        if (nmi_asserted_) {
            pins_ |= M6502_NMI;
        } else {
            pins_ &= ~M6502_NMI;
        }
    }

    std::uint16_t bus_address() const noexcept override
    {
        return M6502_GET_ADDR(pins_);
    }

    std::uint8_t bus_data() const noexcept override
    {
        if (is_write()) {
            return M6502_GET_DATA(pins_);
        }
        return memory_[bus_address()];
    }

    std::uint8_t* memory() noexcept override { return memory_.data(); }

    bool is_write() const noexcept override
    {
        return (pins_ & M6502_RW) == 0u;
    }

    bool is_opcode_fetch() const noexcept override
    {
        return (pins_ & M6502_SYNC) != 0u;
    }

    std::uint16_t pc() const noexcept override { return m6502_pc(const_cast<m6502_t*>(&cpu_)); }
    std::uint8_t s() const noexcept override { return m6502_s(const_cast<m6502_t*>(&cpu_)); }
    std::uint8_t a() const noexcept override { return m6502_a(const_cast<m6502_t*>(&cpu_)); }
    std::uint8_t x() const noexcept override { return m6502_x(const_cast<m6502_t*>(&cpu_)); }
    std::uint8_t y() const noexcept override { return m6502_y(const_cast<m6502_t*>(&cpu_)); }
    std::uint8_t p() const noexcept override { return m6502_p(const_cast<m6502_t*>(&cpu_)); }

private:
    m6502_t cpu_{};
    std::uint64_t pins_ = 0u;
    bool irq_asserted_ = false;
    bool nmi_asserted_ = false;
    std::array<std::uint8_t, 65536> memory_{};
};

} // namespace

std::unique_ptr<cpu6502_bridge::ICpu> make_floooh_chips_nmos_cpu()
{
    return std::make_unique<FlooohChipsCpu>();
}

} // namespace floooh_chips_toolbox
