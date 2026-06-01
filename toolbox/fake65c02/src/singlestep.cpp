#include <fake65c02_toolbox.hpp>

#include <array>
#include <cstdint>
#include <string>
#include <vector>

extern "C" {
void fake65c02_singlestep_clear_memory();
void fake65c02_singlestep_write_memory(unsigned short address, unsigned char value);
unsigned char fake65c02_singlestep_read_memory(unsigned short address);
void fake65c02_singlestep_set_state(unsigned short pc,
                                    unsigned char s,
                                    unsigned char a,
                                    unsigned char x,
                                    unsigned char y,
                                    unsigned char p);
void fake65c02_singlestep_get_state(unsigned short* pc,
                                    unsigned char* s,
                                    unsigned char* a,
                                    unsigned char* x,
                                    unsigned char* y,
                                    unsigned char* p);
unsigned int fake65c02_singlestep_step();
}

namespace {

struct case_result {
    bool instruction_ok = false;
    bool cycle_count_ok = false;
};

void load_ram(const std::vector<benchmark6502::singlestep_memory_cell>& ram)
{
    for (const auto& cell : ram) {
        fake65c02_singlestep_write_memory(cell.address, cell.value);
    }
}

void set_initial_state(const benchmark6502::singlestep_cpu_state& state)
{
    fake65c02_singlestep_set_state(state.pc, state.s, state.a, state.x, state.y, state.p);
}

bool compare_final_state(const benchmark6502::singlestep_cpu_state& expected)
{
    unsigned short pc = 0;
    unsigned char s = 0;
    unsigned char a = 0;
    unsigned char x = 0;
    unsigned char y = 0;
    unsigned char p = 0;
    fake65c02_singlestep_get_state(&pc, &s, &a, &x, &y, &p);

    if (pc != expected.pc ||
        s != expected.s ||
        a != expected.a ||
        x != expected.x ||
        y != expected.y ||
        p != expected.p) {
        return false;
    }

    for (const auto& cell : expected.ram) {
        if (fake65c02_singlestep_read_memory(cell.address) != cell.value) {
            return false;
        }
    }
    return true;
}

case_result run_case(const benchmark6502::singlestep_case& test_case)
{
    fake65c02_singlestep_clear_memory();
    load_ram(test_case.initial.ram);
    set_initial_state(test_case.initial);

    const unsigned int cycles = fake65c02_singlestep_step();

    case_result result;
    result.instruction_ok = compare_final_state(test_case.final);
    result.cycle_count_ok = cycles == test_case.cycles.size();
    return result;
}

benchmark6502::singlestep_result run_singlestep_65c02_corpus(const benchmark6502::singlestep_corpus& corpus,
                                                              const std::string& display_model_name)
{
    benchmark6502::singlestep_result result;
    result.core_name = "fake65c02";
    result.corpus_model = corpus.model;
    result.model_name = display_model_name;
    result.cpu_init_model = "fake65c02";

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

} // namespace

namespace fake65c02_toolbox {

benchmark6502::singlestep_result run_singlestep_wdc65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_65c02_corpus(corpus, "WDC 65C02");
}

benchmark6502::singlestep_result run_singlestep_rockwell65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_65c02_corpus(corpus, "Rockwell 65C02");
}

benchmark6502::singlestep_result run_singlestep_synertek65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_65c02_corpus(corpus, "Synertek/ST 65C02");
}

} // namespace fake65c02_toolbox
