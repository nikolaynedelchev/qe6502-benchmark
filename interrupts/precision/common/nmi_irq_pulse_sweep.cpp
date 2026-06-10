#include "common.hpp"

#include <benchmark6502/opcode_metadata.hpp>
#include <lockstep.hpp>
#include <tools6502/testcase_collections.hpp>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace {

constexpr std::size_t min_pulse_length = 1u;
constexpr std::size_t max_pulse_start_cycle = 8u;
constexpr std::size_t pulse_end_cycle_limit = 10u;
constexpr std::size_t scenario_total_steps = 20u;

struct SweepStats
{
    std::size_t opcodes_total = 0u;
    std::size_t opcodes_passed = 0u;
    std::size_t opcodes_failed = 0u;
    std::size_t opcodes_skipped = 0u;
    std::size_t legal_opcodes_total = 0u;
    std::size_t legal_opcodes_failed = 0u;
    std::size_t illegal_opcodes_total = 0u;
    std::size_t illegal_opcodes_failed = 0u;
    std::size_t illegal_opcodes_skipped = 0u;
    std::size_t testcases_total = 0u;
    std::size_t testcases_skipped = 0u;
    std::size_t scenarios_total = 0u;
    std::size_t scenarios_passed = 0u;
    std::size_t scenarios_failed = 0u;
    std::size_t legal_scenarios_total = 0u;
    std::size_t legal_scenarios_failed = 0u;
    std::size_t illegal_scenarios_total = 0u;
    std::size_t illegal_scenarios_failed = 0u;
    std::size_t nmi_scenarios_total = 0u;
    std::size_t nmi_scenarios_failed = 0u;
    std::size_t irq_scenarios_total = 0u;
    std::size_t irq_scenarios_failed = 0u;
    std::size_t failure_recovery_memory_clears = 0u;
    interrupt_precision::SecondaryMismatchStats secondary{};
};

struct OpcodeStats
{
    std::size_t testcases = 0u;
    std::size_t scenarios = 0u;
    std::size_t failures = 0u;
    std::size_t nmi_scenarios = 0u;
    std::size_t nmi_failures = 0u;
    std::size_t irq_scenarios = 0u;
    std::size_t irq_failures = 0u;
    interrupt_precision::SecondaryMismatchStats secondary{};
};

constexpr std::array<std::uint8_t, 16u> light_opcodes{
    0x00, // BRK: interrupt/vector entry
    0x01, // ORA (zp,X): indexed-indirect read
    0x06, // ASL zp: read-modify-write memory
    0x08, // PHP: stack push
    0x20, // JSR abs: control flow plus stack writes
    0x24, // BIT zp: flag-only memory read
    0x29, // AND #imm: ALU immediate
    0x40, // RTI: interrupt return stack pull
    0x4C, // JMP abs: absolute control transfer
    0x60, // RTS: subroutine return stack pull
    0x69, // ADC #imm: carry/arithmetic
    0x85, // STA zp: memory write
    0xA9, // LDA #imm: load immediate
    0xB0, // BCS: conditional branch timing variants
    0xE9, // SBC #imm: subtract/arithmetic
    0xEA, // NOP: implied idle instruction
};

bool is_light_opcode(std::uint8_t opcode) noexcept
{
    return std::find(light_opcodes.begin(), light_opcodes.end(), opcode) != light_opcodes.end();
}

bool is_unstable_opcode(std::uint8_t opcode) noexcept
{
    switch (opcode) {
    case 0x0B:
    case 0x2B:
    case 0x4B:
    case 0x6B:
    case 0x8B:
    case 0xAB:
    case 0xBB:
        return true;
    default:
        return false;
    }
}

struct FirstFailure
{
    bool set = false;
    std::uint8_t opcode = 0u;
    std::string testcase_description{};
    const char* signal = "";
    std::size_t pulse_start = 0u;
    std::size_t pulse_length = 0u;
    tools6502::LockstepScenarioResult scenario_result{};
};

