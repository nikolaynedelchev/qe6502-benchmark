#include <ares_toolbox.hpp>

#include <ares/ares.hpp>
#include <component/processor/mos6502/mos6502.hpp>

#include <array>
#include <cstdint>
#include <vector>

namespace {

struct case_result { bool instruction_ok = false; bool cycle_count_ok = false; };

class singlestep_cpu final : public ares::MOS6502 {
public:
    std::array<std::uint8_t, 65536> memory{};
    std::uint64_t bus_cycles = 0;

    auto read(n16 address) -> n8 override { ++bus_cycles; return memory[static_cast<std::uint16_t>(address)]; }
    auto write(n16 address, n8 data) -> void override { ++bus_cycles; memory[static_cast<std::uint16_t>(address)] = static_cast<std::uint8_t>(data); }
};

void load_ram(std::array<std::uint8_t, 0x10000>& memory, const std::vector<benchmark6502::singlestep_memory_cell>& ram)
{
    for (const auto& cell : ram) memory[cell.address] = cell.value;
}

void set_initial_state(singlestep_cpu& cpu, const benchmark6502::singlestep_cpu_state& state)
{
    cpu.power();
    cpu.BCD = 1;
    cpu.PC = state.pc;
    cpu.S = state.s;
    cpu.A = state.a;
    cpu.X = state.x;
    cpu.Y = state.y;
    cpu.P = state.p;
    cpu.bus_cycles = 0;
}

bool compare_final_state(const singlestep_cpu& cpu,
                         const std::array<std::uint8_t, 0x10000>& memory,
                         const benchmark6502::singlestep_cpu_state& expected)
{
    if (static_cast<std::uint16_t>(cpu.PC) != expected.pc ||
        static_cast<std::uint8_t>(cpu.S) != expected.s ||
        static_cast<std::uint8_t>(cpu.A) != expected.a ||
        static_cast<std::uint8_t>(cpu.X) != expected.x ||
        static_cast<std::uint8_t>(cpu.Y) != expected.y ||
        static_cast<std::uint8_t>(cpu.P) != expected.p) return false;
    for (const auto& cell : expected.ram) if (memory[cell.address] != cell.value) return false;
    return true;
}

case_result run_case(const benchmark6502::singlestep_case& test_case)
{
    singlestep_cpu cpu;
    load_ram(cpu.memory, test_case.initial.ram);
    set_initial_state(cpu, test_case.initial);
    cpu.instruction();
    return {compare_final_state(cpu, cpu.memory, test_case.final), cpu.bus_cycles == test_case.cycles.size()};
}

} // namespace

namespace ares_toolbox {

benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus)
{
    benchmark6502::singlestep_result result;
    result.core_name = "ares";
    result.model_name = "MOS6502 NMOS/BCD";
    result.cpu_init_model = "ares::MOS6502 BCD=1";
    result.corpus_model = corpus.model;
    for (unsigned opcode_value = 0; opcode_value <= 0xffu; ++opcode_value) {
        const auto opcode = static_cast<std::uint8_t>(opcode_value);
        (void)opcode;
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

} // namespace ares_toolbox
