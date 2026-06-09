#include <string.h>

#include <fake6502.h>

static uint8 memory[65536];

typedef void (*fake6502_trace_callback_t)(ushort address, uint8 value, int write, void* user);

static fake6502_trace_callback_t trace_callback = 0;
static void* trace_user = 0;

static void fake6502_set_trace_callback(fake6502_trace_callback_t callback, void* user)
{
    trace_callback = callback;
    trace_user = user;
}

static const uint8 klaus_nmos_rom[65536] =
#include "cores/qe6502/harness/klaus2m5/6502_functional_test.hex"
;

uint8 read6502(ushort address)
{
    const uint8 value = memory[address];
    if (trace_callback != 0) {
        trace_callback(address, value, 0, trace_user);
    }
    return value;
}

void write6502(ushort address, uint8 value)
{
    memory[address] = value;
    if (trace_callback != 0) {
        trace_callback(address, value, 1, trace_user);
    }
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


void fake6502_singlestep_load_ram(const unsigned short* addresses, const unsigned char* values, unsigned int count)
{
    memset(memory, 0, sizeof(memory));
    for (unsigned int i = 0; i < count; ++i) {
        memory[addresses[i]] = values[i];
    }
}

void fake6502_singlestep_set_state(unsigned short in_pc,
                                   unsigned char in_s,
                                   unsigned char in_a,
                                   unsigned char in_x,
                                   unsigned char in_y,
                                   unsigned char in_p)
{
    pc = in_pc;
    sp = in_s;
    a = in_a;
    x = in_x;
    y = in_y;
    status = in_p;
    instructions = 0;
    clockticks6502 = 0;
    clockgoal6502 = 0;
    oldpc = 0;
    ea = 0;
    reladdr = 0;
    value = 0;
    result = 0;
    opcode = 0;
    oldstatus = 0;
    penaltyop = 0;
    penaltyaddr = 0;
    hookexternal(0);
}

unsigned int fake6502_singlestep_step(void (*callback)(unsigned short address, unsigned char value, int write, void* user), void* user)
{
    fake6502_set_trace_callback(callback, user);
    const uint32 cycles = step6502();
    fake6502_set_trace_callback(0, 0);
    return cycles;
}

unsigned short fake6502_singlestep_pc(void)
{
    return pc;
}

unsigned char fake6502_singlestep_s(void)
{
    return sp;
}

unsigned char fake6502_singlestep_a(void)
{
    return a;
}

unsigned char fake6502_singlestep_x(void)
{
    return x;
}

unsigned char fake6502_singlestep_y(void)
{
    return y;
}

unsigned char fake6502_singlestep_p(void)
{
    return status;
}

unsigned char fake6502_singlestep_memory(unsigned short address)
{
    return memory[address];
}

void fake6502_instruction_clear_memory(unsigned char fill)
{
    memset(memory, fill, sizeof(memory));
}

void fake6502_instruction_write_memory(unsigned short address, unsigned char value)
{
    memory[address] = value;
}

unsigned char fake6502_instruction_read_memory(unsigned short address)
{
    return memory[address];
}

void fake6502_instruction_reset(void)
{
    reset6502();
    instructions = 0;
    clockticks6502 = 0;
    clockgoal6502 = 0;
    oldpc = 0;
    ea = 0;
    reladdr = 0;
    value = 0;
    result = 0;
    opcode = 0;
    oldstatus = 0;
    penaltyop = 0;
    penaltyaddr = 0;
    hookexternal(0);
}

unsigned int fake6502_instruction_step(void)
{
    return step6502();
}

void fake6502_instruction_irq(void)
{
    irq6502();
}

void fake6502_instruction_nmi(void)
{
    nmi6502();
}

unsigned short fake6502_instruction_pc(void)
{
    return pc;
}

unsigned char fake6502_instruction_s(void)
{
    return sp;
}

unsigned char fake6502_instruction_a(void)
{
    return a;
}

unsigned char fake6502_instruction_x(void)
{
    return x;
}

unsigned char fake6502_instruction_y(void)
{
    return y;
}

unsigned char fake6502_instruction_p(void)
{
    return status;
}