void write_opcode_detail_row(std::ostream& out,
                             const interrupt_precision::SuiteContext& context,
                             const char* mode,
                             std::uint8_t opcode,
                             const OpcodeStats& opcode_stats,
                             const char* status)
{
    const auto& metadata = benchmark6502::nmos_opcode_metadata(opcode);
    out << context.core_name << '\t'
        << context.model_name << '\t'
        << context.suite_name << '\t'
        << mode << '\t'
        << interrupt_precision::tsv_hex8(opcode) << '\t'
        << metadata.mnemonic << '\t'
        << benchmark6502::addressing_mode_name(metadata.mode) << '\t'
        << interrupt_precision::yes_no(metadata.legal_nmos) << '\t'
        << opcode_stats.testcases << '\t'
        << opcode_stats.scenarios << '\t'
        << opcode_stats.failures << '\t'
        << opcode_stats.nmi_scenarios << '\t'
        << opcode_stats.nmi_failures << '\t'
        << opcode_stats.irq_scenarios << '\t'
        << opcode_stats.irq_failures << '\t'
        << status << '\n';
    out.flush();
}

bool run_one_scenario(tools6502::LockstepScenarioRunner& runner,
                      const tools6502::testcase& test,
                      bool nmi,
                      std::size_t pulse_start,
                      std::size_t pulse_length,
                      const tools6502::LockstepScenarioConfig& scenario_config,
                      SweepStats& stats,
                      OpcodeStats& opcode_stats,
                      FirstFailure& first_failure)
{
    const auto script = interrupt_precision::make_pulse_script(
        nmi,
        pulse_start,
        pulse_length,
        scenario_total_steps);

    const auto result = runner.restart_run(script, scenario_config);
    const bool legal = benchmark6502::is_legal_nmos_opcode(test.opcode);

    ++stats.scenarios_total;
    ++opcode_stats.scenarios;
    if (legal) {
        ++stats.legal_scenarios_total;
    } else {
        ++stats.illegal_scenarios_total;
    }
    if (nmi) {
        ++stats.nmi_scenarios_total;
        ++opcode_stats.nmi_scenarios;
    } else {
        ++stats.irq_scenarios_total;
        ++opcode_stats.irq_scenarios;
    }

    const auto secondary = interrupt_precision::count_secondary_mismatches(result);
    interrupt_precision::add_secondary_mismatches(stats.secondary, secondary);
    interrupt_precision::add_secondary_mismatches(opcode_stats.secondary, secondary);

    if (result.passed) {
        ++stats.scenarios_passed;
        return true;
    }

    ++stats.scenarios_failed;
    ++opcode_stats.failures;
    if (legal) {
        ++stats.legal_scenarios_failed;
    } else {
        ++stats.illegal_scenarios_failed;
    }
    if (nmi) {
        ++stats.nmi_scenarios_failed;
        ++opcode_stats.nmi_failures;
    } else {
        ++stats.irq_scenarios_failed;
        ++opcode_stats.irq_failures;
    }

    if (!first_failure.set) {
        first_failure.set = true;
        first_failure.opcode = test.opcode;
        first_failure.testcase_description = test.description;
        first_failure.signal = nmi ? "NMI" : "IRQ";
        first_failure.pulse_start = pulse_start;
        first_failure.pulse_length = pulse_length;
        first_failure.scenario_result = result;
    }

    return false;
}

} // namespace

