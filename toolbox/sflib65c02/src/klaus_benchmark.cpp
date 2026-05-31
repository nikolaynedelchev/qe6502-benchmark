#include <sflib65c02_toolbox.hpp>

extern "C" {
#include <lib6502.h>
}

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>

#if !defined(_WIN32)
#include <fcntl.h>
#include <unistd.h>
#endif

static const std::uint8_t s_klaus_65c02_extended_rom[0x10000] =
#include "cores/qe6502/harness/klaus2m5/65C02_extended_opcodes_test.hex"

namespace {

constexpr std::uint16_t klaus_start_address = 0x0400u;
constexpr std::uint16_t klaus_success_address = 0x24F1u;
constexpr std::uint64_t max_instructions = 100000000ull;
constexpr std::uint64_t max_cycles = 2ull * 21986985ull;

int client_read(M6502* cpu, const std::uint16_t address, std::uint8_t)
{
    return cpu->memory[address];
}

int client_write(M6502* cpu, const std::uint16_t address, const std::uint8_t data)
{
    cpu->memory[address] = data;
    return data;
}

void install_client_memory_callbacks(M6502* cpu)
{
    for (std::size_t address = 0; address < 0x10000u; ++address) {
        M6502_setCallback(cpu, read, address, client_read);
        M6502_setCallback(cpu, write, address, client_write);
    }
}


bool is_sflib6502_undefined_opcode(const std::uint8_t opcode)
{
    switch (opcode) {
    case 0x02:
    case 0x03:
    case 0x07:
    case 0x0B:
    case 0x0F:
    case 0x13:
    case 0x17:
    case 0x1B:
    case 0x1F:
    case 0x22:
    case 0x23:
    case 0x27:
    case 0x2B:
    case 0x2F:
    case 0x33:
    case 0x37:
    case 0x3B:
    case 0x3F:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x47:
    case 0x4B:
    case 0x4F:
    case 0x53:
    case 0x54:
    case 0x57:
    case 0x5B:
    case 0x5C:
    case 0x5F:
    case 0x62:
    case 0x63:
    case 0x67:
    case 0x6B:
    case 0x6F:
    case 0x73:
    case 0x77:
    case 0x7B:
    case 0x7F:
    case 0x82:
    case 0x83:
    case 0x87:
    case 0x8B:
    case 0x8F:
    case 0x93:
    case 0x97:
    case 0x9B:
    case 0x9F:
    case 0xA3:
    case 0xA7:
    case 0xAB:
    case 0xAF:
    case 0xB3:
    case 0xB7:
    case 0xBB:
    case 0xBF:
    case 0xC2:
    case 0xC3:
    case 0xC7:
    case 0xCB:
    case 0xCF:
    case 0xD3:
    case 0xD4:
    case 0xD7:
    case 0xDB:
    case 0xDC:
    case 0xDF:
    case 0xE2:
    case 0xE3:
    case 0xE7:
    case 0xEB:
    case 0xEF:
    case 0xF3:
    case 0xF4:
    case 0xF7:
    case 0xFB:
    case 0xFC:
    case 0xFF:
        return true;
    default:
        return false;
    }
}

class scoped_stderr_silencer {
public:
    scoped_stderr_silencer()
    {
#if !defined(_WIN32)
        ::fflush(stderr);
        saved_fd_ = ::dup(STDERR_FILENO);
        null_fd_ = ::open("/dev/null", O_WRONLY);
        if (saved_fd_ >= 0 && null_fd_ >= 0) {
            ::dup2(null_fd_, STDERR_FILENO);
        }
#endif
    }

    scoped_stderr_silencer(const scoped_stderr_silencer&) = delete;
    scoped_stderr_silencer& operator=(const scoped_stderr_silencer&) = delete;

    ~scoped_stderr_silencer()
    {
#if !defined(_WIN32)
        ::fflush(stderr);
        if (saved_fd_ >= 0) {
            ::dup2(saved_fd_, STDERR_FILENO);
            ::close(saved_fd_);
        }
        if (null_fd_ >= 0) {
            ::close(null_fd_);
        }
#endif
    }

private:
#if !defined(_WIN32)
    int saved_fd_ = -1;
    int null_fd_ = -1;
#endif
};

struct run_result {
    bool passed;
    std::uint64_t cycles;
};

run_result run_klaus_once()
{
    scoped_stderr_silencer silence_undefined_instruction_spam;
    M6502_Registers registers{};
    M6502_Memory memory{};
    M6502_Callbacks callbacks{};

    std::memcpy(memory, s_klaus_65c02_extended_rom, sizeof(memory));

    M6502* cpu = M6502_new(&registers, memory, &callbacks);
    if (cpu == nullptr) {
        return {false, 0};
    }
    install_client_memory_callbacks(cpu);

    M6502_reset(cpu);
    registers.pc = klaus_start_address;

    std::uint64_t cycles = 0;

    for (std::uint64_t i = 0; i < max_instructions; ++i) {
        if (registers.pc == klaus_success_address) {
            M6502_delete(cpu);
            return {true, cycles};
        }

        if (is_sflib6502_undefined_opcode(memory[registers.pc])) {
            M6502_delete(cpu);
            return {false, cycles};
        }

        cpu->ticks = 0;
        M6502_step(cpu);
        cycles += cpu->ticks;
        if (cycles > max_cycles) {
            M6502_delete(cpu);
            return {false, cycles};
        }
    }

    M6502_delete(cpu);
    return {false, cycles};
}

} // namespace

namespace sflib65c02_toolbox {

benchmark6502::klaus_benchmark_result run_klaus_65c02_extended(const int measured_runs)
{
    const run_result cold = run_klaus_once();
    if (!cold.passed) {
        return {false, measured_runs, 0, 0.0};
    }

    std::uint64_t total_cycles = 0;
    const auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < measured_runs; ++i) {
        const run_result result = run_klaus_once();
        if (!result.passed) {
            return {false, measured_runs, total_cycles, 0.0};
        }
        total_cycles += result.cycles;
    }

    const auto stop = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed = stop - start;

    return {true, measured_runs, total_cycles, elapsed.count()};
}

} // namespace sflib65c02_toolbox
