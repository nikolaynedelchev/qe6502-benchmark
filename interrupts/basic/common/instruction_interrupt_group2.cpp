#include "instruction_interrupt_group2.hpp"

#include <asm6502/asm6502.h>

#include <cstdint>
#include <cstdio>
#include <exception>
#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace benchmark6502::interrupts::basic {
namespace {

constexpr std::uint8_t flag_i = 0x04u;
constexpr std::uint8_t flag_b = 0x10u;
constexpr std::uint8_t flag_u = 0x20u;

constexpr std::uint16_t main_addr = 0x8000u;
constexpr std::uint16_t nmi_addr = 0x9a37u;
constexpr std::uint16_t irq_addr = 0x8c51u;
constexpr std::uint16_t nmi_alt_addr = 0x9345u;
constexpr std::uint16_t irq_alt_addr = 0x8a23u;

constexpr std::uint16_t marker = 0x0200u;
constexpr std::uint16_t irq_count = 0x0201u;
constexpr std::uint16_t nmi_count = 0x0202u;
constexpr std::uint16_t marker2 = 0x0203u;
constexpr std::uint16_t pushed_p = 0x0204u;

class TestFailure : public std::runtime_error {
public:
    TestFailure(std::string_view test, std::string_view message)
        : std::runtime_error(std::string(message)), test_(test) {}
    std::string_view test() const noexcept { return test_; }
private:
    std::string test_;
};

[[noreturn]] void fail(std::string_view test, std::string_view message) { throw TestFailure(test, message); }
void require(bool condition, std::string_view test, std::string_view message) { if (!condition) fail(test, message); }

std::string sanitize_filename_part(std::string_view text)
{
    std::string out;
    out.reserve(text.size());
    for (char ch : text) {
        if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) {
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
        out.push_back((ch == '\t' || ch == '\r' || ch == '\n') ? ' ' : ch);
    }
    return out;
}

asm6502::Asm6502 base_program()
{
    return asm6502::Asm6502::New()
        .begin()
        .nmi_vector("nmi_handler")
        .reset_vector("main")
        .brk_irq_vector("irq_handler");
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
    cpu->write(marker, 0u);
    cpu->write(irq_count, 0u);
    cpu->write(nmi_count, 0u);
    cpu->write(marker2, 0u);
    cpu->write(pushed_p, 0u);
    return cpu;
}

void step(IInstructionCpu& cpu, unsigned count = 1u)
{
    for (unsigned i = 0; i < count; ++i) {
        cpu.step_instruction();
    }
}

bool step_until(IInstructionCpu& cpu, std::uint16_t address, std::uint8_t expected, unsigned max_steps)
{
    for (unsigned i = 0; i < max_steps; ++i) {
        if (cpu.read(address) == expected) {
            return true;
        }
        cpu.step_instruction();
    }
    return cpu.read(address) == expected;
}

void require_marker(IInstructionCpu& cpu, std::uint16_t address, std::uint8_t expected, unsigned max_steps, std::string_view name, std::string_view message)
{
    require(step_until(cpu, address, expected, max_steps), name, message);
}

std::vector<asm6502::mem_value> compile_standard_handlers(bool irq_loop = false, bool nmi_loop = false)
{
    auto p = base_program();
    p.org(main_addr, "main")
        .nop()
        .jmp("main");

    p.org(nmi_addr, "nmi_handler")
        .inc(asm6502::absolute, nmi_count);
    if (nmi_loop) {
        p.jmp("nmi_handler");
    } else {
        p.rti();
    }

    p.org(irq_addr, "irq_handler")
        .inc(asm6502::absolute, irq_count);
    if (irq_loop) {
        p.jmp("irq_handler");
    } else {
        p.rti();
    }

    return p.end().compile();
}

void test_irq_held_masked_then_cli_accepts(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "IRQ held masked then CLI accepts";
    const auto program = base_program()
        .org(main_addr, "main")
            .sei()
            .nop()
            .cli()
            .lda(0x44u)
            .sta(asm6502::absolute, marker2)
            .jmp("main")
        .org(irq_addr, "irq_handler")
            .inc(asm6502::absolute, irq_count)
            .rti()
        .org(nmi_addr, "nmi_handler")
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu); // SEI
    cpu->assert_irq_for(8);
    step(*cpu); // NOP while masked.
    require(cpu->read(irq_count) == 0u, name, "IRQ ran while masked before CLI");
    step(*cpu); // CLI.
    require_marker(*cpu, irq_count, 1u, 8u, name, "held IRQ was not accepted after CLI");
}

