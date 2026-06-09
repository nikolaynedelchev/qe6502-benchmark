#include <o2_toolbox.hpp>

#include <cpu6502_bridge/cpu.hpp>
#include <O2.hpp>

#include <array>
#include <cstdint>
#include <memory>

namespace o2_toolbox {
namespace {

class O2Cpu final : public O2::CPU, public cpu6502_bridge::ICpu {
public:
    O2Cpu()
    {
        setRW(
            [this](const std::uint16_t address) { return read_memory(address); },
            [this](const std::uint16_t address, const std::uint8_t value) { write_memory(address, value); });
        memory_.fill(0u);
        restart();
    }

    const char* get_name() const noexcept override { return "O2 NMOS"; }

    void restart() noexcept override
    {
        reset_state();
        last_address_ = 0u;
        last_data_ = 0u;
        last_is_write_ = false;
        last_is_opcode_fetch_ = false;
        next_read_is_opcode_fetch_ = false;
        irq_asserted_ = false;
        nmi_asserted_ = false;
    }

    unsigned restart_to_start_fetch(unsigned /*max_steps*/ = 64) override
    {
        const auto start_address = static_cast<std::uint16_t>(
            memory_[0xfffcu] | (static_cast<std::uint16_t>(memory_[0xfffdu]) << 8u));
        restart();
        cold = false;
        intc = false;
        iflag = O2::INONE;
        state = O2::FETCH;
        PC = start_address;
        last_address_ = start_address;
        last_data_ = memory_[start_address];
        last_is_write_ = false;
        last_is_opcode_fetch_ = true;
        return 0u;
    }

    void irq(const bool assert_irq) noexcept override
    {
        irq_asserted_ = assert_irq;
        if (assert_irq) {
            raise(O2::IRQ);
        } else {
            unraise(O2::IRQ);
        }
    }

    bool is_irq_asserted() const noexcept override { return irq_asserted_; }

    void nmi(const bool assert_nmi) noexcept override
    {
        nmi_asserted_ = assert_nmi;
        if (assert_nmi) {
            raise(O2::NMI);
        } else {
            unraise(O2::NMI);
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
        next_read_is_opcode_fetch_ = (state == O2::FETCH && !intc);
        cycle();
        next_read_is_opcode_fetch_ = false;
    }

    std::uint16_t bus_address() const noexcept override { return last_address_; }
    std::uint8_t bus_data() const noexcept override { return last_data_; }
    std::uint8_t* memory() noexcept override { return memory_.data(); }
    bool is_write() const noexcept override { return last_is_write_; }
    bool is_opcode_fetch() const noexcept override { return last_is_opcode_fetch_; }

    std::uint16_t pc() const noexcept override { return PC; }
    std::uint8_t s() const noexcept override { return S; }
    std::uint8_t a() const noexcept override { return A; }
    std::uint8_t x() const noexcept override { return X; }
    std::uint8_t y() const noexcept override { return Y; }
    std::uint8_t p() const noexcept override { return status(); }

private:
    void reset_state() noexcept
    {
        A = 0u;
        X = 0u;
        Y = 0u;
        S = 0xfdu;
        PC = 0u;

        for (bool& flag : P) {
            flag = false;
        }
        P[O2::INTERRUPT] = true;
        P[O2::B0] = true;
        P[O2::B1] = true;

        cold = true;
        state = O2::FETCH;
        rnmi = false;
        rirq = false;
        rbrk = false;
        cbrk = false;
        intc = true;
        iflag = O2::RESET;
        hlt = false;
        cycle_count = 0u;
        instruction_cycles = 0u;
        tsc = 0u;
        addm = O2::IMPLIED;
        opcode = 0u;
        wv0 = 0u;
        wv1 = 0u;
        page = 0u;
        av = 0u;
    }

    std::uint8_t status() const noexcept
    {
        return static_cast<std::uint8_t>((P[O2::CARRY] ? 0x01u : 0u) |
                                         (P[O2::ZERO] ? 0x02u : 0u) |
                                         (P[O2::INTERRUPT] ? 0x04u : 0u) |
                                         (P[O2::DECIMAL] ? 0x08u : 0u) |
                                         (P[O2::B0] ? 0x10u : 0u) |
                                         (P[O2::B1] ? 0x20u : 0u) |
                                         (P[O2::OVERFLOW_FLAG] ? 0x40u : 0u) |
                                         (P[O2::NEGATIVE] ? 0x80u : 0u));
    }

    std::uint8_t read_memory(const std::uint16_t address) noexcept
    {
        last_address_ = address;
        last_data_ = memory_[address];
        last_is_write_ = false;
        last_is_opcode_fetch_ = next_read_is_opcode_fetch_;
        return last_data_;
    }

    void write_memory(const std::uint16_t address, const std::uint8_t value) noexcept
    {
        memory_[address] = value;
        last_address_ = address;
        last_data_ = value;
        last_is_write_ = true;
        last_is_opcode_fetch_ = false;
    }

    std::array<std::uint8_t, 65536> memory_{};
    std::uint16_t last_address_ = 0u;
    std::uint8_t last_data_ = 0u;
    bool last_is_write_ = false;
    bool last_is_opcode_fetch_ = false;
    bool next_read_is_opcode_fetch_ = false;
    bool irq_asserted_ = false;
    bool nmi_asserted_ = false;
};

} // namespace

std::unique_ptr<cpu6502_bridge::ICpu> make_o2_nmos_cpu()
{
    return std::make_unique<O2Cpu>();
}

} // namespace o2_toolbox
