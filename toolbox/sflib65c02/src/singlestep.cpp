#include <sflib65c02_toolbox.hpp>

extern "C" {
#include <lib6502.h>
}

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#if !defined(_WIN32)
#include <fcntl.h>
#include <unistd.h>
#endif

namespace {

struct case_result {
    bool instruction_ok = false;
    bool cycle_count_ok = false;
};

class scoped_stderr_silencer {
public:
    scoped_stderr_silencer()
    {
#if !defined(_WIN32)
        ::fflush(stderr);
        saved_fd_ = ::dup(STDERR_FILENO);
        null_fd_ = ::open("/dev/null", O_WRONLY);
        if (saved_fd_ >= 0 && null_fd_ >= 0) {
            ::dup2(null_fd_, STDERR_FILENO);
        }
#endif
    }

    scoped_stderr_silencer(const scoped_stderr_silencer&) = delete;
    scoped_stderr_silencer& operator=(const scoped_stderr_silencer&) = delete;

    ~scoped_stderr_silencer()
    {
#if !defined(_WIN32)
        ::fflush(stderr);
        if (saved_fd_ >= 0) {
            ::dup2(saved_fd_, STDERR_FILENO);
            ::close(saved_fd_);
        }
        if (null_fd_ >= 0) {
            ::close(null_fd_);
        }
#endif
    }

private:
#if !defined(_WIN32)
    int saved_fd_ = -1;
    int null_fd_ = -1;
#endif
};

void load_ram(M6502_Memory memory, const std::vector<benchmark6502::singlestep_memory_cell>& ram)
{
    for (const auto& cell : ram) {
        memory[cell.address] = cell.value;
    }
}

void set_initial_state(M6502_Registers& registers, const benchmark6502::singlestep_cpu_state& state)
{
    registers.pc = state.pc;
    registers.s = state.s;
    registers.a = state.a;
    registers.x = state.x;
    registers.y = state.y;
    registers.p = state.p;
}

bool compare_final_state(const M6502_Registers& registers,
                         const M6502_Memory memory,
                         const benchmark6502::singlestep_cpu_state& expected)
{
    if (registers.pc != expected.pc ||
        registers.s != expected.s ||
        registers.a != expected.a ||
        registers.x != expected.x ||
        registers.y != expected.y ||
        registers.p != expected.p) {
        return false;
    }

    for (const auto& cell : expected.ram) {
        if (memory[cell.address] != cell.value) {
            return false;
        }
    }
    return true;
}

case_result run_case(const benchmark6502::singlestep_case& test_case)
{
    M6502_Registers registers{};
    M6502_Memory memory{};
    M6502_Callbacks callbacks{};

    load_ram(memory, test_case.initial.ram);
    set_initial_state(registers, test_case.initial);

    M6502* cpu = M6502_new(&registers, memory, &callbacks);
    if (cpu == nullptr) {
        return {};
    }

    cpu->ticks = 0;
    M6502_step(cpu);
    const unsigned int cycles = cpu->ticks;

    case_result result;
    result.instruction_ok = compare_final_state(registers, memory, test_case.final);
    result.cycle_count_ok = cycles == test_case.cycles.size();

    M6502_delete(cpu);
    return result;
}

benchmark6502::singlestep_result run_singlestep_65c02_corpus(const benchmark6502::singlestep_corpus& corpus,
                                                              const std::string& display_model_name)
{
    scoped_stderr_silencer silence_undefined_instruction_spam;

    benchmark6502::singlestep_result result;
    result.core_name = "sflib65c02";
    result.corpus_model = corpus.model;
    result.model_name = display_model_name;
    result.cpu_init_model = "sflib65c02";

    for (unsigned opcode_value = 0; opcode_value <= 0xffu; ++opcode_value) {
        const auto opcode = static_cast<std::uint8_t>(opcode_value);
        const benchmark6502::singlestep_opcode_tests& tests = corpus.opcodes[opcode_value];
        benchmark6502::singlestep_opcode_result& opcode_result = result.opcodes[opcode_value];
        opcode_result.opcode = opcode;
        opcode_result.cases_run = static_cast<std::uint64_t>(tests.cases.size());
        opcode_result.instruction.supported = true;
        opcode_result.cycle_count.supported = true;
        opcode_result.bus_trace.supported = false;

        for (const auto& test_case : tests.cases) {
            const case_result single = run_case(test_case);
            if (!single.instruction_ok) {
                opcode_result.instruction.failed = true;
                opcode_result.instruction.failed_cases++;
            }
            if (!single.cycle_count_ok) {
                opcode_result.cycle_count.failed = true;
                opcode_result.cycle_count.failed_cases++;
            }
        }
    }

    return result;
}

} // namespace

namespace sflib65c02_toolbox {

benchmark6502::singlestep_result run_singlestep_wdc65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_65c02_corpus(corpus, "WDC 65C02");
}

benchmark6502::singlestep_result run_singlestep_rockwell65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_65c02_corpus(corpus, "Rockwell 65C02");
}

benchmark6502::singlestep_result run_singlestep_synertek65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_65c02_corpus(corpus, "Synertek/ST 65C02");
}

} // namespace sflib65c02_toolbox
