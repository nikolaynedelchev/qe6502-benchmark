#include "instruction_interrupt_opcode_sweep.hpp"

#include <asm6502/asm6502.h>
#include <benchmark6502/opcode_metadata.hpp>
#include <tools6502/testcase_collections.hpp>

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace benchmark6502::interrupts::basic {
namespace {

constexpr std::uint16_t main_addr = 0x8000u;
constexpr std::uint16_t target_addr = 0x8010u;
constexpr std::uint16_t nmi_addr = 0x9a37u;
constexpr std::uint16_t irq_addr = 0x8c51u;
constexpr std::uint16_t marker = 0x0200u;
constexpr std::uint16_t irq_count = 0x0201u;
constexpr std::uint16_t nmi_count = 0x0202u;
constexpr std::uint16_t continuation = 0x0203u;
constexpr std::uint16_t pushed_pcl = 0x0204u;
constexpr std::uint16_t pushed_pch = 0x0205u;
constexpr std::uint16_t testcase_irq_count = 0x7f00u;
constexpr std::uint16_t testcase_nmi_count = 0x7f01u;

enum class FailureClass {
    none,
    skipped_opcode,
    synthetic_boundary,
    synthetic_irq_mask,
    testcase_setup,
    testcase_precondition,
    post_opcode_nmi,
    post_opcode_irq,
    other,
};

const char* failure_class_name(FailureClass failure_class) noexcept
{
    switch (failure_class) {
    case FailureClass::none: return "none";
    case FailureClass::skipped_opcode: return "skipped_opcode";
    case FailureClass::synthetic_boundary: return "synthetic_boundary";
    case FailureClass::synthetic_irq_mask: return "synthetic_irq_mask";
    case FailureClass::testcase_setup: return "testcase_setup";
    case FailureClass::testcase_precondition: return "testcase_precondition";
    case FailureClass::post_opcode_nmi: return "post_opcode_nmi";
    case FailureClass::post_opcode_irq: return "post_opcode_irq";
    case FailureClass::other: return "other";
    }
    return "other";
}

bool is_skip_failure(FailureClass failure_class) noexcept
{
    return failure_class == FailureClass::skipped_opcode;
}

class TestFailure : public std::runtime_error {
public:
    TestFailure(FailureClass failure_class, std::string message)
        : std::runtime_error(std::move(message))
        , failure_class_(failure_class)
    {
    }

    FailureClass failure_class() const noexcept { return failure_class_; }

private:
    FailureClass failure_class_ = FailureClass::other;
};

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

std::string hex2(std::uint8_t value)
{
    constexpr char digits[] = "0123456789ABCDEF";
    std::string out = "$00";
    out[1] = digits[(value >> 4u) & 0x0fu];
    out[2] = digits[value & 0x0fu];
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
    cpu->write(continuation, 0u);
    cpu->write(pushed_pcl, 0u);
    cpu->write(pushed_pch, 0u);
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


bool step_until_pc(IInstructionCpu& cpu, std::uint16_t pc, unsigned max_steps)
{
    for (unsigned i = 0; i < max_steps; ++i) {
        if (cpu.state().pc == pc) {
            return true;
        }
        cpu.step_instruction();
    }
    return cpu.state().pc == pc;
}

std::vector<std::uint8_t> operand_bytes(const opcode_metadata& metadata)
{
    switch (metadata.bytes) {
    case 1u:
        return {};
    case 2u:
        if (metadata.mode == addressing_mode::relative) {
            return {0x02u}; // Branch forward over the first byte of the continuation LDA if it ever executes.
        }
        return {0x80u};
    case 3u:
        return {0x80u, 0x00u};
    default:
        return {};
    }
}

bool is_jam_opcode(std::uint8_t opcode) noexcept
{
    switch (opcode) {
    case 0x02u:
    case 0x12u:
    case 0x22u:
    case 0x32u:
    case 0x42u:
    case 0x52u:
    case 0x62u:
    case 0x72u:
    case 0x92u:
    case 0xb2u:
    case 0xd2u:
    case 0xf2u:
        return true;
    default:
        return false;
    }
}

bool is_unstable_opcode(std::uint8_t opcode) noexcept
{
    switch (opcode) {
    case 0x0bu:
    case 0x2bu:
    case 0x4bu:
    case 0x6bu:
    case 0x8bu:
    case 0xabu:
    case 0xbbu:
        return true;
    default:
        return false;
    }
}

bool is_skipped_opcode(std::uint8_t opcode) noexcept
{
    return is_jam_opcode(opcode) || is_unstable_opcode(opcode);
}

std::vector<asm6502::mem_value> interrupt_probe_handlers()
{
    return asm6502::Asm6502::New()
        .begin()
        .org(nmi_addr, "nmi_probe_handler")
            .inc(asm6502::absolute, testcase_nmi_count)
            .rti()
        .org(irq_addr, "irq_probe_handler")
            .inc(asm6502::absolute, testcase_irq_count)
            .rti()
        .end()
        .compile();
}

std::unique_ptr<IInstructionCpu> make_loaded_testcase_cpu(
    const InstructionCpuFactory& factory,
    const tools6502::testcase& test)
{
    auto cpu = factory();
    cpu->clear_memory(0xeau);

    auto bootstrap_test = test;
    bootstrap_test.vectors.reset = 0xfe00u;
    bootstrap_test.vectors.brk_irq = 0xfe00u;
    bootstrap_test.vectors.nmi = 0xfe00u;

    const auto bootstrap = tools6502::make_bootstrap(bootstrap_test);
    const auto handlers = interrupt_probe_handlers();
    cpu->load(test.mem_setup);
    cpu->load(test.program);
    cpu->load(bootstrap);
    cpu->load(handlers);
    cpu->reset();
    cpu->write(testcase_irq_count, 0u);
    cpu->write(testcase_nmi_count, 0u);
    return cpu;
}


void point_vectors_to_testcase(IInstructionCpu& cpu, const tools6502::testcase& test)
{
    cpu.write(0xfffcu, static_cast<std::uint8_t>(test.vectors.reset & 0xffu));
    cpu.write(0xfffdu, static_cast<std::uint8_t>(test.vectors.reset >> 8u));
    cpu.write(0xfffau, static_cast<std::uint8_t>(test.vectors.nmi & 0xffu));
    cpu.write(0xfffbu, static_cast<std::uint8_t>(test.vectors.nmi >> 8u));
    cpu.write(0xfffeu, static_cast<std::uint8_t>(test.vectors.brk_irq & 0xffu));
    cpu.write(0xffffu, static_cast<std::uint8_t>(test.vectors.brk_irq >> 8u));
}

void point_vectors_to_probe_handlers(IInstructionCpu& cpu)
{
    cpu.write(0xfffau, static_cast<std::uint8_t>(nmi_addr & 0xffu));
    cpu.write(0xfffbu, static_cast<std::uint8_t>(nmi_addr >> 8u));
    cpu.write(0xfffeu, static_cast<std::uint8_t>(irq_addr & 0xffu));
    cpu.write(0xffffu, static_cast<std::uint8_t>(irq_addr >> 8u));
}


bool is_self_jump_trap_at(const IInstructionCpu& cpu, std::uint16_t pc)
{
    return cpu.read(pc) == 0x4cu
        && cpu.read(static_cast<std::uint16_t>(pc + 1u)) == static_cast<std::uint8_t>(pc & 0xffu)
        && cpu.read(static_cast<std::uint16_t>(pc + 2u)) == static_cast<std::uint8_t>(pc >> 8u);
}

std::uint16_t recorded_pushed_pc(const IInstructionCpu& cpu)
{
    return static_cast<std::uint16_t>(cpu.read(pushed_pcl))
        | static_cast<std::uint16_t>(static_cast<std::uint16_t>(cpu.read(pushed_pch)) << 8u);
}

void require_recorded_pushed_pc_is_one_of(const IInstructionCpu& cpu,
                                          std::uint16_t expected_a,
                                          std::uint16_t expected_b,
                                          std::string_view message)
{
    const auto pushed_pc = recorded_pushed_pc(cpu);
    if (pushed_pc != expected_a && pushed_pc != expected_b) {
        throw TestFailure(FailureClass::synthetic_boundary, std::string(message));
    }
}

void patch_current_pc_with_cli_probe(IInstructionCpu& cpu)
{
    const auto pc = cpu.state().pc;
    cpu.write(pc, 0x58u); // CLI
    cpu.write(static_cast<std::uint16_t>(pc + 1u), 0xeau); // NOP
    cpu.write(static_cast<std::uint16_t>(pc + 2u), 0x4cu); // JMP pc+1, a stable post-CLI loop.
    cpu.write(static_cast<std::uint16_t>(pc + 3u), static_cast<std::uint8_t>((pc + 1u) & 0xffu));
    cpu.write(static_cast<std::uint16_t>(pc + 4u), static_cast<std::uint8_t>((pc + 1u) >> 8u));
}

void run_full_testcase_then_interrupt_case(
    const InstructionCpuFactory& factory,
    const tools6502::testcase& test,
    bool nmi)
{
    if (is_skipped_opcode(test.opcode)) {
        throw TestFailure(FailureClass::skipped_opcode,
            "opcode skipped because it is KIL/JAM or one of the seven known unstable NMOS opcodes");
    }

    auto cpu = make_loaded_testcase_cpu(factory, test);
    constexpr unsigned max_bootstrap_steps = 16u;
    if (!step_until_pc(*cpu, test.start_at, max_bootstrap_steps)) {
        throw TestFailure(FailureClass::testcase_setup,
            "testcase bootstrap did not reach opcode start address");
    }
    point_vectors_to_testcase(*cpu, test);

    cpu->step_instruction(); // Execute the opcode under test using its existing mini-program.
    const auto trap_pc = cpu->state().pc;
    if (!is_self_jump_trap_at(*cpu, trap_pc)) {
        throw TestFailure(FailureClass::testcase_precondition,
            "testcase opcode did not reach its terminal self-jump trap before interrupt probe");
    }
    point_vectors_to_probe_handlers(*cpu);

    if (nmi) {
        cpu->assert_nmi_for(1);
        if (!step_until(*cpu, testcase_nmi_count, 1u, 12u)) {
            throw TestFailure(FailureClass::post_opcode_nmi,
                "NMI was not accepted after executing testcase opcode");
        }
    } else {
        patch_current_pc_with_cli_probe(*cpu);
        cpu->step_instruction(); // CLI at the current post-opcode fetch address.
        cpu->assert_irq_for(1);
        if (!step_until(*cpu, testcase_irq_count, 1u, 12u)) {
            throw TestFailure(FailureClass::post_opcode_irq,
                "IRQ was not accepted after executing testcase opcode and clearing I");
        }
    }
}

enum class SweepCase {
    first_reset_nmi,
    first_reset_irq_masked,
    prefixed_nmi,
    prefixed_irq,
    testcase_then_nmi,
    testcase_then_irq,
};

std::string case_name(SweepCase c)
{
    switch (c) {
    case SweepCase::first_reset_nmi: return "first_reset_nmi_reaches_handler";
    case SweepCase::first_reset_irq_masked: return "first_reset_irq_masked_opcode_executes";
    case SweepCase::prefixed_nmi: return "prefixed_nmi_before_target";
    case SweepCase::prefixed_irq: return "prefixed_irq_before_target";
    case SweepCase::testcase_then_nmi: return "testcase_opcode_then_nmi";
    case SweepCase::testcase_then_irq: return "testcase_opcode_then_irq";
    }
    return "unknown";
}

std::vector<asm6502::mem_value> compile_sweep_program(const opcode_metadata& metadata, SweepCase c)
{
    auto p = base_program();
    p.org(main_addr, "main");
    switch (c) {
    case SweepCase::first_reset_nmi:
    case SweepCase::first_reset_irq_masked:
        break;
    case SweepCase::prefixed_nmi:
        p.nop().jmp("target");
        break;
    case SweepCase::prefixed_irq:
        p.cli().nop().jmp("target");
        break;
    case SweepCase::testcase_then_nmi:
    case SweepCase::testcase_then_irq:
        break;
    }

    p.org((c == SweepCase::first_reset_nmi || c == SweepCase::first_reset_irq_masked
               || c == SweepCase::testcase_then_nmi || c == SweepCase::testcase_then_irq) ? main_addr : target_addr, "target")
        .db(metadata.opcode);
    for (const auto byte : operand_bytes(metadata)) {
        p.db(byte);
    }
    p.lda(0x44u)
        .sta(asm6502::absolute, continuation)
        .jmp("done")
     .org(0x8040u, "done")
        .jmp("done")
     .org(nmi_addr, "nmi_handler")
        .tsx()
        .lda(asm6502::abx, 0x0102u)
        .sta(asm6502::absolute, pushed_pcl)
        .lda(asm6502::abx, 0x0103u)
        .sta(asm6502::absolute, pushed_pch)
        .inc(asm6502::absolute, nmi_count)
        .rti()
     .org(irq_addr, "irq_handler")
        .tsx()
        .lda(asm6502::abx, 0x0102u)
        .sta(asm6502::absolute, pushed_pcl)
        .lda(asm6502::abx, 0x0103u)
        .sta(asm6502::absolute, pushed_pch)
        .inc(asm6502::absolute, irq_count)
        .rti()
     .org(0x0080u)
        .db(0x34u, 0x12u)
     .org(0x1234u)
        .db(0x56u);
    return p.end().compile();
}

void run_sweep_case(const InstructionCpuFactory& factory, const opcode_metadata& metadata, SweepCase c)
{
    if (is_skipped_opcode(metadata.opcode)) {
        throw TestFailure(FailureClass::skipped_opcode,
            "opcode skipped because it is KIL/JAM or one of the seven known unstable NMOS opcodes");
    }

    auto cpu = make_loaded_cpu(factory, compile_sweep_program(metadata, c));
    switch (c) {
    case SweepCase::first_reset_nmi:
        cpu->assert_nmi_for(1);
        if (!step_until(*cpu, nmi_count, 1u, 8u)) {
            throw TestFailure(FailureClass::synthetic_boundary,
                "NMI was not accepted before first opcode after reset");
        }
        if (cpu->read(continuation) != 0u) {
            throw TestFailure(FailureClass::synthetic_boundary,
                "target opcode continuation ran before NMI handler");
        }
        break;
    case SweepCase::first_reset_irq_masked:
        cpu->assert_irq_for(1);
        cpu->step_instruction();
        if (cpu->read(irq_count) != 0u) {
            throw TestFailure(FailureClass::synthetic_irq_mask,
                "IRQ handler instruction ran even though reset I should mask IRQ");
        }
        if (cpu->state().pc == irq_addr && metadata.opcode != 0x00u) {
            throw TestFailure(FailureClass::synthetic_irq_mask,
                "CPU entered IRQ/BRK vector while external IRQ should be masked");
        }
        if (metadata.opcode == 0x00u && cpu->state().pc == irq_addr) {
            const std::uint8_t pushed_p = cpu->read(static_cast<std::uint16_t>(0x0100u + cpu->state().s + 1u));
            if ((pushed_p & 0x10u) == 0u) {
                throw TestFailure(FailureClass::synthetic_irq_mask,
                    "BRK reached IRQ/BRK vector but pushed status did not mark BRK; external IRQ may have preempted it");
            }
        }
        break;
    case SweepCase::prefixed_nmi:
        step(*cpu); // NOP prefix. The following JMP is a harmless latency absorber before the target opcode.
        cpu->assert_nmi_for(1);
        if (!step_until(*cpu, nmi_count, 1u, 8u)) {
            throw TestFailure(FailureClass::synthetic_boundary,
                "NMI was not accepted before prefixed target opcode");
        }
        require_recorded_pushed_pc_is_one_of(*cpu,
                                             static_cast<std::uint16_t>(main_addr + 1u),
                                             target_addr,
                                             "NMI stack frame was not at the prefix JMP or target fetch; target opcode may have run before NMI");
        if (cpu->read(continuation) != 0u) {
            throw TestFailure(FailureClass::synthetic_boundary,
                "target opcode continuation ran before prefixed NMI handler");
        }
        break;
    case SweepCase::prefixed_irq:
        step(*cpu, 2u); // CLI, NOP. The following JMP is a harmless latency absorber before the target opcode.
        cpu->assert_irq_for(1);
        if (!step_until(*cpu, irq_count, 1u, 8u)) {
            throw TestFailure(FailureClass::synthetic_boundary,
                "IRQ was not accepted before prefixed target opcode with I clear");
        }
        require_recorded_pushed_pc_is_one_of(*cpu,
                                             static_cast<std::uint16_t>(main_addr + 2u),
                                             target_addr,
                                             "IRQ stack frame was not at the prefix JMP or target fetch; target opcode may have run before IRQ");
        if (cpu->read(continuation) != 0u) {
            throw TestFailure(FailureClass::synthetic_boundary,
                "target opcode continuation ran before prefixed IRQ handler");
        }
        break;
    case SweepCase::testcase_then_nmi:
    case SweepCase::testcase_then_irq:
        throw TestFailure(FailureClass::other,
            "internal error: testcase sweep case used with synthetic runner");
    }
}

struct SweepStats {
    unsigned passed = 0;
    unsigned failed = 0;
    unsigned skipped = 0;
    unsigned legal_passed = 0;
    unsigned legal_failed = 0;
    unsigned legal_skipped = 0;
    unsigned illegal_passed = 0;
    unsigned illegal_failed = 0;
    unsigned illegal_skipped = 0;
    unsigned synthetic_boundary_failed = 0;
    unsigned synthetic_irq_mask_failed = 0;
    unsigned testcase_setup_failed = 0;
    unsigned testcase_precondition_failed = 0;
    unsigned post_opcode_nmi_failed = 0;
    unsigned post_opcode_irq_failed = 0;
    unsigned other_failed = 0;
};

void count_pass(SweepStats& stats, bool legal)
{
    ++stats.passed;
    legal ? ++stats.legal_passed : ++stats.illegal_passed;
}

void count_failure_class(SweepStats& stats, FailureClass failure_class)
{
    switch (failure_class) {
    case FailureClass::synthetic_boundary:
        ++stats.synthetic_boundary_failed;
        break;
    case FailureClass::synthetic_irq_mask:
        ++stats.synthetic_irq_mask_failed;
        break;
    case FailureClass::testcase_setup:
        ++stats.testcase_setup_failed;
        break;
    case FailureClass::testcase_precondition:
        ++stats.testcase_precondition_failed;
        break;
    case FailureClass::post_opcode_nmi:
        ++stats.post_opcode_nmi_failed;
        break;
    case FailureClass::post_opcode_irq:
        ++stats.post_opcode_irq_failed;
        break;
    case FailureClass::none:
    case FailureClass::skipped_opcode:
    case FailureClass::other:
        ++stats.other_failed;
        break;
    }
}

void count_fail(SweepStats& stats, bool legal, FailureClass failure_class)
{
    ++stats.failed;
    legal ? ++stats.legal_failed : ++stats.illegal_failed;
    count_failure_class(stats, failure_class);
}

void count_skip(SweepStats& stats, bool legal)
{
    ++stats.skipped;
    legal ? ++stats.legal_skipped : ++stats.illegal_skipped;
}

struct TestcaseFailureCounts {
    unsigned setup = 0;
    unsigned precondition = 0;
    unsigned post_nmi = 0;
    unsigned post_irq = 0;
    unsigned other = 0;

    void add(FailureClass failure_class)
    {
        switch (failure_class) {
        case FailureClass::testcase_setup:
            ++setup;
            break;
        case FailureClass::testcase_precondition:
            ++precondition;
            break;
        case FailureClass::post_opcode_nmi:
            ++post_nmi;
            break;
        case FailureClass::post_opcode_irq:
            ++post_irq;
            break;
        case FailureClass::none:
        case FailureClass::skipped_opcode:
        case FailureClass::synthetic_boundary:
        case FailureClass::synthetic_irq_mask:
        case FailureClass::other:
            ++other;
            break;
        }
    }

    std::string detail() const
    {
        return " failure_classes setup=" + std::to_string(setup)
            + " precondition=" + std::to_string(precondition)
            + " post_nmi=" + std::to_string(post_nmi)
            + " post_irq=" + std::to_string(post_irq)
            + " other=" + std::to_string(other);
    }
};

} // namespace

int run_instruction_interrupt_opcode_sweep(const InstructionCpuFactory& factory)
{
    const auto probe = factory();
    const std::string core_name(probe->core_name());
    const std::string model_name(probe->model_name());
    const std::string log_path = "interrupt_basic_" + sanitize_filename_part(core_name) +
        "_" + sanitize_filename_part(model_name) + "_opcode_sweep.tsv";

    std::ofstream log(log_path);
    if (!log) {
        std::fprintf(stderr, "failed to open %s for writing\n", log_path.c_str());
        return 1;
    }
    log << "suite\tcore\tmodel\tcase\topcode\tmnemonic\tmode\tlegality\tstatus\tdetail\tfailure_class\n";

    const SweepCase cases[] = {
        SweepCase::first_reset_nmi,
        SweepCase::first_reset_irq_masked,
        SweepCase::prefixed_nmi,
        SweepCase::prefixed_irq,
        SweepCase::testcase_then_nmi,
        SweepCase::testcase_then_irq,
    };

    const auto testcases_by_opcode = tools6502::get_nmos6502_opcode_testcases();

    SweepStats stats;
    for (const auto c : cases) {
        for (const auto& metadata : nmos_opcode_metadata_table) {
            const auto cn = case_name(c);
            const auto op = hex2(metadata.opcode);
            const bool legal = metadata.legal_nmos;

            if (c == SweepCase::testcase_then_nmi || c == SweepCase::testcase_then_irq) {
                const auto found = testcases_by_opcode.find(metadata.opcode);
                if (found == testcases_by_opcode.end() || found->second.empty()) {
                    count_skip(stats, legal);
                    log << "opcode_sweep\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                        << cn << "\t" << op << "\t" << metadata.mnemonic << "\t"
                        << addressing_mode_name(metadata.mode) << "\t" << (legal ? "legal" : "illegal")
                        << "\tskip\tno testcase programs available\tno_testcase\n";
                    continue;
                }

                bool opcode_failed = false;
                bool opcode_skipped_all = true;
                std::string first_failure;
                unsigned testcase_passed = 0;
                unsigned testcase_failed = 0;
                unsigned testcase_skipped = 0;
                FailureClass first_failure_class = FailureClass::none;
                TestcaseFailureCounts failure_counts{};

                for (const auto& test : found->second) {
                    try {
                        run_full_testcase_then_interrupt_case(factory, test, c == SweepCase::testcase_then_nmi);
                        ++testcase_passed;
                        opcode_skipped_all = false;
                    } catch (const TestFailure& e) {
                        const std::string detail(e.what());
                        if (is_skip_failure(e.failure_class())) {
                            ++testcase_skipped;
                        } else {
                            ++testcase_failed;
                            opcode_failed = true;
                            opcode_skipped_all = false;
                            failure_counts.add(e.failure_class());
                            if (first_failure.empty()) {
                                first_failure = detail + "; testcase: " + test.description;
                                first_failure_class = e.failure_class();
                            }
                        }
                    } catch (const std::exception& e) {
                        ++testcase_failed;
                        opcode_failed = true;
                        opcode_skipped_all = false;
                        failure_counts.add(FailureClass::other);
                        if (first_failure.empty()) {
                            first_failure = std::string(e.what()) + "; testcase: " + test.description;
                            first_failure_class = FailureClass::other;
                        }
                    }
                }

                const std::string detail = "testcases pass=" + std::to_string(testcase_passed)
                    + " fail=" + std::to_string(testcase_failed)
                    + " skip=" + std::to_string(testcase_skipped)
                    + failure_counts.detail()
                    + (first_failure.empty() ? std::string{} : std::string{"; first_failure="} + first_failure);

                if (opcode_failed) {
                    count_fail(stats, legal, first_failure_class);
                    std::fprintf(stderr, "FAIL %-34s %s %s %s\n", cn.c_str(), op.c_str(), metadata.mnemonic, first_failure.c_str());
                    log << "opcode_sweep\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                        << cn << "\t" << op << "\t" << metadata.mnemonic << "\t"
                        << addressing_mode_name(metadata.mode) << "\t" << (legal ? "legal" : "illegal")
                        << "\tfail\t" << tsv_cell(detail) << "\t" << failure_class_name(first_failure_class) << "\n";
                } else if (opcode_skipped_all) {
                    count_skip(stats, legal);
                    log << "opcode_sweep\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                        << cn << "\t" << op << "\t" << metadata.mnemonic << "\t"
                        << addressing_mode_name(metadata.mode) << "\t" << (legal ? "legal" : "illegal")
                        << "\tskip\t" << tsv_cell(detail) << "\t" << failure_class_name(FailureClass::skipped_opcode) << "\n";
                } else {
                    count_pass(stats, legal);
                    log << "opcode_sweep\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                        << cn << "\t" << op << "\t" << metadata.mnemonic << "\t"
                        << addressing_mode_name(metadata.mode) << "\t" << (legal ? "legal" : "illegal")
                        << "\tpass\t" << tsv_cell(detail) << "\t" << failure_class_name(FailureClass::none) << "\n";
                }
                continue;
            }

            try {
                run_sweep_case(factory, metadata, c);
                count_pass(stats, legal);
                log << "opcode_sweep\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                    << cn << "\t" << op << "\t" << metadata.mnemonic << "\t"
                    << addressing_mode_name(metadata.mode) << "\t" << (legal ? "legal" : "illegal")
                    << "\tpass\t\t" << failure_class_name(FailureClass::none) << "\n";
            } catch (const TestFailure& e) {
                const std::string detail(e.what());
                if (is_skip_failure(e.failure_class())) {
                    count_skip(stats, legal);
                    log << "opcode_sweep\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                        << cn << "\t" << op << "\t" << metadata.mnemonic << "\t"
                        << addressing_mode_name(metadata.mode) << "\t" << (legal ? "legal" : "illegal")
                        << "\tskip\t" << tsv_cell(detail) << "\t" << failure_class_name(e.failure_class()) << "\n";
                } else {
                    count_fail(stats, legal, e.failure_class());
                    std::fprintf(stderr, "FAIL %-34s %s %s %s\n", cn.c_str(), op.c_str(), metadata.mnemonic, e.what());
                    log << "opcode_sweep\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                        << cn << "\t" << op << "\t" << metadata.mnemonic << "\t"
                        << addressing_mode_name(metadata.mode) << "\t" << (legal ? "legal" : "illegal")
                        << "\tfail\t" << tsv_cell(detail) << "\t" << failure_class_name(e.failure_class()) << "\n";
                }
            } catch (const std::exception& e) {
                count_fail(stats, legal, FailureClass::other);
                std::fprintf(stderr, "FAIL %-34s %s %s %s\n", cn.c_str(), op.c_str(), metadata.mnemonic, e.what());
                log << "opcode_sweep\t" << tsv_cell(core_name) << "\t" << tsv_cell(model_name) << "\t"
                    << cn << "\t" << op << "\t" << metadata.mnemonic << "\t"
                    << addressing_mode_name(metadata.mode) << "\t" << (legal ? "legal" : "illegal")
                    << "\tfail\t" << tsv_cell(e.what()) << "\t" << failure_class_name(FailureClass::other) << "\n";
            }
        }
    }

    std::printf("instruction interrupt opcode sweep: %u passed, %u skipped, %u failed\n",
        stats.passed, stats.skipped, stats.failed);
    std::printf("instruction interrupt opcode sweep legal: %u passed, %u skipped, %u failed\n",
        stats.legal_passed, stats.legal_skipped, stats.legal_failed);
    std::printf("instruction interrupt opcode sweep illegal: %u passed, %u skipped, %u failed\n",
        stats.illegal_passed, stats.illegal_skipped, stats.illegal_failed);
    std::printf("instruction interrupt opcode sweep failure classes: synthetic_boundary=%u synthetic_irq_mask=%u testcase_setup=%u testcase_precondition=%u post_opcode_nmi=%u post_opcode_irq=%u other=%u\n",
        stats.synthetic_boundary_failed,
        stats.synthetic_irq_mask_failed,
        stats.testcase_setup_failed,
        stats.testcase_precondition_failed,
        stats.post_opcode_nmi_failed,
        stats.post_opcode_irq_failed,
        stats.other_failed);
    std::printf("instruction interrupt opcode sweep log: %s\n", log_path.c_str());
    return stats.failed == 0u ? 0 : 1;
}

} // namespace benchmark6502::interrupts::basic
