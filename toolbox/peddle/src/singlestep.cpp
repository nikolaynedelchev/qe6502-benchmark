#include <peddle_toolbox.hpp>

#include <Peddle.h>

#include <array>
#include <cstdint>
#include <vector>

namespace {

class harness_cpu final : public peddle::Peddle {
public:
    std::array<std::uint8_t, 0x10000> memory{};
    std::vector<benchmark6502::singlestep_cycle> trace;
    bool jammed = false;

    harness_cpu()
    {
        setModel(peddle::MOS_6502);
    }

    void set_case_state(const benchmark6502::singlestep_cpu_state& state)
    {
        reset();
        reg.pc = state.pc;
        reg.pc0 = state.pc;
        reg.sp = state.s;
        reg.a = state.a;
        reg.x = state.x;
        reg.y = state.y;
        setP(state.p);

        reg.adl = 0;
        reg.adh = 0;
        reg.idl = 0;
        reg.d = 0;
        reg.ovl = false;
        reg.pport.data = 0xff;
        reg.pport.direction = 0;

        clock = 0;
        next = peddle::fetch;
        flags = 0;
        rdyLine = true;
        rdyLineUp = 0;
        rdyLineDown = 0;
        nmiLine = 0;
        irqLine = 0;
        edgeDetector.reset(0);
        levelDetector.reset(0);
        doNmi = false;
        doIrq = false;
        jammed = false;
    }

    peddle::u8 read(const peddle::u16 address) override
    {
        const std::uint8_t value = memory[address];
        trace.push_back({address, value, benchmark6502::singlestep_memory_operation::read});
        return value;
    }

    void write(const peddle::u16 address, const peddle::u8 value) override
    {
        memory[address] = value;
        trace.push_back({address, value, benchmark6502::singlestep_memory_operation::write});
    }

    peddle::u8 readDasm(const peddle::u16 address) const override
    {
        return memory[address];
    }

protected:
    void cpuDidJam() override
    {
        jammed = true;
    }
};

struct case_result {
    bool instruction_ok = false;
    bool cycle_count_ok = false;
    bool bus_trace_ok = false;
};

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
    if (cpu.reg.pc != expected.pc ||
        cpu.reg.sp != expected.s ||
        cpu.reg.a != expected.a ||
        cpu.reg.x != expected.x ||
        cpu.reg.y != expected.y ||
        cpu.getP() != expected.p) {
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
    harness_cpu cpu;
    load_ram(cpu.memory, test_case.initial.ram);
    cpu.trace.reserve(test_case.cycles.size() + 8u);
    cpu.set_case_state(test_case.initial);
    cpu.trace.clear();

    const std::size_t max_cycles = test_case.cycles.size() + 16u;
    std::size_t cycles = 0;
    do {
        cpu.execute();
        ++cycles;
    } while (!cpu.inFetchPhase() && cycles < max_cycles);

    case_result result;
    const bool complete = cpu.inFetchPhase() || cpu.jammed;
    result.instruction_ok = complete && compare_final_state(cpu, cpu.memory, test_case.final);
    result.cycle_count_ok = complete && cycles == test_case.cycles.size();
    result.bus_trace_ok = complete && compare_trace(cpu.trace, test_case.cycles);
    return result;
}

} // namespace

namespace peddle_toolbox {

benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus)
{
    benchmark6502::singlestep_result result;
    result.core_name = "Peddle";
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

} // namespace peddle_toolbox
