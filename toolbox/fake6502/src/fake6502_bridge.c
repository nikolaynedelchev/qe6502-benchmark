#include <string.h>

#include <fake6502.h>

static uint8 memory[65536];

static const uint8 klaus_nmos_rom[65536] =
#include "cores/qe6502/harness/klaus2m5/6502_functional_test.hex"
;

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

int fake6502_klaus_nmos_standard_run_once(unsigned long long* cycles_out)
{
    const ushort start_address = 0x0400u;
    const ushort success_address = 0x3469u;
    unsigned long long cycles = 0;

    memcpy(memory, klaus_nmos_rom, sizeof(memory));
    reset6502();
    pc = start_address;

    for (;;) {
        if (pc == success_address) {
            if (cycles_out != 0) {
                *cycles_out = cycles;
            }
            return 0;
        }
        cycles += step6502();
    }
}
