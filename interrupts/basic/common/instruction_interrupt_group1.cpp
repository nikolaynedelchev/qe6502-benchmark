#include "instruction_interrupt_group1.hpp"

#include <asm6502/asm6502.h>

#include <cstdint>
#include <cstdio>
#include <exception>
#include <fstream>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace benchmark6502::interrupts::basic {
namespace {

constexpr std::uint8_t flag_c = 0x01u;
constexpr std::uint8_t flag_i = 0x04u;
constexpr std::uint8_t flag_d = 0x08u;
constexpr std::uint8_t flag_b = 0x10u;
constexpr std::uint8_t flag_u = 0x20u;

constexpr std::uint16_t main_addr = 0x8000u;
constexpr std::uint16_t nmi_addr = 0x9a37u;
constexpr std::uint16_t irq_addr = 0x8c51u;
constexpr std::uint16_t reset_alt_addr = 0x8442u;

constexpr std::uint16_t marker = 0x0200u;
constexpr std::uint16_t irq_count = 0x0201u;
constexpr std::uint16_t nmi_count = 0x0202u;
constexpr std::uint16_t brk_count = 0x0203u;
constexpr std::uint16_t live_p = 0x0204u;
constexpr std::uint16_t pushed_p = 0x0205u;
constexpr std::uint16_t pushed_pcl = 0x0206u;
constexpr std::uint16_t pushed_pch = 0x0207u;
constexpr std::uint16_t marker2 = 0x0208u;

class TestFailure : public std::runtime_error {
public:
    TestFailure(std::string_view test, std::string_view message)
        : std::runtime_error(std::string(message))
        , test_(test)
    {
    }

