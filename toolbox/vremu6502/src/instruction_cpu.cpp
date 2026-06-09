#include <vremu6502_toolbox.hpp>

#include <vrEmu6502.h>

#include <array>
#include <cstdint>
#include <memory>

struct vrEmu6502Opcode;

struct vrEmu6502_s
{
    vrEmu6502Model model;

    vrEmu6502MemRead readFn;
    vrEmu6502MemWrite writeFn;

    vrEmu6502Interrupt intPin;
    vrEmu6502Interrupt nmiPin;

    std::uint8_t step;
    std::uint8_t currentOpcode;
    std::uint16_t currentOpcodeAddr;

    bool wai;
    bool stp;

    std::uint16_t pc;

    std::uint8_t ac;
    std::uint8_t ix;
    std::uint8_t iy;
    std::uint8_t sp;

    std::uint8_t flags;

    std::uint16_t zpBase;
    std::uint16_t spBase;
    std::uint16_t tmpAddr;

    const vrEmu6502Opcode* opcodes;
    const char* mnemonicNames[256];
    vrEmu6502AddrMode addrModes[256];
};

namespace vremu6502_toolbox {
namespace {

struct MemoryContext {
    std::array<std::uint8_t, 65536>* memory = nullptr;
};

thread_local MemoryContext* g_memory_context = nullptr;

std::uint8_t memory_read(const std::uint16_t address, const bool /*is_debug*/)
{
    return (*g_memory_context->memory)[address];
}

void memory_write(const std::uint16_t address, const std::uint8_t value)
{
    (*g_memory_context->memory)[address] = value;
}

class ScopedMemoryContext final {
public:
    explicit ScopedMemoryContext(MemoryContext& context)
        : previous_(g_memory_context)
    {
        g_memory_context = &context;
    }

    ~ScopedMemoryContext()
    {
        g_memory_context = previous_;
    }

    ScopedMemoryContext(const ScopedMemoryContext&) = delete;
    ScopedMemoryContext& operator=(const ScopedMemoryContext&) = delete;

private:
    MemoryContext* previous_ = nullptr;
};

struct VrEmu6502Destroyer {
    void operator()(VrEmu6502* cpu) const noexcept
    {
        vrEmu6502Destroy(cpu);
    }
};

class VrEmu6502InstructionCpu final : public benchmark6502::IInstructionCpu {
public:
    VrEmu6502InstructionCpu()
    {
        memory_context_.memory = &memory_;
        ScopedMemoryContext active_context(memory_context_);
        cpu_.reset(vrEmu6502New(CPU_6502U, memory_read, memory_write));
        reset();
    }

    std::string_view core_name() const override { return "vrEmu6502"; }
    std::string_view model_name() const override { return "NMOS undocumented"; }

    void clear_memory(std::uint8_t fill) override
    {
        memory_.fill(fill);
    }

    void load(std::span<const asm6502::mem_value> program) override
    {
        for (const auto& [address, value] : program) {
            memory_[address] = value;
        }
    }

    void reset() override
    {
        ScopedMemoryContext active_context(memory_context_);
        instruction_count_ = 0;
        clear_scheduled_irq_deassert();
        clear_scheduled_nmi_deassert();
        initialise_reset_state();
    }

    void step_instruction() override
    {
        ScopedMemoryContext active_context(memory_context_);
        vrEmu6502InstCycle(cpu_.get());
        ++instruction_count_;
        apply_scheduled_interrupt_deasserts();
    }

    void assert_irq_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            *vrEmu6502Int(cpu_.get()) = IntCleared;
            clear_scheduled_irq_deassert();
            return;
        }

        *vrEmu6502Int(cpu_.get()) = IntRequested;
        irq_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_irq_deassert_at_ = true;
    }

    void assert_nmi_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            *vrEmu6502Nmi(cpu_.get()) = IntCleared;
            clear_scheduled_nmi_deassert();
            return;
        }

        *vrEmu6502Nmi(cpu_.get()) = IntRequested;
        nmi_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_nmi_deassert_at_ = true;
    }

    std::uint8_t read(std::uint16_t address) const override
    {
        return memory_[address];
    }

    void write(std::uint16_t address, std::uint8_t value) override
    {
        memory_[address] = value;
    }

    benchmark6502::instruction_cpu_state state() const override
    {
        return benchmark6502::instruction_cpu_state{
            vrEmu6502GetPC(cpu_.get()),
            vrEmu6502GetAcc(cpu_.get()),
            vrEmu6502GetX(cpu_.get()),
            vrEmu6502GetY(cpu_.get()),
            vrEmu6502GetStackPointer(cpu_.get()),
            vrEmu6502GetStatus(cpu_.get()),
        };
    }

private:
    void initialise_reset_state()
    {
        cpu_->intPin = IntCleared;
        cpu_->nmiPin = IntCleared;
        cpu_->step = 0;
        cpu_->currentOpcode = 0;
        cpu_->currentOpcodeAddr = reset_vector();
        cpu_->wai = false;
        cpu_->stp = false;
        cpu_->pc = reset_vector();
        cpu_->ac = 0;
        cpu_->ix = 0;
        cpu_->iy = 0;
        cpu_->sp = 0xfd;
        cpu_->flags = static_cast<std::uint8_t>(FlagI | FlagB | FlagU);
        cpu_->tmpAddr = 0;
    }

    std::uint16_t reset_vector() const
    {
        return static_cast<std::uint16_t>(memory_[0xfffcu] |
                                          (static_cast<std::uint16_t>(memory_[0xfffdu]) << 8u));
    }

    void clear_scheduled_irq_deassert()
    {
        has_irq_deassert_at_ = false;
        irq_deassert_at_ = 0;
    }

    void clear_scheduled_nmi_deassert()
    {
        has_nmi_deassert_at_ = false;
        nmi_deassert_at_ = 0;
    }

    void apply_scheduled_interrupt_deasserts()
    {
        if (has_irq_deassert_at_ && instruction_count_ >= irq_deassert_at_) {
            *vrEmu6502Int(cpu_.get()) = IntCleared;
            clear_scheduled_irq_deassert();
        }

        if (has_nmi_deassert_at_ && instruction_count_ >= nmi_deassert_at_) {
            *vrEmu6502Nmi(cpu_.get()) = IntCleared;
            clear_scheduled_nmi_deassert();
        }
    }

    std::array<std::uint8_t, 65536> memory_{};
    MemoryContext memory_context_{};
    std::unique_ptr<VrEmu6502, VrEmu6502Destroyer> cpu_;
    std::uint64_t instruction_count_ = 0;
    std::uint64_t irq_deassert_at_ = 0;
    std::uint64_t nmi_deassert_at_ = 0;
    bool has_irq_deassert_at_ = false;
    bool has_nmi_deassert_at_ = false;
};

} // namespace

std::unique_ptr<benchmark6502::IInstructionCpu> make_vremu6502_nmos_instruction_cpu()
{
    return std::make_unique<VrEmu6502InstructionCpu>();
}

} // namespace vremu6502_toolbox
