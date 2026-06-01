#include "benchmark6502/mame/mame6502_cpu.hpp"

#include <algorithm>
#include <cstdint>
#include <cstring>

namespace benchmark6502::mame {

namespace {
constexpr std::uint8_t F_N = 0x80;
constexpr std::uint8_t F_V = 0x40;
constexpr std::uint8_t F_E = 0x20;
constexpr std::uint8_t F_B = 0x10;
constexpr std::uint8_t F_D = 0x08;
constexpr std::uint8_t F_I = 0x04;
constexpr std::uint8_t F_Z = 0x02;
constexpr std::uint8_t F_C = 0x01;

constexpr int NMI_LINE = 2;
constexpr int IRQ_LINE = 0;
constexpr int V_LINE = 16;
constexpr int STATE_RESET = 0xff00;

inline bool page_changing(std::uint16_t base, int delta) { return ((base + delta) ^ base) & 0xff00; }
inline std::uint16_t set_l(std::uint16_t base, std::uint8_t value) { return (base & 0xff00u) | value; }
inline std::uint16_t set_h(std::uint16_t base, std::uint8_t value) { return (base & 0x00ffu) | (static_cast<std::uint16_t>(value) << 8); }
} // namespace


#define DECLARE_MAME_6502_DEVICE_CLASS_BEGIN(CLASS_NAME) \
class CLASS_NAME { \
public: \
    explicit CLASS_NAME(Mame6502Cpu& owner) : \
        owner_(owner), \
        m_PPC(owner.ppc_), \
        m_NPC(owner.npc_), \
        m_PC(owner.pc_), \
        m_SP(owner.sp_), \
        m_TMP(owner.tmp_), \
        m_TMP2(owner.tmp2_), \
        m_A(owner.a_), \
        m_X(owner.x_), \
        m_Y(owner.y_), \
        m_P(owner.p_), \
        m_IR(owner.ir_), \
        m_inst_state_base(owner.inst_state_base_), \
        m_inst_state(owner.inst_state_), \
        m_inst_substate(owner.inst_substate_), \
        m_icount(owner.icount_), \
        m_nmi_state(owner.nmi_state_), \
        m_irq_state(owner.irq_state_), \
        m_v_state(owner.v_state_), \
        m_nmi_pending(owner.nmi_pending_), \
        m_irq_taken(owner.irq_taken_), \
        m_sync(owner.sync_), \
        m_inhibit_interrupts(owner.inhibit_interrupts_) {} \
    void execute_run(); \
protected: \
    Mame6502Cpu& owner_; \
    std::uint16_t& m_PPC; \
    std::uint16_t& m_NPC; \
    std::uint16_t& m_PC; \
    std::uint16_t& m_SP; \
    std::uint16_t& m_TMP; \
    std::uint8_t& m_TMP2; \
    std::uint8_t& m_A; \
    std::uint8_t& m_X; \
    std::uint8_t& m_Y; \
    std::uint8_t& m_P; \
    std::uint8_t& m_IR; \
    int& m_inst_state_base; \
    int& m_inst_state; \
    int& m_inst_substate; \
    int& m_icount; \
    bool& m_nmi_state; \
    bool& m_irq_state; \
    bool& m_v_state; \
    bool& m_nmi_pending; \
    bool& m_irq_taken; \
    bool& m_sync; \
    bool& m_inhibit_interrupts; \
    static constexpr std::uint8_t F_N = ::benchmark6502::mame::F_N; \
    static constexpr std::uint8_t F_V = ::benchmark6502::mame::F_V; \
    static constexpr std::uint8_t F_E = ::benchmark6502::mame::F_E; \
    static constexpr std::uint8_t F_B = ::benchmark6502::mame::F_B; \
    static constexpr std::uint8_t F_D = ::benchmark6502::mame::F_D; \
    static constexpr std::uint8_t F_I = ::benchmark6502::mame::F_I; \
    static constexpr std::uint8_t F_Z = ::benchmark6502::mame::F_Z; \
    static constexpr std::uint8_t F_C = ::benchmark6502::mame::F_C; \
    static constexpr int NMI_LINE = ::benchmark6502::mame::NMI_LINE; \
    static constexpr int IRQ_LINE = ::benchmark6502::mame::IRQ_LINE; \
    static constexpr int V_LINE = ::benchmark6502::mame::V_LINE; \
    static constexpr int STATE_RESET = ::benchmark6502::mame::STATE_RESET; \
    static bool page_changing(std::uint16_t base, int delta) { return ::benchmark6502::mame::page_changing(base, delta); } \
    static std::uint16_t set_l(std::uint16_t base, std::uint8_t value) { return ::benchmark6502::mame::set_l(base, value); } \
    static std::uint16_t set_h(std::uint16_t base, std::uint8_t value) { return ::benchmark6502::mame::set_h(base, value); } \
    std::uint8_t read(std::uint16_t address) { return owner_.read(address); } \
    std::uint8_t read_sync(std::uint16_t address) { return owner_.read_sync(address); } \
    std::uint8_t read_arg(std::uint16_t address) { return owner_.read_arg(address); } \
    std::uint8_t read_pc() { return read_arg(m_PC); } \
    std::uint8_t read_vector(std::uint16_t address) { return owner_.read_arg(address); } \
    void end_interrupt() {} \
    void write(std::uint16_t address, std::uint8_t value) { owner_.write(address, value); } \
    bool access_to_be_redone() const { return false; } \
    void debugger_wait_hook() {} \
    void standard_irq_callback(int, std::uint16_t) {} \
    void prefetch_start() { m_sync = true; m_NPC = m_PC; } \
    void prefetch_end() { m_sync = false; if((m_nmi_pending || (m_irq_state && !(m_P & F_I))) && !m_inhibit_interrupts) { m_irq_taken = true; m_IR = 0x00; } else { m_PC++; } } \
    void prefetch_end_noirq() { m_sync = false; m_PC++; } \
    void set_nz(std::uint8_t value) { m_P &= ~(F_Z | F_N); if(value & 0x80) m_P |= F_N; if(!value) m_P |= F_Z; } \
    void dec_SP() { m_SP = set_l(m_SP, static_cast<std::uint8_t>(m_SP - 1)); } \
    void inc_SP() { m_SP = set_l(m_SP, static_cast<std::uint8_t>(m_SP + 1)); } \
    void do_adc_d(std::uint8_t val); \
    void do_adc_nd(std::uint8_t val); \
    void do_sbc_d(std::uint8_t val); \
    void do_sbc_nd(std::uint8_t val); \
    void do_sbc_cd(std::uint8_t val); \
    void do_sbc_c(std::uint8_t val); \
    void do_arr_d(); \
    void do_arr_nd(); \
    void do_adc(std::uint8_t val) { (m_P & F_D) ? do_adc_d(val) : do_adc_nd(val); } \
    void do_sbc(std::uint8_t val) { (m_P & F_D) ? do_sbc_d(val) : do_sbc_nd(val); } \
    void do_arr() { (m_P & F_D) ? do_arr_d() : do_arr_nd(); } \
    void do_cmp(std::uint8_t lhs, std::uint8_t rhs); \
    void do_bit(std::uint8_t val); \
    std::uint8_t do_asl(std::uint8_t val); \
    std::uint8_t do_lsr(std::uint8_t val); \
    std::uint8_t do_ror(std::uint8_t val); \
    std::uint8_t do_rol(std::uint8_t val); \
    std::uint8_t do_asr(std::uint8_t val); \
    void do_exec_full(); \
    void do_exec_partial();

#define DECLARE_MAME_6502_DEVICE_CLASS_END() \
};

DECLARE_MAME_6502_DEVICE_CLASS_BEGIN(mame6502_cpu_device)
#include "m6502_nmos_declarations.inc"
DECLARE_MAME_6502_DEVICE_CLASS_END()

DECLARE_MAME_6502_DEVICE_CLASS_BEGIN(mamew65c02_cpu_device)
#include "m6502_w65c02_declarations.inc"
DECLARE_MAME_6502_DEVICE_CLASS_END()

class mamew65c02s_cpu_device : public mamew65c02_cpu_device {
public:
    explicit mamew65c02s_cpu_device(Mame6502Cpu& owner) : mamew65c02_cpu_device(owner) {}
    void execute_run();
private:
#include "m6502_w65c02s_declarations.inc"
};

class mamer65c02_cpu_device : public mamew65c02_cpu_device {
public:
    explicit mamer65c02_cpu_device(Mame6502Cpu& owner) : mamew65c02_cpu_device(owner) {}
    void execute_run();
private:
    void do_exec_full();
    void do_exec_partial();
};

#define DEFINE_MAME_6502_COMMON_METHODS(CLASS_NAME) \
void CLASS_NAME::do_adc_d(std::uint8_t val) \
{ \
    std::uint8_t c = (m_P & F_C) ? 1 : 0; \
    m_P &= ~(F_N | F_V | F_Z | F_C); \
    std::uint8_t al = (m_A & 15) + (val & 15) + c; \
    if(al > 9) al += 6; \
    std::uint8_t ah = (m_A >> 4) + (val >> 4) + (al > 15); \
    if(!std::uint8_t(m_A + val + c)) m_P |= F_Z; \
    else if(ah & 8) m_P |= F_N; \
    if(~(m_A ^ val) & (m_A ^ (ah << 4)) & 0x80) m_P |= F_V; \
    if(ah > 9) ah += 6; \
    if(ah > 15) m_P |= F_C; \
    m_A = (ah << 4) | (al & 15); \
} \
void CLASS_NAME::do_adc_nd(std::uint8_t val) \
{ \
    std::uint16_t sum = m_A + val + ((m_P & F_C) ? 1 : 0); \
    m_P &= ~(F_N | F_V | F_Z | F_C); \
    if(!std::uint8_t(sum)) m_P |= F_Z; \
    else if(static_cast<std::int8_t>(sum) < 0) m_P |= F_N; \
    if(~(m_A ^ val) & (m_A ^ sum) & 0x80) m_P |= F_V; \
    if(sum & 0xff00) m_P |= F_C; \
    m_A = sum; \
} \
void CLASS_NAME::do_sbc_d(std::uint8_t val) \
{ \
    std::uint8_t c = (m_P & F_C) ? 0 : 1; \
    m_P &= ~(F_N | F_V | F_Z | F_C); \
    std::uint16_t diff = m_A - val - c; \
    std::uint8_t al = (m_A & 15) - (val & 15) - c; \
    std::uint8_t ah = (m_A >> 4) - (val >> 4) - (static_cast<std::int8_t>(al) < 0); \
    if(!std::uint8_t(diff)) m_P |= F_Z; \
    else if(diff & 0x80) m_P |= F_N; \
    if((m_A ^ val) & (m_A ^ diff) & 0x80) m_P |= F_V; \
    if(!(diff & 0xff00)) m_P |= F_C; \
    if(static_cast<std::int8_t>(al) < 0) al -= 6; \
    if(static_cast<std::int8_t>(ah) < 0) ah -= 6; \
    m_A = (ah << 4) | (al & 15); \
} \
void CLASS_NAME::do_sbc_nd(std::uint8_t val) \
{ \
    std::uint16_t diff = m_A - val - ((m_P & F_C) ? 0 : 1); \
    m_P &= ~(F_N | F_V | F_Z | F_C); \
    if(!std::uint8_t(diff)) m_P |= F_Z; \
    else if(static_cast<std::int8_t>(diff) < 0) m_P |= F_N; \
    if((m_A ^ val) & (m_A ^ diff) & 0x80) m_P |= F_V; \
    if(!(diff & 0xff00)) m_P |= F_C; \
    m_A = diff; \
} \
void CLASS_NAME::do_sbc_cd(std::uint8_t val) \
{ \
    std::uint8_t c = (m_P & F_C) ? 0 : 1; \
    m_P &= ~(F_N | F_V | F_Z | F_C); \
    std::uint16_t diff = m_A - val - c; \
    std::uint8_t al = (m_A & 15) - (val & 15) - c; \
    std::uint8_t ah = (m_A >> 4) - (val >> 4) - (static_cast<std::int8_t>(al) < 0); \
    if((m_A ^ val) & (m_A ^ diff) & 0x80) m_P |= F_V; \
    if(!(diff & 0xff00)) m_P |= F_C; \
    m_A = (ah << 4) | (al & 15); \
    if(static_cast<std::int8_t>(al) < 0) m_A -= 6; \
    if(static_cast<std::int8_t>(ah) < 0) m_A -= 0x60; \
} \
void CLASS_NAME::do_sbc_c(std::uint8_t val) { (m_P & F_D) ? do_sbc_cd(val) : do_sbc_nd(val); } \
void CLASS_NAME::do_arr_nd() \
{ \
    bool c = m_P & F_C; \
    m_P &= ~(F_N | F_Z | F_C | F_V); \
    m_A >>= 1; \
    if(c) m_A |= 0x80; \
    if(!m_A) m_P |= F_Z; \
    else if(static_cast<std::int8_t>(m_A) < 0) m_P |= F_N; \
    if(m_A & 0x40) m_P |= F_V | F_C; \
    if(m_A & 0x20) m_P ^= F_V; \
} \
void CLASS_NAME::do_arr_d() \
{ \
    bool c = m_P & F_C; \
    m_P &= ~(F_N | F_Z | F_C | F_V); \
    std::uint8_t a = m_A >> 1; \
    if(c) a |= 0x80; \
    if(!a) m_P |= F_Z; \
    else if(static_cast<std::int8_t>(a) < 0) m_P |= F_N; \
    if((a ^ m_A) & 0x40) m_P |= F_V; \
    if((m_A & 0x0f) >= 0x05) a = ((a + 6) & 0x0f) | (a & 0xf0); \
    if((m_A & 0xf0) >= 0x50) { a += 0x60; m_P |= F_C; } \
    m_A = a; \
} \
void CLASS_NAME::do_cmp(std::uint8_t lhs, std::uint8_t rhs) \
{ \
    m_P &= ~(F_N | F_Z | F_C); \
    std::uint16_t r = lhs - rhs; \
    if(!r) m_P |= F_Z; \
    else if(static_cast<std::int8_t>(r) < 0) m_P |= F_N; \
    if(!(r & 0xff00)) m_P |= F_C; \
} \
void CLASS_NAME::do_bit(std::uint8_t val) \
{ \
    m_P &= ~(F_N | F_Z | F_V); \
    std::uint8_t r = m_A & val; \
    if(!r) m_P |= F_Z; \
    if(val & 0x80) m_P |= F_N; \
    if(val & 0x40) m_P |= F_V; \
} \
std::uint8_t CLASS_NAME::do_asl(std::uint8_t v) \
{ \
    m_P &= ~(F_N | F_Z | F_C); \
    std::uint8_t r = v << 1; \
    if(!r) m_P |= F_Z; \
    else if(static_cast<std::int8_t>(r) < 0) m_P |= F_N; \
    if(v & 0x80) m_P |= F_C; \
    return r; \
} \
std::uint8_t CLASS_NAME::do_lsr(std::uint8_t v) \
{ \
    m_P &= ~(F_N | F_Z | F_C); \
    if(v & 1) m_P |= F_C; \
    v >>= 1; \
    if(!v) m_P |= F_Z; \
    return v; \
} \
std::uint8_t CLASS_NAME::do_ror(std::uint8_t v) \
{ \
    bool c = m_P & F_C; \
    m_P &= ~(F_N | F_Z | F_C); \
    if(v & 1) m_P |= F_C; \
    v >>= 1; \
    if(c) v |= 0x80; \
    if(!v) m_P |= F_Z; \
    else if(static_cast<std::int8_t>(v) < 0) m_P |= F_N; \
    return v; \
} \
std::uint8_t CLASS_NAME::do_rol(std::uint8_t v) \
{ \
    bool c = m_P & F_C; \
    m_P &= ~(F_N | F_Z | F_C); \
    if(v & 0x80) m_P |= F_C; \
    v <<= 1; \
    if(c) v |= 0x01; \
    if(!v) m_P |= F_Z; \
    else if(static_cast<std::int8_t>(v) < 0) m_P |= F_N; \
    return v; \
} \
std::uint8_t CLASS_NAME::do_asr(std::uint8_t v) \
{ \
    m_P &= ~(F_N | F_Z | F_C); \
    if(v & 1) m_P |= F_C; \
    v >>= 1; \
    if(!v) m_P |= F_Z; \
    else if(v & 0x40) { m_P |= F_N; v |= 0x80; } \
    return v; \
} \
void CLASS_NAME::execute_run() \
{ \
    if(m_inst_substate) \
        do_exec_partial(); \
    while(m_icount > 0) { \
        if(m_inst_state < 0xff00) { \
            m_PPC = m_NPC; \
            m_inst_state = m_IR | m_inst_state_base; \
        } \
        do_exec_full(); \
    } \
}

DEFINE_MAME_6502_COMMON_METHODS(mame6502_cpu_device)
DEFINE_MAME_6502_COMMON_METHODS(mamew65c02_cpu_device)

void mamew65c02s_cpu_device::execute_run()
{
    if(m_inst_substate)
        do_exec_partial();
    while(m_icount > 0) {
        if(m_inst_state < 0xff00) {
            m_PPC = m_NPC;
            m_inst_state = m_IR | m_inst_state_base;
        }
        do_exec_full();
    }
}

void mamer65c02_cpu_device::execute_run()
{
    if(m_inst_substate)
        do_exec_partial();
    while(m_icount > 0) {
        if(m_inst_state < 0xff00) {
            m_PPC = m_NPC;
            m_inst_state = m_IR | m_inst_state_base;
        }
        do_exec_full();
    }
}


#include "m6502_nmos_generated.hxx"
#include "m6502_nmos_base_for_mamew65c02_generated.hxx"
#include "m6502_w65c02_generated.hxx"
#include "m6502_r65c02_dispatch_generated.hxx"
#include "m6502_w65c02s_generated.hxx"

Mame6502Cpu::Mame6502Cpu(CpuMode mode) :
    mode_(mode)
{
    clear_memory();
    reset_from_vector();
}

void Mame6502Cpu::set_mode(CpuMode mode)
{
    if(mode_ == mode)
        return;
    mode_ = mode;
    inst_state_base_ = 0;
    set_pc_and_prefetch(pc_);
}

void Mame6502Cpu::clear_memory(std::uint8_t value)
{
    memory_.fill(value);
    read_count_ = write_count_ = sync_read_count_ = 0;
}

void Mame6502Cpu::load_program(std::uint16_t address, const std::uint8_t* data, std::size_t size)
{
    std::copy(data, data + size, memory_.begin() + address);
}

void Mame6502Cpu::set_vector(std::uint16_t vector_address, std::uint16_t target)
{
    memory_[vector_address] = static_cast<std::uint8_t>(target & 0xffu);
    memory_[static_cast<std::uint16_t>(vector_address + 1)] = static_cast<std::uint8_t>(target >> 8);
}

void Mame6502Cpu::reset_from_vector()
{
    ppc_ = npc_ = pc_ = 0;
    sp_ = 0x0100;
    a_ = 0;
    x_ = 0x80;
    y_ = 0;
    p_ = 0x36;
    tmp_ = 0;
    tmp2_ = 0;
    ir_ = 0;
    inst_state_base_ = 0;
    inst_state_ = STATE_RESET;
    inst_substate_ = 0;
    icount_ = 0;
    nmi_state_ = false;
    irq_state_ = false;
    v_state_ = false;
    nmi_pending_ = false;
    irq_taken_ = false;
    sync_ = false;
    inhibit_interrupts_ = false;
    jammed_ = false;

    icount_ = 7;
    switch(mode_) {
    case CpuMode::nmos6502: { mame6502_cpu_device device(*this); device.execute_run(); break; }
    case CpuMode::wdc65c02: { mamew65c02_cpu_device device(*this); device.execute_run(); break; }
    case CpuMode::rockwell65c02: { mamer65c02_cpu_device device(*this); device.execute_run(); break; }
    case CpuMode::wdc65c02s: { mamew65c02s_cpu_device device(*this); device.execute_run(); break; }
    }
    total_cycles_ = 0;
}

void Mame6502Cpu::set_pc_and_prefetch(std::uint16_t pc)
{
    pc_ = pc;
    npc_ = pc;
    ppc_ = pc;
    sync_ = true;
    ir_ = read_sync(pc_);
    sync_ = false;
    pc_++;
    inst_state_ = ir_ | inst_state_base_;
    inst_substate_ = 0;
    irq_taken_ = false;
}

unsigned Mame6502Cpu::execute(unsigned cycles)
{
    if(jammed_ || cycles == 0)
        return 0;

    const int before = static_cast<int>(cycles);
    icount_ = before;
    switch(mode_) {
    case CpuMode::nmos6502: { mame6502_cpu_device device(*this); device.execute_run(); break; }
    case CpuMode::wdc65c02: { mamew65c02_cpu_device device(*this); device.execute_run(); break; }
    case CpuMode::rockwell65c02: { mamer65c02_cpu_device device(*this); device.execute_run(); break; }
    case CpuMode::wdc65c02s: { mamew65c02s_cpu_device device(*this); device.execute_run(); break; }
    }
    const unsigned executed = static_cast<unsigned>(before - icount_);
    total_cycles_ += executed;
    return executed;
}

CpuState Mame6502Cpu::state() const
{
    return CpuState{a_, x_, y_, p_, static_cast<std::uint8_t>(sp_), npc_, jammed_};
}

void Mame6502Cpu::set_state(const CpuState& state)
{
    a_ = state.a;
    x_ = state.x;
    y_ = state.y;
    p_ = state.p;
    sp_ = static_cast<std::uint16_t>(0x0100u | state.sp);
    jammed_ = state.jammed;
    set_pc_and_prefetch(state.pc);
}

void Mame6502Cpu::set_nmi_line(bool asserted)
{
    if(!nmi_state_ && asserted && total_cycles_)
        nmi_pending_ = true;
    nmi_state_ = asserted;
}

void Mame6502Cpu::set_so_line(bool asserted)
{
    if(!v_state_ && asserted && total_cycles_)
        p_ |= F_V;
    v_state_ = asserted;
}

std::uint8_t Mame6502Cpu::read(std::uint16_t address)
{
    ++read_count_;
    return memory_[address];
}

std::uint8_t Mame6502Cpu::read_sync(std::uint16_t address)
{
    ++sync_read_count_;
    ++read_count_;
    if(sync_watch_enabled_ && address == sync_watch_address_) {
        sync_watch_seen_ = true;
    }
    return memory_[address];
}

std::uint8_t Mame6502Cpu::read_arg(std::uint16_t address)
{
    ++read_count_;
    return memory_[address];
}

void Mame6502Cpu::write(std::uint16_t address, std::uint8_t value)
{
    ++write_count_;
    memory_[address] = value;
}

} // namespace benchmark6502::mame
