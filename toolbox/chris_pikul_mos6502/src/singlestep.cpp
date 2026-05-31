#include <chris_pikul_mos6502_toolbox.hpp>

#include <bus.h>
#include <cpu.h>
#include <memory.h>

#include <cstdint>
#include <cstdio>
#include <memory>
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

class singlestep_cpu final : public mos6502::CPU {
public:
    using mos6502::CPU::CPU;

    void set_state(const benchmark6502::singlestep_cpu_state& initial)
    {
        m_PC = initial.pc;
        m_SP = initial.s;
        m_Acc = initial.a;
        m_X = initial.x;
        m_Y = initial.y;
        m_ProcStatus = initial.p;
        m_CyclesRem = 0;
        m_CyclesExecuted = 0;
        ClearSupplied();
    }

    std::uint64_t run_one_instruction()
    {
        std::uint64_t cycles = 0;
        do {
            ++cycles;
        } while (Tick());
        return cycles;
    }
};

void load_ram(mos6502::Memory& memory, const std::vector<benchmark6502::singlestep_memory_cell>& ram)
{
    memory.Clear(0x00);
    for (const auto& cell : ram) {
        memory[cell.address] = cell.value;
    }
}

bool compare_final_state(const singlestep_cpu& cpu,
                         const mos6502::Memory& memory,
                         const benchmark6502::singlestep_cpu_state& expected)
{
    if (cpu.GetProgramCounter() != expected.pc ||
        cpu.GetStackPointer() != expected.s ||
        cpu.GetAccumulator() != expected.a ||
        cpu.GetX() != expected.x ||
        cpu.GetY() != expected.y ||
        cpu.GetStatus().value != expected.p) {
        return false;
    }

    for (const auto& cell : expected.ram) {
        if (memory.begin()[cell.address] != cell.value) {
            return false;
        }
    }
    return true;
}

case_result run_case(const benchmark6502::singlestep_case& test_case)
{
    const auto memory = std::make_shared<mos6502::Memory>();
    const auto bus = std::make_shared<mos6502::Bus>(memory);
    singlestep_cpu cpu(bus);

    load_ram(*memory, test_case.initial.ram);
    cpu.set_state(test_case.initial);

    const std::uint64_t cycles = cpu.run_one_instruction();

    case_result result;
    result.instruction_ok = compare_final_state(cpu, *memory, test_case.final);
    result.cycle_count_ok = cycles == test_case.cycles.size();
    return result;
}

} // namespace

namespace chris_pikul_mos6502_toolbox {

benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus)
{
    benchmark6502::singlestep_result result;
    result.core_name = "chris-pikul/mos6502";
    result.model_name = "NMOS";

    const scoped_stderr_silence silence;

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

} // namespace chris_pikul_mos6502_toolbox
