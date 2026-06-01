#include <applewin_toolbox.hpp>

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace {

struct case_result {
    bool instruction_ok = false;
    bool cycle_count_ok = false;
};

void load_ram(std::array<std::uint8_t, 0x10000>& memory, const std::vector<benchmark6502::singlestep_memory_cell>& ram)
{
    for (const auto& cell : ram) {
        memory[cell.address] = cell.value;
    }
}

void set_initial_state(const benchmark6502::singlestep_cpu_state& state, const applewin_toolbox::CpuMode mode)
{
    applewin_toolbox::set_state(applewin_toolbox::State{
        state.a,
        state.x,
        state.y,
        state.p,
        state.pc,
        static_cast<std::uint16_t>(0x0100u | state.s),
        false,
        mode,
    });
    applewin_toolbox::clear_interrupt_lines();
    applewin_toolbox::clear_cumulative_cycles();
}

bool compare_final_state(const std::array<std::uint8_t, 0x10000>& memory,
                         const benchmark6502::singlestep_cpu_state& expected)
{
    if (applewin_toolbox::pc() != expected.pc ||
        static_cast<std::uint8_t>(applewin_toolbox::sp()) != expected.s ||
        applewin_toolbox::a() != expected.a ||
        applewin_toolbox::x() != expected.x ||
        applewin_toolbox::y() != expected.y ||
        ((applewin_toolbox::p() ^ expected.p) & static_cast<std::uint8_t>(~0x10u)) != 0u) {
        return false;
    }

    for (const auto& cell : expected.ram) {
        if (memory[cell.address] != cell.value) {
            return false;
        }
    }
    return true;
}

case_result run_case(const benchmark6502::singlestep_case& test_case, const applewin_toolbox::CpuMode mode)
{
    std::array<std::uint8_t, 0x10000> memory{};
    load_ram(memory, test_case.initial.ram);
    applewin_toolbox::attach_memory(memory.data(), static_cast<std::uint32_t>(memory.size()));
    set_initial_state(test_case.initial, mode);

    const std::uint32_t cycles = applewin_toolbox::execute(static_cast<std::uint32_t>(test_case.cycles.size()));

    case_result result;
    result.instruction_ok = compare_final_state(memory, test_case.final);
    result.cycle_count_ok = cycles == test_case.cycles.size();
    return result;
}

benchmark6502::singlestep_result run_singlestep_corpus(const benchmark6502::singlestep_corpus& corpus,
                                                   const std::string& display_model_name,
                                                   const applewin_toolbox::CpuMode mode,
                                                   const char* const cpu_init_model)
{
    benchmark6502::singlestep_result result;
    result.core_name = "applewin";
    result.corpus_model = corpus.model;
    result.model_name = display_model_name;
    result.cpu_init_model = cpu_init_model;

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
            const case_result single = run_case(test_case, mode);
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

} // namespace

namespace applewin_toolbox {

benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_corpus(corpus, "NMOS 6502", CpuMode::nmos6502, "CpuMode::nmos6502");
}

benchmark6502::singlestep_result run_singlestep_wdc65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_corpus(corpus, "CMOS 65C02/WDC corpus", CpuMode::cmos65c02, "CpuMode::cmos65c02");
}

benchmark6502::singlestep_result run_singlestep_rockwell65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_corpus(corpus, "CMOS 65C02/Rockwell corpus", CpuMode::cmos65c02, "CpuMode::cmos65c02");
}

benchmark6502::singlestep_result run_singlestep_synertek65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_corpus(corpus, "CMOS 65C02/Synertek-ST corpus", CpuMode::cmos65c02, "CpuMode::cmos65c02");
}

} // namespace applewin_toolbox
