#include <o2_toolbox.hpp>

#include <asm6502/asm6502.h>
#include <cpu6502_bridge/cpu.hpp>
#include <lockstep.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void require(bool condition, const char* message)
{
    if (!condition) {
        throw std::runtime_error(message);
    }
}

std::unique_ptr<cpu6502_bridge::ICpu> make_o2()
{
    return o2_toolbox::make_o2_nmos_cpu();
}

std::unique_ptr<cpu6502_bridge::ICpu> make_perfect()
{
    return cpu6502_bridge::make_perfect6502_cpu();
}

void require_lockstep_passed(const tools6502::LockstepRunResult& result,
                             const char* message)
{
    require(result.passed, message);
    require(!result.first_mismatch, message);
}

void test_cpu_name_is_available()
{
    const auto cpu = make_o2();
    require(std::string(cpu->get_name()) == "O2 NMOS", "unexpected O2 ICpu name");
}

void test_reset_nop_loop_bus_lockstep()
{
    tools6502::testcase test{};
    test.start_at = 0x0400u;
    test.program = asm6502::Asm6502::New()
        .begin()
        .org(test.start_at)
            .nop()
            .nop()
            .jmp(test.start_at)
        .end()
        .compile();

    tools6502::LockstepRunner runner(make_o2(), make_perfect());

    tools6502::LockstepConfig config{};
    config.memory = tools6502::MemoryFill{0x00u};
    require(runner.setup_and_run(test, config), "O2 setup_and_run(nop loop) failed");
    require(runner.left().is_opcode_fetch(), "O2 not at opcode fetch after setup");
    require(runner.left().bus_address() == test.start_at, "O2 not at testcase start after setup");

    const auto result = runner.step_cycles(8u);
    require_lockstep_passed(result, "O2 NOP loop did not stay bus-lockstepped");
    require(result.left_trace.size() == 8u, "unexpected O2 NOP trace length");
    require(result.right_trace.size() == 8u, "unexpected perfect6502 NOP trace length");
}

void test_read_write_program_bus_lockstep()
{
    constexpr std::uint16_t data_address = 0x0200u;

    tools6502::testcase test{};
    test.start_at = 0x0500u;
    test.program = asm6502::Asm6502::New()
        .begin()
        .org(test.start_at)
            .lda(0x42u)
            .sta(asm6502::absolute, data_address)
            .inc(asm6502::absolute, data_address)
            .jmp(test.start_at)
        .end()
        .compile();

    tools6502::LockstepRunner runner(make_o2(), make_perfect());

    tools6502::LockstepConfig config{};
    config.memory = tools6502::MemoryFill{0x00u};
    require(runner.setup_and_run(test, config), "O2 setup_and_run(read/write) failed");
    const auto result = runner.step_cycles(16u);
    require_lockstep_passed(result, "O2 read/write program did not stay bus-lockstepped");
    require(runner.left().memory()[data_address] == 0x43u, "O2 read/write program did not update memory");
    require(runner.right().memory()[data_address] == 0x43u, "perfect6502 read/write program did not update memory");
}

} // namespace

int main()
{
    test_cpu_name_is_available();
    test_reset_nop_loop_bus_lockstep();
    test_read_write_program_bus_lockstep();
    return 0;
}
