#pragma once

#include <array>
#include <cstdint>

namespace benchmark6502::mame {

enum class CpuMode {
    nmos6502,
    wdc65c02,
    rockwell65c02,
    wdc65c02s,
};

struct CpuState {
    std::uint8_t a = 0;
    std::uint8_t x = 0;
    std::uint8_t y = 0;
    std::uint8_t p = 0x34;
    std::uint8_t sp = 0xff;
    std::uint16_t pc = 0;
    bool jammed = false;
};

class Mame6502Cpu {
public:
    explicit Mame6502Cpu(CpuMode mode = CpuMode::nmos6502);

    CpuMode mode() const { return mode_; }
    void set_mode(CpuMode mode);

    void clear_memory(std::uint8_t value = 0);
    void load_program(std::uint16_t address, const std::uint8_t* data, std::size_t size);
    void set_vector(std::uint16_t vector_address, std::uint16_t target);
    void set_reset_vector(std::uint16_t target) { set_vector(0xfffc, target); }
    void set_nmi_vector(std::uint16_t target) { set_vector(0xfffa, target); }
    void set_irq_brk_vector(std::uint16_t target) { set_vector(0xfffe, target); }

    void reset_from_vector();
    void set_pc_and_prefetch(std::uint16_t pc);

    std::uint8_t read_memory(std::uint16_t address) const { return memory_[address]; }
    void write_memory(std::uint16_t address, std::uint8_t value) { memory_[address] = value; }

    unsigned execute(unsigned cycles);
    unsigned execute_one_instruction() { return execute(1); }

    CpuState state() const;
    void set_state(const CpuState& state);

    std::uint8_t a() const { return a_; }
    std::uint8_t x() const { return x_; }
    std::uint8_t y() const { return y_; }
    std::uint8_t p() const { return p_; }
    std::uint8_t sp() const { return static_cast<std::uint8_t>(sp_); }
    std::uint16_t pc() const { return pc_; }
    bool is_jammed() const { return jammed_; }
    unsigned total_cycles() const { return total_cycles_; }

    void set_a(std::uint8_t value) { a_ = value; }
    void set_x(std::uint8_t value) { x_ = value; }
    void set_y(std::uint8_t value) { y_ = value; }
    void set_p(std::uint8_t value) { p_ = value; }
    void set_sp(std::uint8_t value) { sp_ = static_cast<std::uint16_t>(0x0100u | value); }
    void set_jammed(bool value) { jammed_ = value; }
    void clear_jammed() { jammed_ = false; }
    void reset_cycle_counter() { total_cycles_ = 0; }

    void set_irq_line(bool asserted) { irq_state_ = asserted; }
    void clear_irq_line() { set_irq_line(false); }
    void set_nmi_line(bool asserted);
    void pulse_nmi() { set_nmi_line(true); set_nmi_line(false); }
    void set_so_line(bool asserted);
    bool interrupt_pending() const { return nmi_pending_ || (irq_state_ && !(p_ & 0x04)); }

    unsigned read_count() const { return read_count_; }
    unsigned write_count() const { return write_count_; }
    unsigned sync_read_count() const { return sync_read_count_; }

private:
    friend class mame6502_cpu_device;
    friend class mamew65c02_cpu_device;
    friend class mamer65c02_cpu_device;
    friend class mamew65c02s_cpu_device;

    CpuMode mode_ = CpuMode::nmos6502;
    std::array<std::uint8_t, 65536> memory_{};
    unsigned total_cycles_ = 0;
    unsigned read_count_ = 0;
    unsigned write_count_ = 0;
    unsigned sync_read_count_ = 0;

    std::uint16_t ppc_ = 0;
    std::uint16_t npc_ = 0;
    std::uint16_t pc_ = 0;
    std::uint16_t sp_ = 0x0100;
    std::uint16_t tmp_ = 0;
    std::uint8_t tmp2_ = 0;
    std::uint8_t a_ = 0;
    std::uint8_t x_ = 0;
    std::uint8_t y_ = 0;
    std::uint8_t p_ = 0x36;
    std::uint8_t ir_ = 0;
    int inst_state_base_ = 0;
    int inst_state_ = 0;
    int inst_substate_ = 0;
    int icount_ = 0;
    bool nmi_state_ = false;
    bool irq_state_ = false;
    bool v_state_ = false;
    bool nmi_pending_ = false;
    bool irq_taken_ = false;
    bool sync_ = false;
    bool inhibit_interrupts_ = false;
    bool jammed_ = false;

    std::uint8_t read(std::uint16_t address);
    std::uint8_t read_sync(std::uint16_t address);
    std::uint8_t read_arg(std::uint16_t address);
    void write(std::uint16_t address, std::uint8_t value);
};

} // namespace benchmark6502::mame