int interrupt_precision::run_nmi_irq_pulse_sweep(int argc,
                                                 char** argv,
                                                 const SuiteContext& context,
                                                 const LockstepScenarioRunnerFactory& runner_factory)
{
    bool light = false;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--light") {
            light = true;
            continue;
        }

        std::cerr << "usage: " << argv[0] << " [--light]\n";
        return 2;
    }

    const auto testcases_by_opcode = tools6502::get_nmos6502_opcode_testcases();
    const std::string detail_log_path = resolve_detail_log_path(context);
    std::ofstream detail_log(detail_log_path);
    if (!detail_log) {
        std::cerr << "failed to open interrupt precision detail log: " << detail_log_path << '\n';
        return 2;
    }
    detail_log << "core\tmodel\tsuite\tmode\topcode\tmnemonic\taddressing_mode\tlegal_nmos\t"
               << "testcases\tscenarios\tfailures\tnmi_scenarios\tnmi_failures\t"
               << "irq_scenarios\tirq_failures\tsecondary_mismatches\tsecondary_fetch_tag\t"
               << "secondary_irq_line\tsecondary_nmi_line\tstatus\n";

    tools6502::LockstepConfig base_lockstep_config{};
    base_lockstep_config.memory = tools6502::MemoryUnchanged{};
    // Primary precision pass/fail is public bus identity: rw/address/data.
    // Opcode-fetch and visible line annotations are reported as secondary metadata.
    base_lockstep_config.compare.address = true;
    base_lockstep_config.compare.data = true;
    base_lockstep_config.compare.read_write = true;
    base_lockstep_config.compare.opcode_fetch = false;
    base_lockstep_config.compare.registers_on_fetch = false;

    tools6502::LockstepScenarioConfig scenario_config{};
    scenario_config.stop_on_failure = true;

    SweepStats stats{};
    FirstFailure first_failure{};
    const char* mode = light ? "light" : "full";

    std::cout << "nmi_irq_pulse_sweep mode: "
              << (light ? "light (16 representative opcodes)" : "full (all opcodes)")
              << '\n' << std::flush;

    for (const auto& [opcode, tests] : testcases_by_opcode) {
        if (light && !is_light_opcode(opcode)) {
            continue;
        }

        const auto& metadata = benchmark6502::nmos_opcode_metadata(opcode);
        ++stats.opcodes_total;
        if (metadata.legal_nmos) {
            ++stats.legal_opcodes_total;
        } else {
            ++stats.illegal_opcodes_total;
        }
        stats.testcases_total += tests.size();

        OpcodeStats opcode_stats{};
        opcode_stats.testcases = tests.size();

        if (is_unstable_opcode(opcode)) {
            ++stats.opcodes_skipped;
            if (!metadata.legal_nmos) {
                ++stats.illegal_opcodes_skipped;
            }
            stats.testcases_skipped += tests.size();
            write_opcode_detail_row(detail_log, context, mode, opcode, opcode_stats, "skipped_unstable");
            std::cout << "opcode " << interrupt_precision::hex8(opcode)
                      << ' ' << metadata.mnemonic
                      << ' ' << (metadata.legal_nmos ? "legal" : "illegal")
                      << " done: SKIP unstable testcases=" << opcode_stats.testcases
                      << " scenarios=0 failures=0\n" << std::flush;
            continue;
        }

        bool opcode_passed = true;

        for (const auto& test : tests) {
            auto unchanged_lockstep_config = base_lockstep_config;
            auto clean_lockstep_config = base_lockstep_config;
            clean_lockstep_config.memory = tools6502::MemoryFill{context.setup_memory_fill};
            auto runner = runner_factory();
            bool next_scenario_requires_clean_memory = false;

            if (!runner->setup(test, unchanged_lockstep_config)) {
                opcode_passed = false;
                ++stats.scenarios_total;
                ++stats.scenarios_failed;
                if (metadata.legal_nmos) {
                    ++stats.legal_scenarios_total;
                    ++stats.legal_scenarios_failed;
                } else {
                    ++stats.illegal_scenarios_total;
                    ++stats.illegal_scenarios_failed;
                }
                ++opcode_stats.scenarios;
                ++opcode_stats.failures;
                if (!first_failure.set) {
                    first_failure.set = true;
                    first_failure.opcode = test.opcode;
                    first_failure.testcase_description = test.description;
                    first_failure.signal = "setup";
                }
                continue;
            }

            auto run_checked_scenario = [&](bool nmi,
                                            std::size_t pulse_start,
                                            std::size_t pulse_length) {
                const bool used_recovery_clear = next_scenario_requires_clean_memory;
                if (next_scenario_requires_clean_memory) {
                    ++stats.failure_recovery_memory_clears;
                    runner = runner_factory();
                    runner->setup(test, clean_lockstep_config);
                    next_scenario_requires_clean_memory = false;
                }

                const bool passed = run_one_scenario(*runner,
                                                     test,
                                                     nmi,
                                                     pulse_start,
                                                     pulse_length,
                                                     scenario_config,
                                                     stats,
                                                     opcode_stats,
                                                     first_failure);

                if (used_recovery_clear && passed) {
                    runner->setup(test, unchanged_lockstep_config);
                }
                if (!passed) {
                    opcode_passed = false;
                    next_scenario_requires_clean_memory = true;
                }
            };

            for (std::size_t pulse_start = 0u;
                 pulse_start <= max_pulse_start_cycle;
                 ++pulse_start) {
                const std::size_t max_pulse_length = pulse_end_cycle_limit - pulse_start;

                for (std::size_t pulse_length = min_pulse_length;
                     pulse_length <= max_pulse_length;
                     ++pulse_length) {
                    run_checked_scenario(true, pulse_start, pulse_length);
                    run_checked_scenario(false, pulse_start, pulse_length);
                }
            }
        }

        if (opcode_passed) {
            ++stats.opcodes_passed;
        } else {
            ++stats.opcodes_failed;
            if (metadata.legal_nmos) {
                ++stats.legal_opcodes_failed;
            } else {
                ++stats.illegal_opcodes_failed;
            }
        }

        write_opcode_detail_row(detail_log,
                                context,
                                mode,
                                opcode,
                                opcode_stats,
                                opcode_passed ? "passed" : "failed");

        std::cout << "opcode " << interrupt_precision::hex8(opcode)
                  << ' ' << metadata.mnemonic
                  << ' ' << (metadata.legal_nmos ? "legal" : "illegal")
                  << " done: " << (opcode_passed ? "PASS" : "FAIL")
                  << " testcases=" << opcode_stats.testcases
                  << " scenarios=" << opcode_stats.scenarios
                  << " failures=" << opcode_stats.failures
                  << " secondary=" << opcode_stats.secondary.total
                  << '\n' << std::flush;
    }

    std::cout << "\nsummary\n"
              << "  opcodes:   total=" << stats.opcodes_total
              << " pass=" << stats.opcodes_passed
              << " fail=" << stats.opcodes_failed
              << " skip=" << stats.opcodes_skipped << '\n'
              << "  legal opcodes:   total=" << stats.legal_opcodes_total
              << " fail=" << stats.legal_opcodes_failed << '\n'
              << "  illegal opcodes: total=" << stats.illegal_opcodes_total
              << " fail=" << stats.illegal_opcodes_failed
              << " skip=" << stats.illegal_opcodes_skipped << '\n'
              << "  testcases: total=" << stats.testcases_total
              << " skipped=" << stats.testcases_skipped << '\n'
              << "  scenarios: total=" << stats.scenarios_total
              << " pass=" << stats.scenarios_passed
              << " fail=" << stats.scenarios_failed << '\n'
              << "  legal scenarios:   total=" << stats.legal_scenarios_total
              << " fail=" << stats.legal_scenarios_failed << '\n'
              << "  illegal scenarios: total=" << stats.illegal_scenarios_total
              << " fail=" << stats.illegal_scenarios_failed << '\n'
              << "  NMI:       total=" << stats.nmi_scenarios_total
              << " fail=" << stats.nmi_scenarios_failed << '\n'
              << "  IRQ:       total=" << stats.irq_scenarios_total
              << " fail=" << stats.irq_scenarios_failed << '\n'
              << "  failure recovery memory clears: " << stats.failure_recovery_memory_clears << '\n'
              << "  secondary metadata mismatches: total=" << stats.secondary.total
              << " fetch_tag=" << stats.secondary.opcode_fetch
              << " irq_line=" << stats.secondary.irq_line
              << " nmi_line=" << stats.secondary.nmi_line << '\n'
              << "  detail log: " << detail_log_path << '\n';

    if (first_failure.set) {
        const auto& metadata = benchmark6502::nmos_opcode_metadata(first_failure.opcode);
        std::cout << "\nfirst failure\n"
                  << "  opcode:       " << interrupt_precision::hex8(first_failure.opcode)
                  << ' ' << metadata.mnemonic
                  << ' ' << (metadata.legal_nmos ? "legal" : "illegal") << '\n'
                  << "  testcase:     " << first_failure.testcase_description << '\n'
                  << "  signal:       " << first_failure.signal << '\n'
                  << "  pulse_start:  " << first_failure.pulse_start << '\n'
                  << "  pulse_length: " << first_failure.pulse_length << '\n';

        if (first_failure.scenario_result.first_failed_command) {
            const auto command_index = *first_failure.scenario_result.first_failed_command;
            std::cout << "  command:      " << command_index << '\n';
            if (command_index < first_failure.scenario_result.results.size()) {
                interrupt_precision::print_failure_trace(
                    std::cout,
                    first_failure.scenario_result.results[command_index]);
            }
        }
    }

    return stats.scenarios_failed == 0u ? 0 : 1;
}
