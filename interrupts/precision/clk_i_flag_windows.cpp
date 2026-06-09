#include "common/common.hpp"

#include <clk_toolbox.hpp>

#include <cpu6502_bridge/cpu.hpp>
#include <lockstep.hpp>

#include <memory>

namespace {

std::unique_ptr<tools6502::LockstepScenarioRunner> make_runner()
{
    return std::make_unique<tools6502::LockstepScenarioRunner>(
        clk_toolbox::make_clk_p6502_nmos_cpu(),
        cpu6502_bridge::make_perfect6502_cpu());
}

} // namespace

int main()
{
    const interrupt_precision::SuiteContext context{"clk", "nmos", "i_flag_windows", {}};
    return interrupt_precision::run_i_flag_windows(context, make_runner);
}
