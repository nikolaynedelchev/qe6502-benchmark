#include <clk_toolbox.hpp>

#include <Processors/6502/6502.hpp>

#include <array>
#include <cstdint>
#include <vector>

namespace {

bool is_nmos_kil_opcode(const std::uint8_t opcode)
{
    switch (opcode) {
    case 0x02u: case 0x12u: case 0x22u: case 0x32u:
    case 0x42u: case 0x52u: case 0x62u: case 0x72u:
    case 0x92u: case 0xB2u: case 0xD2u: case 0xF2u:
        return true;
    default:
        return false;
    }
}

struct case_result {
    bool instruction_ok = false;
    bool cycle_count_ok = false;
    bool bus_trace_ok = false;
};

class singlestep_bus : public CPU::MOS6502::BusHandler {
public:
    using AddressType = std::uint16_t;

    std::array<std::uint8_t, 65536> memory{};
    std::vector<benchmark6502::singlestep_cycle> cycles;

    Cycles perform_bus_operation(const CPU::MOS6502::BusOperation operation,
                                 const AddressType address,
                                 std::uint8_t* const value)
    {
        const bool is_read = CPU::MOS6502Esque::is_read(operation);
        const bool is_write = CPU::MOS6502Esque::is_write(operation);
        if (is_read) {
            *value = memory[address];
        }
        cycles.push_back({address,
                          is_write ? *value : memory[address],
                          is_write ? benchmark6502::singlestep_memory_operation::write
                                   : benchmark6502::singlestep_memory_operation::read});
        if (is_write) {
            memory[address] = *value;
        }
        return Cycles(1);
    }
};

template <CPU::MOS6502::Personality personality>
using processor_for = CPU::MOS6502::Processor<personality, singlestep_bus, false>;

using processor_type = processor_for<CPU::MOS6502::P6502>;

void load_ram(std::array<std::uint8_t, 0x10000>& memory, const std::vector<benchmark6502::singlestep_memory_cell>& ram)
{
    for (const auto& cell : ram) {
        memory[cell.address] = cell.value;
    }
}

template <typename Processor>
void set_initial_state(Processor& cpu, const benchmark6502::singlestep_cpu_state& state)
{
    cpu.set_power_on(false);
    cpu.set_reset_line(false);
    cpu.set_value_of(CPU::MOS6502::Register::ProgramCounter, state.pc);
    cpu.set_value_of(CPU::MOS6502::Register::StackPointer, state.s);
    cpu.set_value_of(CPU::MOS6502::Register::A, state.a);
    cpu.set_value_of(CPU::MOS6502::Register::X, state.x);
    cpu.set_value_of(CPU::MOS6502::Register::Y, state.y);
    cpu.set_value_of(CPU::MOS6502::Register::Flags, state.p);
    cpu.restart_operation_fetch();
}

template <typename Processor>
bool compare_final_state(Processor& cpu,
                         const std::array<std::uint8_t, 0x10000>& memory,
                         const benchmark6502::singlestep_cpu_state& expected,
                         const bool consumed_following_opcode_fetch)
{
    const auto actual_pc = static_cast<std::uint16_t>(cpu.value_of(CPU::MOS6502::Register::ProgramCounter) - (consumed_following_opcode_fetch ? 1u : 0u));
    if (actual_pc != expected.pc ||
        cpu.value_of(CPU::MOS6502::Register::StackPointer) != expected.s ||
        cpu.value_of(CPU::MOS6502::Register::A) != expected.a ||
        cpu.value_of(CPU::MOS6502::Register::X) != expected.x ||
        cpu.value_of(CPU::MOS6502::Register::Y) != expected.y ||
        ((cpu.value_of(CPU::MOS6502::Register::Flags) ^ expected.p) & 0xefu) != 0u) {
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

template <CPU::MOS6502::Personality personality>
case_result run_case_for_model(const benchmark6502::singlestep_case& test_case, const std::uint8_t opcode)
{
    singlestep_bus bus;
    load_ram(bus.memory, test_case.initial.ram);
    processor_for<personality> cpu(bus);
    set_initial_state(cpu, test_case.initial);

    const bool kil_opcode = personality == CPU::MOS6502::P6502 && is_nmos_kil_opcode(opcode);
    const std::size_t expected_cycles = kil_opcode && test_case.cycles.size() > 2u ? 2u : test_case.cycles.size();
    const bool consume_following_opcode_fetch = !kil_opcode;
    cpu.run_for(Cycles(static_cast<int>(expected_cycles + (consume_following_opcode_fetch ? 1u : 0u))));
    if (consume_following_opcode_fetch && bus.cycles.size() > expected_cycles) {
        bus.cycles.resize(expected_cycles);
    }

    case_result result;
    result.instruction_ok = compare_final_state(cpu, bus.memory, test_case.final, consume_following_opcode_fetch);
    result.cycle_count_ok = bus.cycles.size() == expected_cycles;
    result.bus_trace_ok = result.cycle_count_ok;
    if (result.bus_trace_ok) {
        for (std::size_t i = 0; i < expected_cycles; ++i) {
            if (!compare_cycle(bus.cycles[i], test_case.cycles[i])) {
                result.bus_trace_ok = false;
                break;
            }
        }
    }
    return result;
}

} // namespace

namespace clk_toolbox {

template <CPU::MOS6502::Personality personality>
benchmark6502::singlestep_result run_singlestep_model(const benchmark6502::singlestep_corpus& corpus,
                                                       const char* const model_name,
                                                       const char* const cpu_init_model)
{
    benchmark6502::singlestep_result result;
    result.core_name = "clk";
    result.model_name = model_name;
    result.cpu_init_model = cpu_init_model;
    result.corpus_model = corpus.model;

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
            const case_result single = run_case_for_model<personality>(test_case, opcode);
            if (!single.instruction_ok) { opcode_result.instruction.failed = true; opcode_result.instruction.failed_cases++; }
            if (!single.cycle_count_ok) { opcode_result.cycle_count.failed = true; opcode_result.cycle_count.failed_cases++; }
            if (!single.bus_trace_ok) { opcode_result.bus_trace.failed = true; opcode_result.bus_trace.failed_cases++; }
        }
    }
    return result;
}

benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_model<CPU::MOS6502::P6502>(corpus, "P6502 NMOS", "CPU::MOS6502::P6502");
}

benchmark6502::singlestep_result run_singlestep_wdc65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_model<CPU::MOS6502::PWDC65C02>(corpus, "WDC 65C02", "CPU::MOS6502::PWDC65C02");
}

benchmark6502::singlestep_result run_singlestep_rockwell65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_model<CPU::MOS6502::PRockwell65C02>(corpus, "Rockwell 65C02", "CPU::MOS6502::PRockwell65C02");
}

benchmark6502::singlestep_result run_singlestep_synertek65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_model<CPU::MOS6502::PSynertek65C02>(corpus, "Synertek/ST 65C02", "CPU::MOS6502::PSynertek65C02");
}

} // namespace clk_toolbox