void test_sei_masks_held_irq(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "SEI masks held IRQ";
    const auto program = base_program()
        .org(main_addr, "main")
            .sei()
            .nop()
            .lda(0x44u)
            .sta(asm6502::absolute, marker2)
            .jmp("main")
        .org(irq_addr, "irq_handler")
            .inc(asm6502::absolute, irq_count)
            .rti()
        .org(nmi_addr, "nmi_handler")
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    cpu->assert_irq_for(5);
    step(*cpu, 5u);
    require(cpu->read(irq_count) == 0u, name, "IRQ ran while held and masked by SEI/reset I");
    require(cpu->read(marker2) == 0x44u, name, "main did not continue under masked IRQ");
}

std::vector<asm6502::mem_value> compile_rti_restore_program()
{
    return base_program()
        .org(main_addr, "main")
            .rti()
        .org(0x8040u, "after_rti")
            .lda(0x44u)
            .sta(asm6502::absolute, marker2)
            .jmp("after_rti")
        .org(irq_addr, "irq_handler")
            .inc(asm6502::absolute, irq_count)
            .rti()
        .org(nmi_addr, "nmi_handler")
            .rti()
        .end()
        .compile();
}

void seed_rti_frame(IInstructionCpu& cpu, std::uint8_t p)
{
    const std::uint8_t s = cpu.state().s;
    cpu.write(static_cast<std::uint16_t>(0x0100u + static_cast<std::uint8_t>(s + 1u)), p);
    cpu.write(static_cast<std::uint16_t>(0x0100u + static_cast<std::uint8_t>(s + 2u)), 0x40u);
    cpu.write(static_cast<std::uint16_t>(0x0100u + static_cast<std::uint8_t>(s + 3u)), 0x80u);
}

void test_rti_i_variants_with_held_irq(const InstructionCpuFactory& factory)
{
    {
        constexpr std::string_view name = "RTI clears I then held IRQ accepts";
        auto cpu = make_loaded_cpu(factory, compile_rti_restore_program());
        seed_rti_frame(*cpu, flag_u); // I clear after RTI.
        cpu->assert_irq_for(8);
        step(*cpu); // RTI.
        require_marker(*cpu, irq_count, 1u, 8u, name, "held IRQ was not accepted after RTI restored I clear");
    }
    {
        constexpr std::string_view name = "RTI sets I then held IRQ masked";
        auto cpu = make_loaded_cpu(factory, compile_rti_restore_program());
        seed_rti_frame(*cpu, static_cast<std::uint8_t>(flag_u | flag_i));
        cpu->assert_irq_for(5);
        step(*cpu); // RTI.
        step(*cpu, 5u);
        require(cpu->read(irq_count) == 0u, name, "held IRQ ran after RTI restored I set");
        require(cpu->read(marker2) == 0x44u, name, "main did not continue after RTI restored I set");
    }
}

void test_second_nmi_assertion_after_rti(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "second NMI assertion after RTI";
    auto cpu = make_loaded_cpu(factory, compile_standard_handlers());
    cpu->assert_nmi_for(1);
    require_marker(*cpu, nmi_count, 1u, 8u, name, "first NMI assertion was not accepted");
    step(*cpu, 2u);
    cpu->assert_nmi_for(1);
    require_marker(*cpu, nmi_count, 2u, 8u, name, "second NMI assertion was not accepted");
}

void test_nmi_priority_over_irq(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "NMI priority over IRQ";
    const auto program = base_program()
        .org(main_addr, "main")
            .cli()
            .nop()
            .jmp("main")
        .org(nmi_addr, "nmi_handler")
            .inc(asm6502::absolute, nmi_count)
            .rti()
        .org(irq_addr, "irq_handler")
            .inc(asm6502::absolute, irq_count)
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu); // CLI complete.
    cpu->assert_irq_for(8);
    cpu->assert_nmi_for(1);
    require_marker(*cpu, nmi_count, 1u, 8u, name, "NMI did not run first when IRQ was also pending");
    require(cpu->read(irq_count) == 0u, name, "IRQ ran before NMI marker was recorded");
}

void test_nmi_then_held_irq_after_rti(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "NMI then held IRQ after RTI";
    const auto program = base_program()
        .org(main_addr, "main")
            .cli()
            .nop()
            .jmp("main")
        .org(nmi_addr, "nmi_handler")
            .inc(asm6502::absolute, nmi_count)
            .rti()
        .org(irq_addr, "irq_handler")
            .inc(asm6502::absolute, irq_count)
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu); // CLI complete.
    cpu->assert_irq_for(16);
    cpu->assert_nmi_for(1);
    require_marker(*cpu, nmi_count, 1u, 8u, name, "NMI did not run first");
    require_marker(*cpu, irq_count, 1u, 12u, name, "held IRQ did not run after NMI RTI restored I clear");
}

