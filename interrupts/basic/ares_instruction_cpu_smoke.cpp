#include <ares_toolbox.hpp>

#include <cstdlib>

int main()
{
    auto cpu = ares_toolbox::make_ares_nmos_instruction_cpu();
    cpu->clear_memory(0xea);
    cpu->write(0xfffc, 0x00);
    cpu->write(0xfffd, 0x20);
    cpu->reset();
    const auto state = cpu->state();
    return state.pc == 0x2000 ? EXIT_SUCCESS : EXIT_FAILURE;
}