    std::string_view test() const noexcept { return test_; }

private:
    std::string test_;
};

[[noreturn]] void fail(std::string_view test, std::string_view message)
{
    throw TestFailure(test, message);
}

void require(bool condition, std::string_view test, std::string_view message)
{
    if (!condition) {
        fail(test, message);
    }
}

std::unique_ptr<IInstructionCpu> make_loaded_cpu(
    const InstructionCpuFactory& factory,
    const std::vector<asm6502::mem_value>& program,
    std::uint8_t fill = 0xeau)
{
    auto cpu = factory();
    cpu->clear_memory(fill);
    cpu->load(program);
    cpu->reset();
    return cpu;
}

void step(IInstructionCpu& cpu, unsigned count = 1u)
{
    for (unsigned i = 0; i < count; ++i) {
        cpu.step_instruction();
    }
}

void run_to_marker(IInstructionCpu& cpu, std::uint16_t address, std::uint8_t expected, unsigned max_steps);

void trigger_nmi_until(IInstructionCpu& cpu, std::uint16_t address, std::uint8_t expected, unsigned max_steps)
{
    cpu.assert_nmi_for(1);
    run_to_marker(cpu, address, expected, max_steps);
}

asm6502::Asm6502 base_program()
{
    return asm6502::Asm6502::New()
        .begin()
        .nmi_vector("nmi_handler")
        .reset_vector("main")
        .brk_irq_vector("irq_handler");
}

asm6502::Asm6502& record_interrupt_frame(asm6502::Asm6502& p)
{
    return p
        .tsx()
        .lda(asm6502::abx, 0x0101u)
        .sta(asm6502::absolute, pushed_p)
        .lda(asm6502::abx, 0x0102u)
        .sta(asm6502::absolute, pushed_pcl)
        .lda(asm6502::abx, 0x0103u)
        .sta(asm6502::absolute, pushed_pch)
        .php()
        .pla()
        .sta(asm6502::absolute, live_p);
}

std::vector<asm6502::mem_value> compile_simple_loop()
{
    return base_program()
        .org(main_addr, "main")
            .nop()
            .jmp("main")
        .org(nmi_addr, "nmi_handler")
            .lda(0x11u)
            .sta(asm6502::absolute, marker)
            .rti()
        .org(irq_addr, "irq_handler")
            .lda(0x22u)
            .sta(asm6502::absolute, marker)
            .rti()
        .end()
        .compile();
}

std::vector<asm6502::mem_value> compile_interrupt_frame_program(bool brk_main = false)
{
    auto p = base_program();
    p.org(main_addr, "main");
    if (brk_main) {
        p.brk().db(0x00u).lda(0x44u).sta(asm6502::absolute, marker2).jmp("done");
    } else {
        p.nop().lda(0x44u).sta(asm6502::absolute, marker2).jmp("done");
    }
    p.org(0x8020u, "done").jmp("done");

    p.org(nmi_addr, "nmi_handler");
    record_interrupt_frame(p)
        .lda(0x11u)
        .sta(asm6502::absolute, nmi_count)
        .rti();

    p.org(irq_addr, "irq_handler");
    record_interrupt_frame(p)
        .lda(brk_main ? 0x33u : 0x22u)
        .sta(asm6502::absolute, brk_main ? brk_count : irq_count)
        .rti();

    return p.end().compile();
}

void run_to_marker(IInstructionCpu& cpu, std::uint16_t address, std::uint8_t expected, unsigned max_steps)
{
    for (unsigned i = 0; i < max_steps; ++i) {
        if (cpu.read(address) == expected) {
            return;
        }
        cpu.step_instruction();
    }
}

void test_reset_vector(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "reset vector routing";
    const auto program = asm6502::Asm6502::New()
        .begin()
        .reset_vector("main")
        .org(reset_alt_addr, "main")
            .nop()
            .jmp("main")
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    require(cpu->state().pc == reset_alt_addr, name, "PC did not use reset vector");
}

void test_i_after_reset(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "I flag after reset";
    auto cpu = make_loaded_cpu(factory, compile_simple_loop());
    require((cpu->state().p & flag_i) != 0u, name, "I flag is not set after reset");
}

void test_irq_masked_after_reset(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "IRQ masked after reset";
    auto cpu = make_loaded_cpu(factory, compile_simple_loop());
    cpu->assert_irq_for(4);
    step(*cpu, 4u);
    require(cpu->read(marker) != 0x22u, name, "IRQ handler ran while I was set after reset");
}

void test_nmi_after_reset(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "NMI accepted after reset";
    auto cpu = make_loaded_cpu(factory, compile_simple_loop());
    trigger_nmi_until(*cpu, marker, 0x11u, 8u);
    require(cpu->read(marker) == 0x11u, name, "NMI handler did not run");
}

void require_state_unchanged(const instruction_cpu_state& before,
                             const instruction_cpu_state& after,
                             std::string_view name,
                             std::string_view signal_name)
{
    require(after.pc == before.pc, name, std::string(signal_name) + " assertion changed PC synchronously");
    require(after.a == before.a, name, std::string(signal_name) + " assertion changed A synchronously");
    require(after.x == before.x, name, std::string(signal_name) + " assertion changed X synchronously");
    require(after.y == before.y, name, std::string(signal_name) + " assertion changed Y synchronously");
    require(after.s == before.s, name, std::string(signal_name) + " assertion changed S synchronously");
    require(after.p == before.p, name, std::string(signal_name) + " assertion changed P synchronously");
}

void test_assert_nmi_for_not_synchronous(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "assert_nmi_for is not synchronous";
    auto cpu = make_loaded_cpu(factory, compile_simple_loop());
    const auto before_state = cpu->state();
    const auto before_marker = cpu->read(marker);

    cpu->assert_nmi_for(1);

    require_state_unchanged(before_state, cpu->state(), name, "NMI");
    require(cpu->read(marker) == before_marker, name, "NMI handler changed memory before step_instruction");

    run_to_marker(*cpu, marker, 0x11u, 8u);
    require(cpu->read(marker) == 0x11u, name, "deferred NMI did not run after step_instruction");
}

void test_assert_irq_for_not_synchronous(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "assert_irq_for is not synchronous";
    const auto program = base_program()
        .org(main_addr, "main")
            .cli()
            .nop()
            .jmp("main")
        .org(irq_addr, "irq_handler")
            .lda(0x22u)
            .sta(asm6502::absolute, marker)
            .rti()
        .org(nmi_addr, "nmi_handler")
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu); // CLI complete before IRQ assertion.
    const auto before_state = cpu->state();
    const auto before_marker = cpu->read(marker);

