#include <gianlucag_mos6502_toolbox.hpp>

int main()
{
    auto cpu = gianlucag_mos6502_toolbox::make_gianlucag_mos6502_nmos_instruction_cpu();
    cpu->clear_memory(0xea);
    cpu->write(0xfffc, 0x00);
    cpu->write(0xfffd, 0x80);
    cpu->reset();
    return cpu->state().pc == 0x8000u ? 0 : 1;
}
