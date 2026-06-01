#pragma once

#include <cstdint>

namespace applewin_toolbox {

constexpr std::uint16_t nmi_vector = 0xfffa;
constexpr std::uint16_t reset_vector = 0xfffc;
constexpr std::uint16_t irq_brk_vector = 0xfffe;

enum class CpuMode : std::uint8_t {
    nmos6502,
    cmos65c02,
};

struct State {
    std::uint8_t a;
    std::uint8_t x;
    std::uint8_t y;
    std::uint8_t p;
    std::uint16_t pc;
    std::uint16_t sp;
    bool jammed;
    CpuMode mode;
};

namespace detail {

struct Registers {
    std::uint8_t a;
    std::uint8_t x;
    std::uint8_t y;
    std::uint8_t ps;
    std::uint16_t pc;
    std::uint16_t sp;
    std::uint8_t bJammed;
};

extern Registers registers;
extern CpuMode current_mode;
extern bool irq_asserted;
extern bool nmi_pending_flag;
extern std::uint8_t ram[65536];
extern std::uint64_t cumulative_cycles_counter;

std::uint32_t execute_nmos6502(std::uint32_t cycles);
std::uint32_t execute_cmos65c02(std::uint32_t cycles);
void reset_memory_storage();

} // namespace detail

void reset_memory();
inline std::uint8_t* memory() { return detail::ram; }
inline const std::uint8_t* memory_data() { return detail::ram; }
inline std::uint8_t read_memory(std::uint16_t address) { return detail::ram[address]; }
inline void write_memory(std::uint16_t address, std::uint8_t value) { detail::ram[address] = value; }
inline void load_program(std::uint16_t address, const std::uint8_t* data, std::uint32_t size) {
    for(std::uint32_t i = 0; i < size; ++i) {
        detail::ram[(address + i) & 0xffffu] = data[i];
    }
}
inline std::uint16_t read_word(std::uint16_t address) {
    return static_cast<std::uint16_t>(detail::ram[address] | (static_cast<std::uint16_t>(detail::ram[(address + 1u) & 0xffffu]) << 8));
}
inline void set_vector(std::uint16_t vector_address, std::uint16_t target_address) {
    detail::ram[vector_address] = static_cast<std::uint8_t>(target_address & 0xffu);
    detail::ram[(vector_address + 1u) & 0xffffu] = static_cast<std::uint8_t>(target_address >> 8);
}
inline void set_reset_vector(std::uint16_t target_address) { set_vector(reset_vector, target_address); }
inline void set_nmi_vector(std::uint16_t target_address) { set_vector(nmi_vector, target_address); }
inline void set_irq_brk_vector(std::uint16_t target_address) { set_vector(irq_brk_vector, target_address); }

inline void reset_cpu(std::uint16_t pc, CpuMode mode = CpuMode::nmos6502) {
    detail::current_mode = mode;
    detail::registers.a = 0;
    detail::registers.x = 0;
    detail::registers.y = 0;
    detail::registers.ps = 0x20u | 0x04u;
    detail::registers.pc = pc;
    detail::registers.sp = 0x01fdu;
    detail::registers.bJammed = 0;
    detail::irq_asserted = false;
    detail::nmi_pending_flag = false;
    detail::cumulative_cycles_counter = 0;
}
inline void reset_cpu(std::uint16_t pc, bool cmos) {
    reset_cpu(pc, cmos ? CpuMode::cmos65c02 : CpuMode::nmos6502);
}
inline void reset_from_vector(CpuMode mode = CpuMode::nmos6502) {
    reset_cpu(read_word(reset_vector), mode);
}
inline void set_state(const State& state) {
    detail::current_mode = state.mode;
    detail::registers.a = state.a;
    detail::registers.x = state.x;
    detail::registers.y = state.y;
    detail::registers.ps = state.p;
    detail::registers.pc = state.pc;
    detail::registers.sp = state.sp;
    detail::registers.bJammed = state.jammed ? 1 : 0;
}
inline State state() {
    return State{
        detail::registers.a,
        detail::registers.x,
        detail::registers.y,
        detail::registers.ps,
        detail::registers.pc,
        detail::registers.sp,
        detail::registers.bJammed != 0,
        detail::current_mode,
    };
}

inline void set_a(std::uint8_t value) { detail::registers.a = value; }
inline void set_x(std::uint8_t value) { detail::registers.x = value; }
inline void set_y(std::uint8_t value) { detail::registers.y = value; }
inline void set_p(std::uint8_t value) { detail::registers.ps = value; }
inline void set_pc(std::uint16_t value) { detail::registers.pc = value; }
inline void set_sp(std::uint16_t value) { detail::registers.sp = value; }
inline void set_jammed(bool value) { detail::registers.bJammed = value ? 1 : 0; }

inline std::uint8_t a() { return detail::registers.a; }
inline std::uint8_t x() { return detail::registers.x; }
inline std::uint8_t y() { return detail::registers.y; }
inline std::uint8_t p() { return detail::registers.ps; }
inline std::uint16_t pc() { return detail::registers.pc; }
inline std::uint16_t sp() { return detail::registers.sp; }
inline bool jammed() { return detail::registers.bJammed != 0; }
inline CpuMode mode() { return detail::current_mode; }
inline void set_mode(CpuMode mode) { detail::current_mode = mode; }

inline void set_irq_line(bool asserted) { detail::irq_asserted = asserted; }
inline void clear_irq_line() { detail::irq_asserted = false; }
inline void pulse_nmi() { detail::nmi_pending_flag = true; }
inline void set_nmi_pending(bool pending) { detail::nmi_pending_flag = pending; }
inline void clear_nmi_pending() { detail::nmi_pending_flag = false; }
inline void clear_interrupt_lines() {
    detail::irq_asserted = false;
    detail::nmi_pending_flag = false;
}
inline bool irq_line() { return detail::irq_asserted; }
inline bool nmi_pending() { return detail::nmi_pending_flag; }
inline bool interrupt_pending() { return detail::nmi_pending_flag || detail::irq_asserted; }

inline std::uint32_t execute(std::uint32_t cycles) {
    const auto executed = (detail::current_mode == CpuMode::cmos65c02)
        ? detail::execute_cmos65c02(cycles)
        : detail::execute_nmos6502(cycles);
    detail::cumulative_cycles_counter += executed;
    return executed;
}
inline std::uint32_t execute_one_instruction() { return execute(1); }
inline std::uint64_t cumulative_cycles() { return detail::cumulative_cycles_counter; }
inline void clear_cumulative_cycles() { detail::cumulative_cycles_counter = 0; }

} // namespace applewin_toolbox