    cpu->assert_irq_for(1);

    require_state_unchanged(before_state, cpu->state(), name, "IRQ");
    require(cpu->read(marker) == before_marker, name, "IRQ handler changed memory before step_instruction");

    run_to_marker(*cpu, marker, 0x22u, 8u);
    require(cpu->read(marker) == 0x22u, name, "deferred IRQ did not run after step_instruction");
}

void test_irq_masked_by_sei(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "IRQ masked when I=1";
    const auto program = base_program()
        .org(main_addr, "main")
            .sei()
            .nop()
            .nop()
            .lda(0x44u)
            .sta(asm6502::absolute, marker2)
            .jmp("main")
        .org(irq_addr, "irq_handler")
            .lda(0x22u)
            .sta(asm6502::absolute, marker)
            .rti()
        .org(nmi_addr, "nmi_handler")
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu); // SEI
    cpu->assert_irq_for(5);
    step(*cpu, 5u);
    require(cpu->read(marker) != 0x22u, name, "IRQ handler ran while I was set");
    require(cpu->read(marker2) == 0x44u, name, "main code did not continue while IRQ was masked");
}

void test_irq_accepted_when_clear(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "IRQ accepted when I=0";
    const auto program = base_program()
        .org(main_addr, "main")
            .cli()
            .nop()
            .jmp("main")
        .org(irq_addr, "irq_handler")
            .lda(0x22u)
            .sta(asm6502::absolute, marker)
            .rti()
        .org(nmi_addr, "nmi_handler")
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu); // CLI is complete before IRQ assertion.
    cpu->assert_irq_for(1);
    run_to_marker(*cpu, marker, 0x22u, 8u);
    require(cpu->read(marker) == 0x22u, name, "IRQ handler did not run with I clear");
}

void test_irq_deasserted_before_unmask(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "IRQ deasserted before unmask";
    const auto program = base_program()
        .org(main_addr, "main")
            .sei()
            .nop()
            .cli()
            .lda(0x44u)
            .sta(asm6502::absolute, marker2)
            .jmp("main")
        .org(irq_addr, "irq_handler")
            .lda(0x22u)
            .sta(asm6502::absolute, marker)
            .rti()
        .org(nmi_addr, "nmi_handler")
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu); // SEI
    cpu->assert_irq_for(1);
    step(*cpu); // NOP while masked, then scheduled IRQ deasserts before CLI.
    step(*cpu, 5u);
    require(cpu->read(marker) != 0x22u, name, "IRQ handler ran after IRQ was deasserted before CLI");
    require(cpu->read(marker2) == 0x44u, name, "main code did not run after CLI");
}

void test_nmi_ignores_i(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "NMI ignores I";
    const auto program = base_program()
        .org(main_addr, "main")
            .sei()
            .nop()
            .jmp("main")
        .org(nmi_addr, "nmi_handler")
            .lda(0x11u)
            .sta(asm6502::absolute, marker)
            .rti()
        .org(irq_addr, "irq_handler")
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu); // SEI
    trigger_nmi_until(*cpu, marker, 0x11u, 8u);
    require(cpu->read(marker) == 0x11u, name, "NMI did not run with I set");
}

