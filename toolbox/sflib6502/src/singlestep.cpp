#include <sflib6502_toolbox.hpp>

#include <benchmark6502/opcode_metadata.hpp>

extern "C" {
#include <lib6502.h>
}

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#if defined(_WIN32)
#include <io.h>
#define dup _dup
#define dup2 _dup2
#define close _close
#define fileno _fileno
#else
#include <unistd.h>
#endif

namespace {

struct case_result {
    bool instruction_ok = false;
    bool cycle_count_ok = false;
};


class scoped_stderr_silence {
public:
    scoped_stderr_silence()
    {
        std::fflush(stderr);
        saved_stderr_ = dup(fileno(stderr));
        null_file_ = std::fopen(null_device_path(), "w");
        if (saved_stderr_ >= 0 && null_file_ != nullptr) {
            dup2(fileno(null_file_), fileno(stderr));
            active_ = true;
        }
    }

    ~scoped_stderr_silence()
    {
        std::fflush(stderr);
        if (active_) {
            dup2(saved_stderr_, fileno(stderr));
        }
        if (saved_stderr_ >= 0) {
            close(saved_stderr_);
        }
        if (null_file_ != nullptr) {
            std::fclose(null_file_);
        }
    }

    scoped_stderr_silence(const scoped_stderr_silence&) = delete;
    scoped_stderr_silence& operator=(const scoped_stderr_silence&) = delete;

private:
    static const char* null_device_path()
    {
#if defined(_WIN32)
        return "NUL";
#else
        return "/dev/null";
#endif
    }

    int saved_stderr_ = -1;
    FILE* null_file_ = nullptr;
    bool active_ = false;
};

void load_ram(M6502_Memory memory, const std::vector<benchmark6502::singlestep_memory_cell>& ram)
{
    std::memset(memory, 0, sizeof(M6502_Memory));
    for (const auto& cell : ram) {
        memory[cell.address] = cell.value;
    }
}

void set_initial_state(M6502_Registers& registers, const benchmark6502::singlestep_cpu_state& initial)
{
    registers.a = initial.a;
    registers.x = initial.x;
    registers.y = initial.y;
    registers.p = initial.p;
    registers.s = initial.s;
    registers.pc = initial.pc;
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

    case_result result;
    result.instruction_ok = compare_final_state(registers, memory, test_case.final);
    result.cycle_count_ok = cpu->ticks == test_case.cycles.size();

    M6502_delete(cpu);
    return result;
}

} // namespace

namespace sflib6502_toolbox {

benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus)
{
    benchmark6502::singlestep_result result;
    result.core_name = "sflib6502";
    result.model_name = "NMOS";

    scoped_stderr_silence silence;

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

} // namespace sflib6502_toolbox
