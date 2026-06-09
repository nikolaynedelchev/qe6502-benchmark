#include <fake6502_toolbox.hpp>

#include <cstdint>
#include <memory>

extern "C" {
void fake6502_instruction_clear_memory(unsigned char fill);
void fake6502_instruction_write_memory(unsigned short address, unsigned char value);
unsigned char fake6502_instruction_read_memory(unsigned short address);
void fake6502_instruction_reset(void);
unsigned int fake6502_instruction_step(void);
void fake6502_instruction_irq(void);
void fake6502_instruction_nmi(void);
unsigned short fake6502_instruction_pc(void);
unsigned char fake6502_instruction_s(void);
unsigned char fake6502_instruction_a(void);
unsigned char fake6502_instruction_x(void);
unsigned char fake6502_instruction_y(void);
unsigned char fake6502_instruction_p(void);
}

namespace fake6502_toolbox {
namespace {

class Fake6502InstructionCpu final : public benchmark6502::IInstructionCpu {
public:
    Fake6502InstructionCpu()
    {
        reset();
    }

    std::string_view core_name() const override { return "fake6502"; }
    std::string_view model_name() const override { return "nmos"; }

    void clear_memory(std::uint8_t fill) override
    {
        fake6502_instruction_clear_memory(fill);
    }

    void load(std::span<const asm6502::mem_value> program) override
    {
        for (const auto& [address, value] : program) {
            fake6502_instruction_write_memory(address, value);
        }
    }

    void reset() override
    {
        instruction_count_ = 0;
        clear_scheduled_irq_deassert();
        clear_scheduled_nmi_deassert();
        irq_asserted_ = false;
        fake6502_instruction_reset();
    }

    void step_instruction() override
    {
        if (irq_asserted_) {
            fake6502_instruction_irq();
        }

        fake6502_instruction_step();

        ++instruction_count_;
        apply_scheduled_interrupt_deasserts();
    }

    void assert_irq_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            irq_asserted_ = false;
            clear_scheduled_irq_deassert();
            return;
        }

        irq_asserted_ = true;
        irq_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_irq_deassert_at_ = true;
    }

    void assert_nmi_for(int instruction_count) override
    {
        if (instruction_count <= 0) {
            clear_scheduled_nmi_deassert();
            return;
        }

        fake6502_instruction_nmi();
        nmi_deassert_at_ = instruction_count_ + static_cast<std::uint64_t>(instruction_count);
        has_nmi_deassert_at_ = true;
    }

    std::uint8_t read(std::uint16_t address) const override
    {
        return fake6502_instruction_read_memory(address);
    }

    void write(std::uint16_t address, std::uint8_t value) override
    {
        fake6502_instruction_write_memory(address, value);
    }

    benchmark6502::instruction_cpu_state state() const override
    {
        return benchmark6502::instruction_cpu_state{
            fake6502_instruction_pc(),
            fake6502_instruction_a(),
            fake6502_instruction_x(),
            fake6502_instruction_y(),
            fake6502_instruction_s(),
            fake6502_instruction_p(),
        };
    }

private:
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
            irq_asserted_ = false;
            clear_scheduled_irq_deassert();
        }

        if (has_nmi_deassert_at_ && instruction_count_ >= nmi_deassert_at_) {
            clear_scheduled_nmi_deassert();
        }
    }

    std::uint64_t instruction_count_ = 0;
    std::uint64_t irq_deassert_at_ = 0;
    std::uint64_t nmi_deassert_at_ = 0;
    bool has_irq_deassert_at_ = false;
    bool has_nmi_deassert_at_ = false;
    bool irq_asserted_ = false;
};

} // namespace

std::unique_ptr<benchmark6502::IInstructionCpu> make_fake6502_nmos_instruction_cpu()
{
    return std::make_unique<Fake6502InstructionCpu>();
}

} // namespace fake6502_toolbox