void test_rti_resumes_nmi_irq(const InstructionCpuFactory& factory)
{
    {
        constexpr std::string_view name = "NMI RTI resumes";
        auto cpu = make_loaded_cpu(factory, compile_interrupt_frame_program(false));
        const auto old_s = cpu->state().s;
        trigger_nmi_until(*cpu, nmi_count, 0x11u, 12u);
        run_to_marker(*cpu, marker2, 0x44u, 16u);
        require(cpu->read(nmi_count) == 0x11u, name, "NMI handler did not run");
        require(cpu->read(marker2) == 0x44u, name, "main did not resume after NMI RTI");
        require(cpu->state().s == old_s, name, "NMI RTI did not restore S");
    }
    {
        constexpr std::string_view name = "IRQ RTI resumes";
        auto cpu = make_loaded_cpu(factory, compile_interrupt_frame_program(false));
        step(*cpu); // NOP after reset.
        cpu->write(marker2, 0u);
        cpu->write(irq_count, 0u);
        // Need I clear for IRQ; use direct P restore test elsewhere, so run a CLI program here.
        const auto program = base_program()
            .org(main_addr, "main")
                .cli()
                .nop()
                .lda(0x44u)
                .sta(asm6502::absolute, marker2)
                .jmp("done")
            .org(0x8020u, "done")
                .jmp("done")
            .org(irq_addr, "irq_handler")
                .lda(0x22u)
                .sta(asm6502::absolute, irq_count)
                .rti()
            .org(nmi_addr, "nmi_handler")
                .rti()
            .end()
            .compile();
        cpu = make_loaded_cpu(factory, program);
        step(*cpu); // CLI complete before IRQ assertion.
        const auto old_s = cpu->state().s;
        cpu->assert_irq_for(1);
        run_to_marker(*cpu, irq_count, 0x22u, 12u);
        run_to_marker(*cpu, marker2, 0x44u, 16u);
        require(cpu->read(irq_count) == 0x22u, name, "IRQ handler did not run");
        require(cpu->read(marker2) == 0x44u, name, "main did not resume after IRQ RTI");
        require(cpu->state().s == old_s, name, "IRQ RTI did not restore S");
    }
}

void test_brk_vector_and_rti(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "BRK vector and RTI";
    auto cpu = make_loaded_cpu(factory, compile_interrupt_frame_program(true));
    const auto old_s = cpu->state().s;
    run_to_marker(*cpu, marker2, 0x44u, 16u);
    require(cpu->read(brk_count) == 0x33u, name, "BRK handler did not run");
    require(cpu->read(marker2) == 0x44u, name, "BRK RTI did not resume at PC+2");
    require(cpu->state().s == old_s, name, "BRK RTI did not restore S");
    require(cpu->read(pushed_pcl) == 0x02u, name, "BRK did not push low byte of PC+2");
    require(cpu->read(pushed_pch) == 0x80u, name, "BRK did not push high byte of PC+2");
}

void test_pushed_b_bits_and_live_i(const InstructionCpuFactory& factory)
{
    {
        constexpr std::string_view name = "NMI pushed B=0 and live I=1";
        auto cpu = make_loaded_cpu(factory, compile_interrupt_frame_program(false));
        trigger_nmi_until(*cpu, nmi_count, 0x11u, 12u);
        require((cpu->read(pushed_p) & flag_b) == 0u, name, "NMI pushed B set");
        require((cpu->read(live_p) & flag_i) != 0u, name, "NMI handler live I clear");
    }
    {
        constexpr std::string_view name = "IRQ pushed B=0 and live I=1";
        const auto program = base_program()
            .org(main_addr, "main")
                .cli()
                .nop()
                .jmp("main")
            .org(irq_addr, "irq_handler")
                .tsx()
                .lda(asm6502::abx, 0x0101u)
                .sta(asm6502::absolute, pushed_p)
                .php()
                .pla()
                .sta(asm6502::absolute, live_p)
                .lda(0x22u)
                .sta(asm6502::absolute, irq_count)
                .rti()
            .org(nmi_addr, "nmi_handler")
                .rti()
            .end()
            .compile();
        auto cpu = make_loaded_cpu(factory, program);
        step(*cpu); // CLI complete.
        cpu->assert_irq_for(1);
        run_to_marker(*cpu, irq_count, 0x22u, 12u);
        require((cpu->read(pushed_p) & flag_b) == 0u, name, "IRQ pushed B set");
        require((cpu->read(live_p) & flag_i) != 0u, name, "IRQ handler live I clear");
    }
    {
        constexpr std::string_view name = "BRK pushed B=1 and live I=1";
        auto cpu = make_loaded_cpu(factory, compile_interrupt_frame_program(true));
        run_to_marker(*cpu, brk_count, 0x33u, 12u);
        require((cpu->read(pushed_p) & flag_b) != 0u, name, "BRK pushed B clear");
        require((cpu->read(live_p) & flag_i) != 0u, name, "BRK handler live I clear");
    }
}

