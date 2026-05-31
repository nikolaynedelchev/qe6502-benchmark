#include <qe6502_toolbox.hpp>

#include <qe6502/cpu.hpp>

#include <array>
#include <exception>
#include <sstream>
#include <vector>

namespace {

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

void set_initial_state(qe6502::cpu& cpu, const benchmark6502::singlestep_cpu_state& state)
{
    cpu.pc(state.pc);
    cpu.s(state.s);
    cpu.a(state.a);
    cpu.x(state.x);
    cpu.y(state.y);
    cpu.p(state.p);
    cpu.jump_to(cpu.pc());
}

bool is_nmos_kil_opcode(const std::uint8_t opcode)
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
    case 0xB2u:
    case 0xD2u:
    case 0xF2u:
        return true;
    default:
        return false;
    }
}

bool compare_final_state(const qe6502::cpu& cpu,
                         const std::array<std::uint8_t, 0x10000>& memory,
                         const benchmark6502::singlestep_cpu_state& expected)
{
    if (cpu.pc() != expected.pc ||
        cpu.s() != expected.s ||
        cpu.a() != expected.a ||
        cpu.x() != expected.x ||
        cpu.y() != expected.y ||
        cpu.p() != expected.p) {
        return false;
    }

    for (const auto& cell : expected.ram) {
        if (memory[cell.address] != cell.value) {
            return false;
        }
    }
    return true;
}

bool compare_cycle(const benchmark6502::singlestep_cycle& actual, const benchmark6502::singlestep_cycle& expected)
{
    return actual.address == expected.address &&
           actual.value == expected.value &&
           actual.operation == expected.operation;
}

case_result run_case(const benchmark6502::singlestep_case& test_case, const std::uint8_t opcode)
{
    std::array<std::uint8_t, 0x10000> memory{};
    load_ram(memory, test_case.initial.ram);

    qe6502::cpu cpu{qe6502::model::nmos};
    set_initial_state(cpu, test_case.initial);

    const bool kil_opcode = is_nmos_kil_opcode(opcode);
    const std::size_t expected_cycle_count = kil_opcode && test_case.cycles.size() > 2u
        ? 2u
        : test_case.cycles.size();
    const std::size_t max_cycles = expected_cycle_count + 64u;

    std::vector<benchmark6502::singlestep_cycle> actual_cycles;
    actual_cycles.reserve(expected_cycle_count == 0u ? 1u : expected_cycle_count);

    for (std::size_t cycle = 0; cycle < max_cycles; ++cycle) {
        const bool write = cpu.is_write();
        const std::uint16_t address = cpu.bus_address();
        const std::uint8_t bus_value = write ? cpu.bus_data() : memory[address];
        actual_cycles.push_back({address,
                                 bus_value,
                                 write ? benchmark6502::singlestep_memory_operation::write
                                       : benchmark6502::singlestep_memory_operation::read});

        if (write) {
            memory[address] = bus_value;
        }

        cpu.tick(bus_value);

        if (kil_opcode && actual_cycles.size() >= expected_cycle_count) {
            break;
        }
        if (!kil_opcode && cpu.is_opcode_fetch()) {
            break;
        }
    }

    case_result result;
    result.instruction_ok = compare_final_state(cpu, memory, test_case.final);
    result.cycle_count_ok = actual_cycles.size() == expected_cycle_count;
    result.bus_trace_ok = result.cycle_count_ok;
    if (result.bus_trace_ok) {
        for (std::size_t i = 0; i < expected_cycle_count; ++i) {
            if (!compare_cycle(actual_cycles[i], test_case.cycles[i])) {
                result.bus_trace_ok = false;
                break;
            }
        }
    }
    return result;
}

} // namespace

namespace qe6502_toolbox {

benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus)
{
    benchmark6502::singlestep_result result;
    result.core_name = "qe6502";
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
            const case_result single = run_case(test_case, opcode);
            if (!single.instruction_ok) {
                opcode_result.instruction.failed = true;
            }
            if (!single.cycle_count_ok) {
                opcode_result.cycle_count.failed = true;
            }
            if (!single.bus_trace_ok) {
                opcode_result.bus_trace.failed = true;
            }
        }
    }

    return result;
}

} // namespace qe6502_toolbox
