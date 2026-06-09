#include "common/common.hpp"

#include <floooh_chips_toolbox.hpp>

#include <cpu6502_bridge/cpu.hpp>
#include <lockstep.hpp>

#include <memory>

namespace {

std::unique_ptr<tools6502::LockstepScenarioRunner> make_runner()
{
    return std::make_unique<tools6502::LockstepScenarioRunner>(
        floooh_chips_toolbox::make_floooh_chips_nmos_cpu(),
        cpu6502_bridge::make_perfect6502_cpu());
}

} // namespace

int main(int argc, char** argv)
{
    const interrupt_precision::SuiteContext context{"floooh_chips", "nmos", "nmi_irq_pulse_sweep", {}};
    return interrupt_precision::run_nmi_irq_pulse_sweep(argc, argv, context, make_runner);
}
