#include <o2_toolbox.hpp>

#include <O2.hpp>

#include <array>
#include <cstdint>
#include <vector>

namespace {

struct memory_context {
    std::array<std::uint8_t, 0x10000>* memory = nullptr;
    std::vector<benchmark6502::singlestep_cycle>* trace = nullptr;
};

class harness_cpu : public O2::CPU {
  public:
    using O2::CPU::CPU;

    void set_case_state(const benchmark6502::singlestep_cpu_state& state)
    {
        PC = state.pc;
        A = state.a;
        X = state.x;
        Y = state.y;
        S = state.s;
        set_status(state.p);

        instruction_cycles = 0;
        tsc = 0;
        this->state = O2::FETCH;

        cold = false;
        addm = O2::IMPLIED;
        iflag = O2::INONE;
        rnmi = false;
        rirq = false;
        rbrk = false;
        cbrk = false;
        intc = false;
        hlt = false;
        cycle_count = 0;
        opcode = 0;
        wv0 = 0;
        wv1 = 0;
        page = 0;
        av = 0;
    }

    std::uint8_t status() const
    {
        return static_cast<std::uint8_t>((P[O2::CARRY] ? 0x01u : 0u) |
                                         (P[O2::ZERO] ? 0x02u : 0u) |
                                         (P[O2::INTERRUPT] ? 0x04u : 0u) |
                                         (P[O2::DECIMAL] ? 0x08u : 0u) |
                                         (P[O2::B0] ? 0x10u : 0u) |
                                         (P[O2::B1] ? 0x20u : 0u) |
                                         (P[O2::OVERFLOW_FLAG] ? 0x40u : 0u) |
                                         (P[O2::NEGATIVE] ? 0x80u : 0u));
    }

    bool complete() const
    {
        return hlt || (state == O2::FETCH && !intc);
    }

  private:
    void set_status(const std::uint8_t value)
    {
        P[O2::CARRY] = (value & 0x01u) != 0u;
        P[O2::ZERO] = (value & 0x02u) != 0u;
        P[O2::INTERRUPT] = (value & 0x04u) != 0u;
        P[O2::DECIMAL] = (value & 0x08u) != 0u;
        P[O2::B0] = (value & 0x10u) != 0u;
        P[O2::B1] = (value & 0x20u) != 0u;
        P[O2::OVERFLOW_FLAG] = (value & 0x40u) != 0u;
        P[O2::NEGATIVE] = (value & 0x80u) != 0u;
    }
};

struct case_result {
    bool instruction_ok = false;
    bool cycle_count_ok = false;
    bool bus_trace_ok = false;
};

thread_local memory_context* g_context = nullptr;

std::uint8_t memory_read(const std::uint16_t address)
{
    const std::uint8_t value = (*g_context->memory)[address];
    g_context->trace->push_back({address, value, benchmark6502::singlestep_memory_operation::read});
    return value;
}

void memory_write(const std::uint16_t address, const std::uint8_t value)
{
    (*g_context->memory)[address] = value;
    g_context->trace->push_back({address, value, benchmark6502::singlestep_memory_operation::write});
}

void load_ram(std::array<std::uint8_t, 0x10000>& memory, const std::vector<benchmark6502::singlestep_memory_cell>& ram)
{
    for (const auto& cell : ram) {
        memory[cell.address] = cell.value;
    }
}

bool compare_trace(const std::vector<benchmark6502::singlestep_cycle>& actual,
                   const std::vector<benchmark6502::singlestep_cycle>& expected)
{
    if (actual.size() != expected.size()) {
        return false;
    }
    for (std::size_t i = 0; i < expected.size(); ++i) {
        if (actual[i].address != expected[i].address ||
            actual[i].value != expected[i].value ||
            actual[i].operation != expected[i].operation) {
            return false;
        }
    }
    return true;
}

bool compare_final_state(const harness_cpu& cpu,
                         const std::array<std::uint8_t, 0x10000>& memory,
                         const benchmark6502::singlestep_cpu_state& expected)
{
    if (cpu.PC != expected.pc ||
        cpu.S != expected.s ||
        cpu.A != expected.a ||
        cpu.X != expected.x ||
        cpu.Y != expected.y ||
        cpu.status() != expected.p) {
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
    std::array<std::uint8_t, 0x10000> memory{};
    load_ram(memory, test_case.initial.ram);

    std::vector<benchmark6502::singlestep_cycle> trace;
    trace.reserve(test_case.cycles.size() + 8u);

    memory_context context{&memory, &trace};
    g_context = &context;

    harness_cpu cpu(memory_read, memory_write);
    cpu.set_case_state(test_case.initial);

    const std::size_t max_cycles = test_case.cycles.size() + 16u;
    std::size_t cycles = 0;
    do {
        cpu.cycle();
        ++cycles;
    } while (!cpu.complete() && cycles < max_cycles);

    case_result result;
    result.instruction_ok = cpu.complete() && compare_final_state(cpu, memory, test_case.final);
    result.cycle_count_ok = cpu.complete() && cycles == test_case.cycles.size();
    result.bus_trace_ok = cpu.complete() && compare_trace(trace, test_case.cycles);

    g_context = nullptr;
    return result;
}

} // namespace

namespace o2_toolbox {

benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus)
{
    benchmark6502::singlestep_result result;
    result.core_name = "O2";
    result.model_name = "NMOS";

    for (unsigned opcode_value = 0; opcode_value <= 0xffu; ++opcode_value) {
        const auto opcode = static_cast<std::uint8_t>(opcode_value);
        const benchmark6502::singlestep_opcode_tests& tests = corpus.opcodes[opcode_value];
        benchmark6502::singlestep_opcode_result& opcode_result = result.opcodes[opcode_value];
        opcode_result.opcode = opcode;
        opcode_result.cases_run = static_cast<std::uint64_t>(tests.cases.size());
        opcode_result.instruction.supported = true;
        opcode_result.cycle_count.supported = true;
        opcode_result.bus_trace.supported = true;

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
            if (!single.bus_trace_ok) {
                opcode_result.bus_trace.failed = true;
                opcode_result.bus_trace.failed_cases++;
            }
        }
    }

    return result;
}

} // namespace o2_toolbox
