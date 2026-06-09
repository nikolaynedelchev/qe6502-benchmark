#pragma once

#include <lockstep.hpp>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <iosfwd>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace interrupt_precision {

struct SuiteContext
{
    std::string core_name;
    std::string model_name;
    std::string suite_name;
    std::string detail_log_path;
    std::uint8_t setup_memory_fill = 0xeau;
};

std::string hex8(std::uint8_t value);
std::string hex16(std::uint16_t value);
std::string tsv_hex8(std::uint8_t value);
std::string sanitize_path_component(std::string_view text);
std::string default_detail_log_path(const SuiteContext& context);
std::string resolve_detail_log_path(const SuiteContext& context);
const char* yes_no(bool value) noexcept;
const char* pass_fail(bool passed) noexcept;

void print_failure_trace(std::ostream& out,
                         const tools6502::LockstepRunResult& result,
                         std::size_t context_before = 4u,
                         std::size_t context_after = 4u);

std::vector<tools6502::LockstepCommand> make_pulse_script(bool nmi,
                                                          std::size_t pulse_start_cycle,
                                                          std::size_t pulse_length,
                                                          std::size_t total_steps);

using LockstepScenarioRunnerFactory = std::function<std::unique_ptr<tools6502::LockstepScenarioRunner>()>;

int run_i_flag_windows(const SuiteContext& context,
                       const LockstepScenarioRunnerFactory& runner_factory);
int run_interrupt_arbitration(const SuiteContext& context,
                              const LockstepScenarioRunnerFactory& runner_factory);
int run_nmi_irq_pulse_sweep(int argc,
                            char** argv,
                            const SuiteContext& context,
                            const LockstepScenarioRunnerFactory& runner_factory);

} // namespace interrupt_precision