void test_nmi_inside_irq_handler(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "NMI inside IRQ handler";
    const auto program = base_program()
        .org(main_addr, "main")
            .cli()
            .nop()
            .jmp("main")
        .org(irq_addr, "irq_handler")
            .inc(asm6502::absolute, irq_count)
        .org(0x8c58u, "irq_loop")
            .jmp("irq_loop")
        .org(nmi_addr, "nmi_handler")
            .inc(asm6502::absolute, nmi_count)
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu); // CLI complete.
    cpu->assert_irq_for(4);
    require_marker(*cpu, irq_count, 1u, 8u, name, "IRQ handler did not run");
    cpu->assert_nmi_for(1);
    require_marker(*cpu, nmi_count, 1u, 8u, name, "NMI did not interrupt IRQ handler loop");
}

void test_held_irq_does_not_enter_inside_nmi(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "held IRQ does not enter inside NMI";
    const auto program = base_program()
        .org(main_addr, "main")
            .cli()
            .nop()
            .jmp("main")
        .org(nmi_addr, "nmi_handler")
            .inc(asm6502::absolute, nmi_count)
        .org(0x9a3du, "nmi_loop")
            .jmp("nmi_loop")
        .org(irq_addr, "irq_handler")
            .inc(asm6502::absolute, irq_count)
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu); // CLI complete.
    cpu->assert_nmi_for(1);
    require_marker(*cpu, nmi_count, 1u, 8u, name, "NMI handler did not run");
    cpu->assert_irq_for(8);
    step(*cpu, 8u);
    require(cpu->read(irq_count) == 0u, name, "held IRQ entered while CPU was inside NMI with I set");
}

void test_modified_vectors_are_used(const InstructionCpuFactory& factory)
{
    {
        constexpr std::string_view name = "modified IRQ vector is used";
        const auto program = base_program()
            .org(main_addr, "main")
                .cli()
                .nop()
                .jmp("main")
            .org(irq_addr, "irq_handler")
                .lda(0x22u)
                .sta(asm6502::absolute, marker)
                .rti()
            .org(irq_alt_addr, "irq_alt")
                .lda(0x77u)
                .sta(asm6502::absolute, marker)
                .rti()
            .org(nmi_addr, "nmi_handler")
                .rti()
            .end()
            .compile();
        auto cpu = make_loaded_cpu(factory, program);
        cpu->write(0xfffeu, static_cast<std::uint8_t>(irq_alt_addr & 0xffu));
        cpu->write(0xffffu, static_cast<std::uint8_t>(irq_alt_addr >> 8u));
        step(*cpu); // CLI complete.
        cpu->assert_irq_for(6);
        require_marker(*cpu, marker, 0x77u, 10u, name, "IRQ did not use modified vector");
    }
    {
        constexpr std::string_view name = "modified NMI vector is used";
        const auto program = base_program()
            .org(main_addr, "main")
                .nop()
                .jmp("main")
            .org(nmi_addr, "nmi_handler")
                .lda(0x11u)
                .sta(asm6502::absolute, marker)
                .rti()
            .org(nmi_alt_addr, "nmi_alt")
                .lda(0x66u)
                .sta(asm6502::absolute, marker)
                .rti()
            .org(irq_addr, "irq_handler")
                .rti()
            .end()
            .compile();
        auto cpu = make_loaded_cpu(factory, program);
        cpu->write(0xfffau, static_cast<std::uint8_t>(nmi_alt_addr & 0xffu));
        cpu->write(0xfffbu, static_cast<std::uint8_t>(nmi_alt_addr >> 8u));
        cpu->assert_nmi_for(1);
        require_marker(*cpu, marker, 0x66u, 10u, name, "NMI did not use modified vector");
    }
}

void test_pushed_status_pattern_variants(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "pushed status pattern variants";
    const auto program = base_program()
        .org(main_addr, "main")
            .lda(static_cast<std::uint8_t>(flag_u | flag_i | flag_b))
            .pha()
            .plp()
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
    step(*cpu, 4u); // LDA, PHA, PLP, CLI.
    cpu->assert_irq_for(8);
    for (unsigned i = 0; i < 10u && cpu->read(pushed_p) == 0u; ++i) {
        cpu->step_instruction();
    }
    require(cpu->read(pushed_p) != 0u, name, "IRQ did not record pushed status pattern");
    require((cpu->read(pushed_p) & flag_b) == 0u, name, "IRQ pushed B set for status pattern variant");
    require((cpu->read(pushed_p) & flag_i) == 0u, name, "IRQ pushed stale I after CLI-cleared status pattern");
}


void test_held_nmi_assertion_generates_single_nmi(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "held NMI assertion generates single NMI";
    const auto program = base_program()
        .org(main_addr, "main")
            .nop()
            .jmp("main")
        .org(nmi_addr, "nmi_handler")
            .inc(asm6502::absolute, nmi_count)
            .rti()
        .org(irq_addr, "irq_handler")
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    cpu->assert_nmi_for(6);
    require_marker(*cpu, nmi_count, 1u, 8u, name, "held NMI assertion was not accepted");
    step(*cpu, 8u);
    require(cpu->read(nmi_count) == 1u, name, "one held NMI assertion generated more than one NMI entry");
}

