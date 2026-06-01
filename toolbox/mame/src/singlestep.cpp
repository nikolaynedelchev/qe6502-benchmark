#include <mame_toolbox.hpp>

#include <array>
#include <cstdint>
#include <vector>

namespace {

struct case_result { bool instruction_ok = false; bool cycle_count_ok = false; };

void load_ram(benchmark6502::mame::Mame6502Cpu& cpu, const std::vector<benchmark6502::singlestep_memory_cell>& ram)
{
    cpu.clear_memory(0);
    for (const auto& cell : ram) {
        cpu.write_memory(cell.address, cell.value);
    }
}

void set_initial_state(benchmark6502::mame::Mame6502Cpu& cpu, const benchmark6502::singlestep_cpu_state& state)
{
    cpu.set_state(benchmark6502::mame::CpuState{state.a, state.x, state.y, state.p, state.s, state.pc, false});
    cpu.reset_cycle_counter();
    cpu.clear_irq_line();
    cpu.set_nmi_line(false);
}

bool compare_final_state(const benchmark6502::mame::Mame6502Cpu& cpu,
                         const benchmark6502::singlestep_cpu_state& expected)
{
    if (cpu.pc() != expected.pc ||
        cpu.sp() != expected.s ||
        cpu.a() != expected.a ||
        cpu.x() != expected.x ||
        cpu.y() != expected.y ||
        cpu.p() != expected.p) {
        return false;
    }
    for (const auto& cell : expected.ram) {
        if (cpu.read_memory(cell.address) != cell.value) {
            return false;
        }
    }
    return true;
}

case_result run_case(const benchmark6502::singlestep_case& test_case)
{
    benchmark6502::mame::Mame6502Cpu cpu(benchmark6502::mame::CpuMode::nmos6502);
    load_ram(cpu, test_case.initial.ram);
    set_initial_state(cpu, test_case.initial);
    const unsigned cycles = cpu.execute(static_cast<unsigned>(test_case.cycles.size()));
    return {compare_final_state(cpu, test_case.final), cycles == test_case.cycles.size()};
}

} // namespace

namespace mame_toolbox {

benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus)
{
    benchmark6502::singlestep_result result;
    result.core_name = "mame";
    result.model_name = "NMOS 6502";
    result.cpu_init_model = "CpuMode::nmos6502";
    result.corpus_model = corpus.model;

    for (unsigned opcode_value = 0; opcode_value <= 0xffu; ++opcode_value) {
        const auto& tests = corpus.opcodes[opcode_value];
        auto& opcode_result = result.opcodes[opcode_value];
        opcode_result.opcode = static_cast<std::uint8_t>(opcode_value);
        opcode_result.cases_run = static_cast<std::uint64_t>(tests.cases.size());
        opcode_result.instruction.supported = true;
        opcode_result.cycle_count.supported = true;
        opcode_result.bus_trace.supported = false;
        for (const auto& test_case : tests.cases) {
            const case_result single = run_case(test_case);
            if (!single.instruction_ok) { opcode_result.instruction.failed = true; opcode_result.instruction.failed_cases++; }
            if (!single.cycle_count_ok) { opcode_result.cycle_count.failed = true; opcode_result.cycle_count.failed_cases++; }
        }
    }
    return result;
}

} // namespace mame_toolbox
