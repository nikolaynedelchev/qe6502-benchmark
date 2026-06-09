#include "common/common.hpp"

#include <peddle_toolbox.hpp>

#include <cpu6502_bridge/cpu.hpp>
#include <lockstep.hpp>

#include <memory>

namespace {

std::unique_ptr<tools6502::LockstepScenarioRunner> make_runner()
{
    return std::make_unique<tools6502::LockstepScenarioRunner>(
        peddle_toolbox::make_peddle_nmos_cpu(),
        cpu6502_bridge::make_perfect6502_cpu());
}

} // namespace

int main()
{
    const interrupt_precision::SuiteContext context{"peddle", "nmos", "i_flag_windows", {}, 0x00u};
    return interrupt_precision::run_i_flag_windows(context, make_runner);
}