void test_plp_changes_irq_mask(const InstructionCpuFactory& factory)
{
    {
        constexpr std::string_view name = "PLP sets I and masks IRQ";
        const auto program = base_program()
            .org(main_addr, "main")
                .lda(flag_u | flag_i)
                .pha()
                .plp()
                .nop()
                .lda(0x44u)
                .sta(asm6502::absolute, marker2)
                .jmp("main")
            .org(irq_addr, "irq_handler")
                .lda(0x22u)
                .sta(asm6502::absolute, irq_count)
                .rti()
            .org(nmi_addr, "nmi_handler")
                .rti()
            .end()
            .compile();
        auto cpu = make_loaded_cpu(factory, program);
        step(*cpu, 2u); // LDA, PHA
        cpu->assert_irq_for(6);
        step(*cpu, 6u); // PLP should set I and mask held IRQ.
        require(cpu->read(irq_count) != 0x22u, name, "IRQ ran after PLP set I");
        require(cpu->read(marker2) == 0x44u, name, "main did not continue after PLP set I");
    }
    {
        constexpr std::string_view name = "PLP clears I and enables IRQ";
        const auto program = base_program()
            .org(main_addr, "main")
                .lda(flag_u)
                .pha()
                .plp()
                .nop()
                .jmp("main")
            .org(irq_addr, "irq_handler")
                .lda(0x22u)
                .sta(asm6502::absolute, irq_count)
                .rti()
            .org(nmi_addr, "nmi_handler")
                .rti()
            .end()
            .compile();
        auto cpu = make_loaded_cpu(factory, program);
        step(*cpu, 2u); // LDA, PHA
        cpu->assert_irq_for(4);
        run_to_marker(*cpu, irq_count, 0x22u, 12u);
        require(cpu->read(irq_count) == 0x22u, name, "IRQ did not run after PLP cleared I");
    }
}

void test_rti_restores_pc_p(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "RTI restores PC and P";
    const auto program = asm6502::Asm6502::New()
        .begin()
        .reset_vector("main")
        .org(main_addr, "main")
            .ldx(0xfau)
            .txs()
            .lda(flag_u | flag_c | flag_d) // P with I clear.
            .sta(asm6502::absolute, 0x01fbu)
            .lda(0x34u)
            .sta(asm6502::absolute, 0x01fcu)
            .lda(0x12u)
            .sta(asm6502::absolute, 0x01fdu)
            .rti()
        .org(0x1234u)
            .lda(0x55u)
            .sta(asm6502::absolute, marker)
            .jmp(0x1234u)
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    run_to_marker(*cpu, marker, 0x55u, 16u);
    require(cpu->read(marker) == 0x55u, name, "RTI did not resume at synthetic PC");
    require((cpu->state().p & flag_c) != 0u, name, "RTI did not restore C");
    require((cpu->state().p & flag_d) != 0u, name, "RTI did not restore D");
    require((cpu->state().p & flag_i) == 0u, name, "RTI did not restore I clear");
}

