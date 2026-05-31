#include <fake6502_toolbox.hpp>

#include <array>
#include <cstdint>
#include <vector>

extern "C" {
void fake6502_singlestep_load_ram(const unsigned short* addresses, const unsigned char* values, unsigned int count);
void fake6502_singlestep_set_state(unsigned short pc,
                                   unsigned char s,
                                   unsigned char a,
                                   unsigned char x,
                                   unsigned char y,
                                   unsigned char p);
unsigned int fake6502_singlestep_step(void (*callback)(unsigned short address, unsigned char value, int write, void* user), void* user);
unsigned short fake6502_singlestep_pc(void);
unsigned char fake6502_singlestep_s(void);
unsigned char fake6502_singlestep_a(void);
unsigned char fake6502_singlestep_x(void);
unsigned char fake6502_singlestep_y(void);
unsigned char fake6502_singlestep_p(void);
unsigned char fake6502_singlestep_memory(unsigned short address);
}

namespace {

struct trace_context {
    std::vector<benchmark6502::singlestep_cycle>* trace = nullptr;
};

struct case_result {
    bool instruction_ok = false;
    bool cycle_count_ok = false;
};

void trace_callback(const unsigned short address, const unsigned char value, const int write, void* user)
{
    auto* context = static_cast<trace_context*>(user);
    if (context == nullptr || context->trace == nullptr) {
        return;
    }
    context->trace->push_back({static_cast<std::uint16_t>(address),
                               static_cast<std::uint8_t>(value),
                               write != 0 ? benchmark6502::singlestep_memory_operation::write
                                          : benchmark6502::singlestep_memory_operation::read});
}

void load_ram(const std::vector<benchmark6502::singlestep_memory_cell>& ram)
{
    std::vector<unsigned short> addresses;
    std::vector<unsigned char> values;
    addresses.reserve(ram.size());
    values.reserve(ram.size());
    for (const auto& cell : ram) {
        addresses.push_back(cell.address);
        values.push_back(cell.value);
    }
    fake6502_singlestep_load_ram(addresses.data(), values.data(), static_cast<unsigned int>(ram.size()));
}

bool compare_final_state(const benchmark6502::singlestep_cpu_state& expected)
{
    if (fake6502_singlestep_pc() != expected.pc ||
        fake6502_singlestep_s() != expected.s ||
        fake6502_singlestep_a() != expected.a ||
        fake6502_singlestep_x() != expected.x ||
        fake6502_singlestep_y() != expected.y ||
        fake6502_singlestep_p() != expected.p) {
        return false;
    }

    for (const auto& cell : expected.ram) {
        if (fake6502_singlestep_memory(cell.address) != cell.value) {
            return false;
        }
    }
    return true;
}

case_result run_case(const benchmark6502::singlestep_case& test_case)
{
    load_ram(test_case.initial.ram);
    fake6502_singlestep_set_state(test_case.initial.pc,
                                  test_case.initial.s,
                                  test_case.initial.a,
                                  test_case.initial.x,
                                  test_case.initial.y,
                                  test_case.initial.p);

    std::vector<benchmark6502::singlestep_cycle> trace;
    trace.reserve(test_case.cycles.size() + 8u);
    trace_context context{&trace};

    const unsigned int cycles = fake6502_singlestep_step(trace_callback, &context);

    case_result result;
    result.instruction_ok = compare_final_state(test_case.final);
    result.cycle_count_ok = cycles == test_case.cycles.size();
    return result;
}

} // namespace

namespace fake6502_toolbox {

benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus)
{
    benchmark6502::singlestep_result result;
    result.core_name = "fake6502";
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

} // namespace fake6502_toolbox
