#include "common/common.hpp"

#include <cpu6502_bridge/cpu.hpp>
#include <lockstep.hpp>

#include <memory>

namespace {

std::unique_ptr<tools6502::LockstepScenarioRunner> make_runner()
{
    return std::make_unique<tools6502::LockstepScenarioRunner>(
        cpu6502_bridge::make_qe6502_cpu(),
        cpu6502_bridge::make_perfect6502_cpu());
}

} // namespace

int main()
{
    const interrupt_precision::SuiteContext context{"qe6502", "nmos", "interrupt_arbitration", {}};
    return interrupt_precision::run_interrupt_arbitration(context, make_runner);
}