void test_distinct_vectors_and_endian(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "distinct vectors and endian";
    const auto program = asm6502::Asm6502::New()
        .begin()
        .nmi_vector(nmi_addr)
        .reset_vector(reset_alt_addr)
        .brk_irq_vector(irq_addr)
        .org(reset_alt_addr, "main")
            .cli()
            .nop()
            .jmp("main")
        .org(nmi_addr)
            .lda(0x11u)
            .sta(asm6502::absolute, nmi_count)
            .rti()
        .org(irq_addr)
            .lda(0x22u)
            .sta(asm6502::absolute, irq_count)
            .rti()
        .end()
        .compile();

    auto cpu = make_loaded_cpu(factory, program);
    require(cpu->state().pc == reset_alt_addr, name, "reset vector wrong");
    trigger_nmi_until(*cpu, nmi_count, 0x11u, 8u);
    require(cpu->read(nmi_count) == 0x11u, name, "NMI vector wrong");

    cpu = make_loaded_cpu(factory, program);
    step(*cpu); // CLI
    cpu->assert_irq_for(1);
    run_to_marker(*cpu, irq_count, 0x22u, 8u);
    require(cpu->read(irq_count) == 0x22u, name, "IRQ vector wrong");
}

void test_non_default_sp_and_stack_pointer(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "non-default SP interrupt stack";
    const auto program = base_program()
        .org(main_addr, "main")
            .ldx(0x80u)
            .txs()
            .cli()
            .nop()
            .jmp("main")
        .org(irq_addr, "irq_handler")
            .tsx()
            .stx(asm6502::absolute, marker)
            .lda(asm6502::absolute, 0x017eu) // P pushed at old S - 2 = $017E when old S=$80.
            .sta(asm6502::absolute, pushed_p)
            .rti()
        .org(nmi_addr, "nmi_handler")
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu, 3u); // LDX, TXS, CLI.
    cpu->assert_irq_for(1);
    run_to_marker(*cpu, marker, 0x7du, 12u);
    require(cpu->read(marker) == 0x7du, name, "SP was not decremented by 3 on interrupt entry");
    require((cpu->read(pushed_p) & flag_b) == 0u, name, "pushed P not found at custom stack location");
}

void test_php_vs_interrupt_b(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "PHP vs interrupt B flag";
    const auto program = base_program()
        .org(main_addr, "main")
            .php()
            .pla()
            .sta(asm6502::absolute, marker)
            .cli()
            .nop()
            .jmp("main")
        .org(irq_addr, "irq_handler")
            .tsx()
            .lda(asm6502::abx, 0x0101u)
            .sta(asm6502::absolute, pushed_p)
            .rti()
        .org(nmi_addr, "nmi_handler")
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu, 4u); // PHP, PLA, STA, CLI.
    require((cpu->read(marker) & flag_b) != 0u, name, "PHP stack representation did not set B");
    cpu->assert_irq_for(1);
    for (unsigned i = 0; i < 12u && cpu->read(pushed_p) == 0u; ++i) {
        cpu->step_instruction();
    }
    require(cpu->read(pushed_p) != 0u, name, "IRQ did not record pushed P");
    require((cpu->read(pushed_p) & flag_b) == 0u, name, "IRQ stack representation set B");
}

void test_decimal_flag_preserved_nmos(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "decimal flag preserved on NMOS interrupt";
    const auto program = base_program()
        .org(main_addr, "main")
            .sed()
            .cli()
            .nop()
            .jmp("main")
        .org(irq_addr, "irq_handler")
            .tsx()
            .lda(asm6502::abx, 0x0101u)
            .sta(asm6502::absolute, pushed_p)
            .php()
            .pla()
            .sta(asm6502::absolute, live_p)
            .rti()
        .org(nmi_addr, "nmi_handler")
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu, 2u); // SED, CLI.
    cpu->assert_irq_for(1);
    for (unsigned i = 0; i < 12u && cpu->read(pushed_p) == 0u; ++i) {
        cpu->step_instruction();
    }
    require((cpu->read(pushed_p) & flag_d) != 0u, name, "pushed P lost D flag");
    require((cpu->read(live_p) & flag_d) != 0u, name, "live P lost D flag in handler");
}

using TestFn = void (*)(const InstructionCpuFactory&);

struct TestCase {
    std::string_view name;
    TestFn fn;
};

