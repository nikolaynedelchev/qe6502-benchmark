#include <vremu6502_toolbox.hpp>

#include <vrEmu6502.h>

#include <array>
#include <cstdint>
#include <string>
#include <vector>

struct vrEmu6502Opcode;

struct vrEmu6502_s
{
    vrEmu6502Model model;

    vrEmu6502MemRead readFn;
    vrEmu6502MemWrite writeFn;

    vrEmu6502Interrupt intPin;
    vrEmu6502Interrupt nmiPin;

    std::uint8_t step;
    std::uint8_t currentOpcode;
    std::uint16_t currentOpcodeAddr;

    bool wai;
    bool stp;

    std::uint16_t pc;

    std::uint8_t ac;
    std::uint8_t ix;
    std::uint8_t iy;
    std::uint8_t sp;

    std::uint8_t flags;

    std::uint16_t zpBase;
    std::uint16_t spBase;
    std::uint16_t tmpAddr;

    const vrEmu6502Opcode* opcodes;
    const char* mnemonicNames[256];
    vrEmu6502AddrMode addrModes[256];
};

namespace {

struct memory_context {
    std::array<std::uint8_t, 0x10000>* memory = nullptr;
};

struct case_result {
    bool instruction_ok = false;
    bool cycle_count_ok = false;
};

thread_local memory_context* g_context = nullptr;

std::uint8_t memory_read(const std::uint16_t address, const bool /*is_debug*/)
{
    return (*g_context->memory)[address];
}

void memory_write(const std::uint16_t address, const std::uint8_t value)
{
    (*g_context->memory)[address] = value;
}

void load_ram(std::array<std::uint8_t, 0x10000>& memory, const std::vector<benchmark6502::singlestep_memory_cell>& ram)
{
    for (const auto& cell : ram) {
        memory[cell.address] = cell.value;
    }
}

void set_initial_state(VrEmu6502* cpu, const benchmark6502::singlestep_cpu_state& state)
{
    cpu->step = 0;
    cpu->currentOpcode = 0;
    cpu->currentOpcodeAddr = state.pc;
    cpu->wai = false;
    cpu->stp = false;
    cpu->pc = state.pc;
    cpu->ac = state.a;
    cpu->ix = state.x;
    cpu->iy = state.y;
    cpu->sp = state.s;
    cpu->flags = state.p;
    cpu->tmpAddr = 0;
    cpu->intPin = IntCleared;
    cpu->nmiPin = IntCleared;
}

bool compare_final_state(VrEmu6502* cpu,
                         const std::array<std::uint8_t, 0x10000>& memory,
                         const benchmark6502::singlestep_cpu_state& expected)
{
    if (vrEmu6502GetPC(cpu) != expected.pc ||
        vrEmu6502GetStackPointer(cpu) != expected.s ||
        vrEmu6502GetAcc(cpu) != expected.a ||
        vrEmu6502GetX(cpu) != expected.x ||
        vrEmu6502GetY(cpu) != expected.y ||
        vrEmu6502GetStatus(cpu) != expected.p) {
        return false;
    }

    for (const auto& cell : expected.ram) {
        if (memory[cell.address] != cell.value) {
            return false;
        }
    }
    return true;
}

case_result run_case(const benchmark6502::singlestep_case& test_case, const vrEmu6502Model model)
{
    std::array<std::uint8_t, 0x10000> memory{};
    load_ram(memory, test_case.initial.ram);

    memory_context context{&memory};
    g_context = &context;

    VrEmu6502* cpu = vrEmu6502New(model, memory_read, memory_write);
    if (cpu == nullptr) {
        g_context = nullptr;
        return {};
    }

    set_initial_state(cpu, test_case.initial);
    const std::uint8_t cycles = vrEmu6502InstCycle(cpu);

    case_result result;
    result.instruction_ok = compare_final_state(cpu, memory, test_case.final);
    result.cycle_count_ok = cycles == test_case.cycles.size();

    vrEmu6502Destroy(cpu);
    g_context = nullptr;
    return result;
}

} // namespace

namespace vremu6502_toolbox {

benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus)
{
    benchmark6502::singlestep_result result;
    result.core_name = "vrEmu6502";
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
            const case_result single = run_case(test_case, CPU_6502U);
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

benchmark6502::singlestep_result run_singlestep_65c02_model(const benchmark6502::singlestep_corpus& corpus,
                                                                 const vrEmu6502Model cpu_model,
                                                                 const char* const cpu_model_name,
                                                                 const std::string& display_model_name)
{
    benchmark6502::singlestep_result result;
    result.core_name = "vrEmu6502";
    result.corpus_model = corpus.model;
    result.model_name = display_model_name;
    result.cpu_init_model = cpu_model_name;

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
            const case_result single = run_case(test_case, cpu_model);
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

benchmark6502::singlestep_result run_singlestep_cpu65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_65c02_model(corpus,
                                      CPU_65C02,
                                      "CPU_65C02",
                                      std::string{"CPU_65C02/"} + benchmark6502::singlestep_model_name(corpus.model) + " corpus");
}

benchmark6502::singlestep_result run_singlestep_wdc65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_65c02_model(corpus, CPU_W65C02, "CPU_W65C02", "WDC 65C02");
}

benchmark6502::singlestep_result run_singlestep_rockwell65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_65c02_model(corpus, CPU_R65C02, "CPU_R65C02", "Rockwell 65C02");
}

benchmark6502::singlestep_result run_singlestep_synertek65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_65c02_model(corpus, CPU_65C02, "CPU_65C02", "Synertek/ST 65C02");
}

} // namespace vremu6502_toolbox
