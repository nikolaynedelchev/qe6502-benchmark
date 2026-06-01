#include <string.h>

#define read6502 fake65c02_read6502
#define write6502 fake65c02_write6502
#include <fake65c02.h>
#undef read6502
#undef write6502

static uint8 memory[65536];

static const uint8 klaus_65c02_extended_rom[65536] =
#include "cores/qe6502/harness/klaus2m5/65C02_extended_opcodes_test.hex"
;

uint8 fake65c02_read6502(ushort address)
{
    return memory[address];
}

void fake65c02_write6502(ushort address, uint8 value)
{
    memory[address] = value;
}

int fake65c02_smoke_run(unsigned int* cycles_out)
{
    unsigned int cycles = 0;

    memset(memory, 0xea, sizeof(memory));
    memory[0x8000] = 0xa9u; /* LDA #$77 */
    memory[0x8001] = 0x77u;
    memory[0x8002] = 0x8du; /* STA $2000 */
    memory[0x8003] = 0x00u;
    memory[0x8004] = 0x20u;
    memory[0x8005] = 0x9cu; /* STZ $2000 (65C02) */
    memory[0x8006] = 0x00u;
    memory[0x8007] = 0x20u;
    memory[0x8008] = 0x80u; /* BRA +2 (65C02) */
    memory[0x8009] = 0x02u;
    memory[0x800a] = 0xa9u; /* skipped: LDA #$01 */
    memory[0x800b] = 0x01u;
    memory[0x800c] = 0xa9u; /* LDA #$42 */
    memory[0x800d] = 0x42u;
    memory[0xfffc] = 0x00u;
    memory[0xfffd] = 0x80u;

    reset6502();
    cycles += step6502();
    cycles += step6502();
    cycles += step6502();
    cycles += step6502();
    cycles += step6502();

    if (cycles_out != 0) {
        *cycles_out = cycles;
    }

    if (memory[0x2000] != 0x00u) {
        return 1;
    }
    if (a != 0x42u) {
        return 2;
    }
    return 0;
}

int fake65c02_klaus_extended_run_once(unsigned long long* cycles_out)
{
    const ushort start_address = 0x0400u;
    const ushort success_address = 0x24F1u;
    const unsigned long long max_cycles = 2ull * 21986985ull;
    unsigned long long cycles = 0;

    memcpy(memory, klaus_65c02_extended_rom, sizeof(memory));
    reset6502();
    pc = start_address;

    for (;;) {
        if (pc == success_address) {
            if (cycles_out != 0) {
                *cycles_out = cycles;
            }
            return 0;
        }
        if (cycles > max_cycles) {
            if (cycles_out != 0) {
                *cycles_out = cycles;
            }
            return 1;
        }
        cycles += step6502();
    }
}

void fake65c02_singlestep_clear_memory(void)
{
    memset(memory, 0, sizeof(memory));
}

void fake65c02_singlestep_write_memory(unsigned short address, unsigned char value)
{
    memory[address] = value;
}

unsigned char fake65c02_singlestep_read_memory(unsigned short address)
{
    return memory[address];
}

void fake65c02_singlestep_set_state(unsigned short state_pc,
                                    unsigned char state_s,
                                    unsigned char state_a,
                                    unsigned char state_x,
                                    unsigned char state_y,
                                    unsigned char state_p)
{
    pc = state_pc;
    sp = state_s;
    a = state_a;
    x = state_x;
    y = state_y;
    status = state_p;
    clockticks6502 = 0;
    instructions = 0;
    waiting6502 = 0;
}

void fake65c02_singlestep_get_state(unsigned short* state_pc,
                                    unsigned char* state_s,
                                    unsigned char* state_a,
                                    unsigned char* state_x,
                                    unsigned char* state_y,
                                    unsigned char* state_p)
{
    if (state_pc != 0) {
        *state_pc = pc;
    }
    if (state_s != 0) {
        *state_s = sp;
    }
    if (state_a != 0) {
        *state_a = a;
    }
    if (state_x != 0) {
        *state_x = x;
    }
    if (state_y != 0) {
        *state_y = y;
    }
    if (state_p != 0) {
        *state_p = status;
    }
}

unsigned int fake65c02_singlestep_step(void)
{
    return step6502();
}
