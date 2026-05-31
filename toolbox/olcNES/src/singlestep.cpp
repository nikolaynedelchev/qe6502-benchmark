#include <olcNES_toolbox.hpp>

#include <Bus.h>

#include <cstdint>
#include <vector>

namespace {

struct case_result {
    bool instruction_ok = false;
    bool cycle_count_ok = false;
};

void load_ram(Bus& bus, const std::vector<benchmark6502::singlestep_memory_cell>& ram)
{
    bus.ram.fill(0x00);
    for (const auto& cell : ram) {
        bus.ram[cell.address] = cell.value;
    }
}

void set_initial_state(olc6502& cpu, const benchmark6502::singlestep_cpu_state& initial)
{
    cpu.a = initial.a;
    cpu.x = initial.x;
    cpu.y = initial.y;
    cpu.stkp = initial.s;
    cpu.pc = initial.pc;
    cpu.status = initial.p;
}

bool compare_final_state(const Bus& bus, const benchmark6502::singlestep_cpu_state& expected)
{
    const olc6502& cpu = bus.cpu;
    if (cpu.pc != expected.pc ||
        cpu.stkp != expected.s ||
        cpu.a != expected.a ||
        cpu.x != expected.x ||
        cpu.y != expected.y ||
        cpu.status != expected.p) {
        return false;
    }

    for (const auto& cell : expected.ram) {
        if (bus.ram[cell.address] != cell.value) {
            return false;
        }
    }
    return true;
}

std::uint64_t run_one_instruction(olc6502& cpu)
{
    std::uint64_t cycles = 0;
    do {
        cpu.clock();
        ++cycles;
    } while (!cpu.complete());
    return cycles;
}

case_result run_case(const benchmark6502::singlestep_case& test_case)
{
    Bus bus;
    load_ram(bus, test_case.initial.ram);
    set_initial_state(bus.cpu, test_case.initial);

    const std::uint64_t cycles = run_one_instruction(bus.cpu);

    case_result result;
    result.instruction_ok = compare_final_state(bus, test_case.final);
    result.cycle_count_ok = cycles == test_case.cycles.size();
    return result;
}

} // namespace

namespace olcNES_toolbox {

benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus)
{
    benchmark6502::singlestep_result result;
    result.core_name = "olcNES";
    result.model_name = "NMOS";

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

} // namespace olcNES_toolbox
