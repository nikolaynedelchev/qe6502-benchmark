#include <altirrasdl_toolbox.hpp>

#include <at/atcore/scheduler.h>
#include <at/atcpu/co6502.h>
#include <at/atcpu/execstate.h>
#include <at/atcpu/memorymap.h>

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace {

struct case_result {
    bool instruction_ok = false;
    bool cycle_count_ok = false;
};

void load_ram(std::array<uint8, 0x10000>& memory, const std::vector<benchmark6502::singlestep_memory_cell>& ram)
{
    for (const auto& cell : ram) {
        memory[cell.address] = cell.value;
    }
}

void set_initial_state(ATCoProc6502& cpu, const benchmark6502::singlestep_cpu_state& state)
{
    ATCPUExecState exec_state{};
    exec_state.m6502.mPC = state.pc;
    exec_state.m6502.mA = state.a;
    exec_state.m6502.mX = state.x;
    exec_state.m6502.mY = state.y;
    exec_state.m6502.mS = state.s;
    exec_state.m6502.mP = state.p;
    exec_state.m6502.mbEmulationFlag = true;
    exec_state.m6502.mbAtInsnStep = true;
    cpu.SetExecState(exec_state);
}

bool compare_final_state(ATCoProc6502& cpu,
                         const std::array<uint8, 0x10000>& memory,
                         const benchmark6502::singlestep_cpu_state& expected)
{
    ATCPUExecState actual{};
    cpu.GetExecState(actual);

    // ATCoProc6502::GetExecState() reports mInsnPC, which is the PC of the
    // instruction currently being decoded/executed. At an instruction boundary
    // after a one-instruction SingleStep case, that can still be the previous
    // instruction's PC, not the next fetch address expected by SingleStepTests.
    // Use the core's current CPU address (mPC) for the final PC comparison.
    if (cpu.GetCPUAddress() != expected.pc ||
        actual.m6502.mS != expected.s ||
        actual.m6502.mA != expected.a ||
        actual.m6502.mX != expected.x ||
        actual.m6502.mY != expected.y ||
        ((actual.m6502.mP ^ expected.p) & static_cast<uint8>(~0x10u)) != 0u) {
        return false;
    }

    for (const auto& cell : expected.ram) {
        if (memory[cell.address] != cell.value) {
            return false;
        }
    }
    return true;
}

case_result run_case(const benchmark6502::singlestep_case& test_case, const bool cmos)
{
    std::array<uint8, 0x10000> memory{};
    load_ram(memory, test_case.initial.ram);

    ATCoProc6502 cpu(cmos, false);
    ATCoProcMemoryMapView map(cpu.GetReadMap(), cpu.GetWriteMap(), cpu.GetTraceMap());
    map.SetMemory(0, 256, memory.data());
    set_initial_state(cpu, test_case.initial);

    const std::size_t expected_cycles = test_case.cycles.size();
    const std::size_t max_cycles = expected_cycles + 64u;
    std::uint64_t cycles = 0;

    while (cycles < max_cycles) {
        ATScheduler scheduler;
        const std::uint32_t start_tick = scheduler.GetTick();

        // Altirra's scheduler stops before executing the cycle that lands exactly
        // on the stop time. A +1 stop window therefore permits zero CPU cycles
        // and can deadlock this SingleStep loop, especially on the first case.
        // Use a +2 window to advance exactly one CPU cycle in this stubbed
        // scheduler integration.
        scheduler.SetStopTime(start_tick + 2u);
        cpu.Run(scheduler);
        const std::uint32_t elapsed = scheduler.GetTick() - start_tick;
        if (elapsed == 0) {
            break;
        }
        cycles += elapsed;

        ATCPUExecState state{};
        cpu.GetExecState(state);
        if (state.m6502.mbAtInsnStep) {
            break;
        }
    }

    case_result result;
    result.instruction_ok = compare_final_state(cpu, memory, test_case.final);
    result.cycle_count_ok = cycles == expected_cycles;
    return result;
}

benchmark6502::singlestep_result run_singlestep_corpus(const benchmark6502::singlestep_corpus& corpus,
                                                   const std::string& display_model_name,
                                                   const bool cmos,
                                                   const char* const cpu_init_model)
{
    benchmark6502::singlestep_result result;
    result.core_name = "altirrasdl";
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
            const case_result single = run_case(test_case, cmos);
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

namespace altirrasdl_toolbox {

benchmark6502::singlestep_result run_singlestep_nmos(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_corpus(corpus, "NMOS 6502", false, "ATCoProc6502(isC02=false)");
}

benchmark6502::singlestep_result run_singlestep_wdc65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_corpus(corpus, "65C02/WDC corpus", true, "ATCoProc6502(isC02=true)");
}

benchmark6502::singlestep_result run_singlestep_rockwell65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_corpus(corpus, "65C02/Rockwell corpus", true, "ATCoProc6502(isC02=true)");
}

benchmark6502::singlestep_result run_singlestep_synertek65c02(const benchmark6502::singlestep_corpus& corpus)
{
    return run_singlestep_corpus(corpus, "65C02/Synertek-ST corpus", true, "ATCoProc6502(isC02=true)");
}

} // namespace altirrasdl_toolbox
