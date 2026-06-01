#pragma once

#include <cstdint>

namespace applewin_toolbox {

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

void reset_memory();
std::uint8_t* memory();
const std::uint8_t* memory_data();
std::uint8_t read_memory(std::uint16_t address);
void write_memory(std::uint16_t address, std::uint8_t value);
void load_program(std::uint16_t address, const std::uint8_t* data, std::uint32_t size);
void set_vector(std::uint16_t vector_address, std::uint16_t target_address);

void reset_cpu(std::uint16_t pc, CpuMode mode = CpuMode::nmos6502);
void reset_cpu(std::uint16_t pc, bool cmos);
void reset_from_vector(CpuMode mode = CpuMode::nmos6502);
void set_state(const State& state);
State state();

void set_a(std::uint8_t value);
void set_x(std::uint8_t value);
void set_y(std::uint8_t value);
void set_p(std::uint8_t value);
void set_pc(std::uint16_t value);
void set_sp(std::uint16_t value);
void set_jammed(bool value);

std::uint8_t a();
std::uint8_t x();
std::uint8_t y();
std::uint8_t p();
std::uint16_t pc();
std::uint16_t sp();
bool jammed();
CpuMode mode();

void set_irq_line(bool asserted);
void pulse_nmi();
void clear_interrupt_lines();
bool irq_line();
bool nmi_pending();

std::uint32_t execute(std::uint32_t cycles);
std::uint64_t cumulative_cycles();
void clear_cumulative_cycles();

} // namespace applewin_toolbox
