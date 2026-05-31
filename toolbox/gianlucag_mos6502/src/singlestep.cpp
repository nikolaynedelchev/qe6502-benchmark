#include <gianlucag_mos6502_toolbox.hpp>

#include <mos6502.h>

#include <array>
#include <cstdint>
#include <cstring>
#include <vector>

namespace {

std::array<std::uint8_t, 65536> memory{};

std::uint8_t read_byte(const std::uint16_t address)
{
    return memory[address];
}

void write_byte(const std::uint16_t address, const std::uint8_t value)
{
    memory[address] = value;
}

struct case_result {
    bool instruction_ok = false;
    bool cycle_count_ok = false;
};

void load_ram(const std::vector<benchmark6502::singlestep_memory_cell>& ram)
{
    memory.fill(0x00);
    for (const auto& cell : ram) {
        memory[cell.address] = cell.value;
    }
}

void set_initial_state(mos6502& cpu, const benchmark6502::singlestep_cpu_state& initial)
{
    cpu.Reset();
    cpu.SetPC(initial.pc);
    cpu.SetS(initial.s);
    cpu.SetA(initial.a);
    cpu.SetX(initial.x);
    cpu.SetY(initial.y);
    cpu.SetP(initial.p);
}

bool compare_final_state(mos6502& cpu, const benchmark6502::singlestep_cpu_state& expected)
{
    if (cpu.GetPC() != expected.pc ||
        cpu.GetS() != expected.s ||
        cpu.GetA() != expected.a ||
        cpu.GetX() != expected.x ||
        cpu.GetY() != expected.y ||
        cpu.GetP() != expected.p) {
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
    load_ram(test_case.initial.ram);

    mos6502 cpu(read_byte, write_byte);
    set_initial_state(cpu, test_case.initial);

    std::uint64_t cycles = 0;
    cpu.Run(1, cycles, mos6502::INST_COUNT);

    case_result result;
    result.instruction_ok = compare_final_state(cpu, test_case.final);
    result.cycle_count_ok = cycles == test_case.cycles.size();
    return result;
}

} // namespace

namespace gianlucag_mos6502_toolbox {

benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus)
{
    benchmark6502::singlestep_result result;
    result.core_name = "gianlucag_mos6502";
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

} // namespace gianlucag_mos6502_toolbox