std::string sanitize_filename_part(std::string_view text)
{
    std::string out;
    out.reserve(text.size());
    for (char ch : text) {
        if ((ch >= '0' && ch <= '9') ||
            (ch >= 'A' && ch <= 'Z') ||
            (ch >= 'a' && ch <= 'z')) {
            out.push_back(ch);
        } else {
            out.push_back('_');
        }
    }
    return out.empty() ? std::string("unknown") : out;
}

std::string tsv_cell(std::string_view text)
{
    std::string out;
    out.reserve(text.size());
    for (char ch : text) {
        if (ch == '\t' || ch == '\r' || ch == '\n') {
            out.push_back(' ');
        } else {
            out.push_back(ch);
        }
    }
    return out;
}

} // namespace

int run_instruction_interrupt_group1(const InstructionCpuFactory& factory)
{
    const TestCase tests[] = {
        {"reset vector routing", test_reset_vector},
        {"I flag after reset", test_i_after_reset},
        {"IRQ masked after reset", test_irq_masked_after_reset},
        {"NMI accepted after reset", test_nmi_after_reset},
        {"assert_nmi_for is not synchronous", test_assert_nmi_for_not_synchronous},
        {"assert_irq_for is not synchronous", test_assert_irq_for_not_synchronous},
        {"IRQ masked when I=1", test_irq_masked_by_sei},
        {"IRQ accepted when I=0", test_irq_accepted_when_clear},
        {"IRQ deasserted before unmask", test_irq_deasserted_before_unmask},
        {"NMI ignores I", test_nmi_ignores_i},
        {"NMI/IRQ RTI resumes", test_rti_resumes_nmi_irq},
        {"BRK vector and RTI", test_brk_vector_and_rti},
        {"pushed B bits and live I", test_pushed_b_bits_and_live_i},
        {"PLP changes IRQ mask", test_plp_changes_irq_mask},
        {"RTI restores PC and P", test_rti_restores_pc_p},
        {"distinct vectors and endian", test_distinct_vectors_and_endian},
        {"non-default SP interrupt stack", test_non_default_sp_and_stack_pointer},
        {"PHP vs interrupt B flag", test_php_vs_interrupt_b},
        {"decimal flag preserved", test_decimal_flag_preserved_nmos},
    };

    const auto probe = factory();
    const std::string core_name(probe->core_name());
    const std::string model_name(probe->model_name());
    const std::string log_path = "interrupt_basic_" + sanitize_filename_part(core_name) +
        "_" + sanitize_filename_part(model_name) + "_group1.tsv";

    std::ofstream log(log_path);
    if (!log) {
        std::fprintf(stderr, "failed to open %s for writing\n", log_path.c_str());
        return 1;
    }
    log << "suite\tcore\tmodel\ttest\tstatus\tdetail\n";

    unsigned passed = 0u;
    unsigned failed = 0u;
    for (const auto& test : tests) {
        try {
            test.fn(factory);
            ++passed;
            std::printf("PASS %-42.*s\n", static_cast<int>(test.name.size()), test.name.data());
            log << "group1\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                << tsv_cell(test.name) << "\tpass\t\n";
        } catch (const TestFailure& failure) {
            ++failed;
            std::fprintf(stderr, "FAIL %-42.*s %s\n",
                static_cast<int>(failure.test().size()), failure.test().data(), failure.what());
            log << "group1\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                << tsv_cell(failure.test()) << "\tfail\t" << tsv_cell(failure.what()) << "\n";
        } catch (const std::exception& e) {
            ++failed;
            std::fprintf(stderr, "FAIL %-42.*s %s\n",
                static_cast<int>(test.name.size()), test.name.data(), e.what());
            log << "group1\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                << tsv_cell(test.name) << "\tfail\t" << tsv_cell(e.what()) << "\n";
        }
    }

    std::printf("instruction interrupt group1: %u/%zu passed\n", passed, sizeof(tests) / sizeof(tests[0]));
    std::printf("instruction interrupt group1 log: %s\n", log_path.c_str());
    return failed == 0u ? 0 : 1;
}

} // namespace benchmark6502::interrupts::basic
