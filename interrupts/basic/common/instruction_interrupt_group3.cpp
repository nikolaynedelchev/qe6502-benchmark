#include "instruction_interrupt_group3.hpp"

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

constexpr std::uint16_t main_addr = 0x8000u;
constexpr std::uint16_t nmi_addr = 0x9a37u;
constexpr std::uint16_t irq_addr = 0x8c51u;

constexpr std::uint16_t irq_count = 0x0201u;
constexpr std::uint16_t nmi_count = 0x0202u;
constexpr std::uint16_t marker = 0x0203u;

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
    cpu->write(irq_count, 0u);
    cpu->write(nmi_count, 0u);
    cpu->write(marker, 0u);
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

void test_held_irq_does_not_retrigger_when_rti_restores_i_set(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "held IRQ does not retrigger when RTI restores I set";
    const auto program = base_program()
        .org(main_addr, "main")
            .cli()
        .org(0x8001u, "after_cli")
            .nop()
            .jmp("after_cli")
        .org(irq_addr, "irq_handler")
            .tsx()
            .lda(asm6502::abx, 0x0101u) // P pushed by IRQ.
            .ora(flag_i)
            .sta(asm6502::abx, 0x0101u) // RTI restores I set, so the held IRQ must not retrigger.
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
    step(*cpu, 12u);
    require(cpu->read(irq_count) == 1u, name, "held IRQ retriggered even though RTI restored I set");
}

void test_short_irq_assertion_expires_before_cli(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "short IRQ assertion expires before CLI";
    const auto program = base_program()
        .org(main_addr, "main")
            .sei()
            .nop()
            .cli()
            .lda(0x44u)
            .sta(asm6502::absolute, marker)
            .jmp("main")
        .org(irq_addr, "irq_handler")
            .inc(asm6502::absolute, irq_count)
            .rti()
        .org(nmi_addr, "nmi_handler")
            .rti()
        .end()
        .compile();

    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu); // SEI, keeping I set explicitly.
    cpu->assert_irq_for(1);
    step(*cpu); // NOP consumes the whole short IRQ assertion while I is set.
    step(*cpu); // CLI after the IRQ assertion has expired.
    step(*cpu, 2u); // LDA, STA marker.
    require(cpu->read(irq_count) == 0u, name, "expired short IRQ assertion was accepted after CLI");
    require(cpu->read(marker) == 0x44u, name, "main did not continue after expired IRQ assertion");
}

void test_long_irq_assertion_survives_until_cli(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "long IRQ assertion survives until CLI";
    const auto program = base_program()
        .org(main_addr, "main")
            .sei()
            .nop()
            .cli()
            .lda(0x44u)
            .sta(asm6502::absolute, marker)
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
    cpu->assert_irq_for(4);
    step(*cpu); // NOP while I is set.
    require(cpu->read(irq_count) == 0u, name, "IRQ ran while I was still set before CLI");
    step(*cpu); // CLI.
    require_marker(*cpu, irq_count, 1u, 8u, name, "long IRQ assertion was not accepted after CLI");
    require(cpu->read(marker) == 0u, name, "main instruction after CLI ran before held IRQ was serviced");
}

void test_short_irq_during_nmi_expires_before_unmask(const InstructionCpuFactory& factory)
{
    constexpr std::string_view name = "short IRQ during NMI expires before unmask";
    const auto program = base_program()
        .org(main_addr, "main")
            .cli()
            .nop()
            .jmp("main")
        .org(nmi_addr, "nmi_handler")
            .inc(asm6502::absolute, nmi_count)
            .nop()
            .rti()
        .org(irq_addr, "irq_handler")
            .inc(asm6502::absolute, irq_count)
            .rti()
        .end()
        .compile();

    auto cpu = make_loaded_cpu(factory, program);
    step(*cpu); // CLI complete.
    cpu->assert_nmi_for(1);
    require_marker(*cpu, nmi_count, 1u, 8u, name, "NMI did not run before IRQ duration check");
    cpu->assert_irq_for(1);
    step(*cpu); // NOP inside NMI, with I set; the short IRQ assertion expires here.
    step(*cpu); // RTI restores I clear after the IRQ assertion has expired.
    step(*cpu, 4u);
    require(cpu->read(irq_count) == 0u, name, "expired IRQ assertion during NMI became a sticky pending IRQ");
}

using TestFn = void (*)(const InstructionCpuFactory&);
struct TestCase { std::string_view name; TestFn fn; };

} // namespace

int run_instruction_interrupt_group3(const InstructionCpuFactory& factory)
{
    const TestCase tests[] = {
        {"held IRQ does not retrigger when RTI restores I set", test_held_irq_does_not_retrigger_when_rti_restores_i_set},
        {"short IRQ assertion expires before CLI", test_short_irq_assertion_expires_before_cli},
        {"long IRQ assertion survives until CLI", test_long_irq_assertion_survives_until_cli},
        {"short IRQ during NMI expires before unmask", test_short_irq_during_nmi_expires_before_unmask},
    };

    const auto probe = factory();
    const std::string core_name(probe->core_name());
    const std::string model_name(probe->model_name());
    const std::string log_path = "interrupt_basic_" + sanitize_filename_part(core_name) +
        "_" + sanitize_filename_part(model_name) + "_group3.tsv";

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
            std::printf("PASS %-52.*s\n", static_cast<int>(test.name.size()), test.name.data());
            log << "group3\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                << tsv_cell(test.name) << "\tpass\t\n";
        } catch (const TestFailure& failure) {
            ++failed;
            std::fprintf(stderr, "FAIL %-52.*s %s\n",
                static_cast<int>(failure.test().size()), failure.test().data(), failure.what());
            log << "group3\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                << tsv_cell(failure.test()) << "\tfail\t" << tsv_cell(failure.what()) << "\n";
        } catch (const std::exception& e) {
            ++failed;
            std::fprintf(stderr, "FAIL %-52.*s %s\n",
                static_cast<int>(test.name.size()), test.name.data(), e.what());
            log << "group3\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                << tsv_cell(test.name) << "\tfail\t" << tsv_cell(e.what()) << "\n";
        }
    }

    std::printf("instruction interrupt group3: %u/%zu passed\n", passed, sizeof(tests) / sizeof(tests[0]));
    std::printf("instruction interrupt group3 log: %s\n", log_path.c_str());
    return failed == 0u ? 0 : 1;
}

} // namespace benchmark6502::interrupts::basic