void test_held_irq_retriggers_after_rti_when_i_clear(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "held IRQ retriggers after RTI when I clear";
    const auto program = base_program()
        .org(main_addr, "main")
            .cli()
            .nop()
            .jmp("main")
        .org(irq_addr, "irq_handler")
            .tsx()
            .lda(asm6502::abx, 0x0101u) // P pushed by IRQ.
            .and_(static_cast<std::uint8_t>(~flag_i))
            .sta(asm6502::abx, 0x0101u) // RTI restores I clear, so a held IRQ may fire again.
            .inc(asm6502::absolute, irq_count)
            .rti()
        .org(nmi_addr, "nmi_handler")
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu); // CLI complete.
    cpu->assert_irq_for(24);
    require_marker(*cpu, irq_count, 1u, 8u, name, "first IRQ did not run");
    require_marker(*cpu, irq_count, 2u, 16u, name, "held IRQ did not retrigger after RTI restored I clear");
}

void test_cli_pending_irq_observable_delay(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "CLI pending IRQ observable delay";
    const auto program = base_program()
        .org(main_addr, "main")
            .sei()
            .cli()
            .lda(0x44u)
            .sta(asm6502::absolute, marker2)
            .jmp("main")
        .org(irq_addr, "irq_handler")
            .inc(asm6502::absolute, irq_count)
            .rti()
        .org(nmi_addr, "nmi_handler")
            .rti()
        .end()
        .compile();
    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu); // SEI.
    cpu->assert_irq_for(8);
    step(*cpu); // CLI must complete as the one instruction executed by this step.
    require(cpu->read(irq_count) == 0u, name, "IRQ was serviced inside the same step_instruction() call as CLI");
    require_marker(*cpu, irq_count, 1u, 8u, name, "pending IRQ was not accepted after CLI completed");
    require(cpu->read(marker2) == 0u, name, "main instruction after CLI ran before pending IRQ was serviced");
}

using TestFn = void (*)(const InstructionCpuFactory&);
struct TestCase { std::string_view name; TestFn fn; };

} // namespace

int run_instruction_interrupt_group2(const InstructionCpuFactory& factory)
{
    const TestCase tests[] = {
        {"IRQ held masked then CLI accepts", test_irq_held_masked_then_cli_accepts},
        {"SEI masks held IRQ", test_sei_masks_held_irq},
        {"RTI I variants with held IRQ", test_rti_i_variants_with_held_irq},
        {"second NMI assertion after RTI", test_second_nmi_assertion_after_rti},
        {"NMI priority over IRQ", test_nmi_priority_over_irq},
        {"NMI then held IRQ after RTI", test_nmi_then_held_irq_after_rti},
        {"NMI inside IRQ handler", test_nmi_inside_irq_handler},
        {"held IRQ does not enter inside NMI", test_held_irq_does_not_enter_inside_nmi},
        {"modified vectors are used", test_modified_vectors_are_used},
        {"pushed status pattern variants", test_pushed_status_pattern_variants},
        {"held NMI assertion generates single NMI", test_held_nmi_assertion_generates_single_nmi},
        {"held IRQ retriggers after RTI when I clear", test_held_irq_retriggers_after_rti_when_i_clear},
        {"CLI pending IRQ observable delay", test_cli_pending_irq_observable_delay},
    };

    const auto probe = factory();
    const std::string core_name(probe->core_name());
    const std::string model_name(probe->model_name());
    const std::string log_path = "interrupt_basic_" + sanitize_filename_part(core_name) +
        "_" + sanitize_filename_part(model_name) + "_group2.tsv";

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
            log << "group2\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                << tsv_cell(test.name) << "\tpass\t\n";
        } catch (const TestFailure& failure) {
            ++failed;
            std::fprintf(stderr, "FAIL %-42.*s %s\n",
                static_cast<int>(failure.test().size()), failure.test().data(), failure.what());
            log << "group2\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                << tsv_cell(failure.test()) << "\tfail\t" << tsv_cell(failure.what()) << "\n";
        } catch (const std::exception& e) {
            ++failed;
            std::fprintf(stderr, "FAIL %-42.*s %s\n",
                static_cast<int>(test.name.size()), test.name.data(), e.what());
            log << "group2\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                << tsv_cell(test.name) << "\tfail\t" << tsv_cell(e.what()) << "\n";
        }
    }

    std::printf("instruction interrupt group2: %u/%zu passed\n", passed, sizeof(tests) / sizeof(tests[0]));
    std::printf("instruction interrupt group2 log: %s\n", log_path.c_str());
    return failed == 0u ? 0 : 1;
}

} // namespace benchmark6502::interrupts::basic
