#include <fake6502_toolbox.hpp>

#include <asm6502/asm6502.h>

#include <cstdint>
#include <stdexcept>

namespace {

void require(bool condition, const char* message)
{
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void test_name_and_reset_state()
{
    auto cpu = fake6502_toolbox::make_fake6502_nmos_instruction_cpu();
    require(cpu->core_name() == "fake6502", "unexpected core name");
    require(cpu->model_name() == "nmos", "unexpected model name");

    const auto program = asm6502::Asm6502::New()
        .begin()
        .reset_vector("main")
        .org(0x0400u, "main")
            .nop()
            .jmp("main")
        .end()
        .compile();

    cpu->clear_memory(0xeau);
    cpu->load(program);
    cpu->reset();

    require(cpu->state().pc == 0x0400u, "reset did not reach start PC");
}

void test_instruction_step_and_memory()
{
    constexpr std::uint16_t data = 0x0200u;

    const auto program = asm6502::Asm6502::New()
        .begin()
        .reset_vector("main")
        .org(0x0400u, "main")
            .lda(0x12u)
            .sta(asm6502::absolute, data)
            .ldx(0x00u)
            .inx()
            .jmp("done")
        .org(0x0410u, "done")
            .nop()
            .jmp("done")
        .end()
        .compile();

    auto cpu = fake6502_toolbox::make_fake6502_nmos_instruction_cpu();
    cpu->clear_memory(0xeau);
    cpu->load(program);
    cpu->reset();

    require(cpu->state().pc == 0x0400u, "unexpected initial PC");

    cpu->step_instruction();
    require(cpu->state().a == 0x12u, "LDA did not update A");
    require(cpu->state().pc == 0x0402u, "LDA did not advance PC");

    cpu->step_instruction();
    require(cpu->read(data) == 0x12u, "STA did not write memory");

    cpu->step_instruction();
    require(cpu->state().x == 0x00u, "LDX did not update X");

    cpu->step_instruction();
    require(cpu->state().x == 0x01u, "INX did not update X");

    cpu->step_instruction();
    require(cpu->state().pc == 0x0410u, "JMP did not reach target");
}

} // namespace

int main()
{
    test_name_and_reset_state();
    test_instruction_step_and_memory();
    return 0;
}
