#include <string.h>

#include <fake6502.h>

static uint8 memory[65536];

uint8 read6502(ushort address)
{
    return memory[address];
}

void write6502(ushort address, uint8 value)
{
    memory[address] = value;
}

int fake6502_smoke_run(unsigned int* cycles_out)
{
    memset(memory, 0xea, sizeof(memory));
    memory[0x8000] = 0xa2u; /* LDX #$42 */
    memory[0x8001] = 0x42u;
    memory[0xfffc] = 0x00u;
    memory[0xfffd] = 0x80u;

    reset6502();
    const uint32 cycles = step6502();

    if (cycles_out != 0) {
        *cycles_out = cycles;
    }

    if (x != 0x42u) {
        return 1;
    }
    if (cycles != 2u) {
        return 2;
    }
    return 0;
}
