#include <olcNES_toolbox.hpp>

#include <Bus.h>

namespace olcNES_toolbox {
namespace {

void run_until_complete(olc6502& cpu)
{
    do {
        cpu.clock();
    } while (!cpu.complete());
}

} // namespace

benchmark6502::smoke_result run_smoke_test()
{
    Bus bus;

    bus.ram[0xFFFC] = 0x00;
    bus.ram[0xFFFD] = 0x80;
    bus.ram[0x8000] = 0xA2; // LDX #$42
    bus.ram[0x8001] = 0x42;

    bus.cpu.reset();
    run_until_complete(bus.cpu);
    run_until_complete(bus.cpu);

    if (bus.cpu.x != 0x42) {
        return {false, "LDX #$42 did not set X to $42"};
    }

    return {true, "ok"};
}

} // namespace olcNES_toolbox
