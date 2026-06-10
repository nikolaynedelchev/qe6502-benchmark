#include "common.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <ostream>
#include <sstream>

namespace interrupt_precision {

std::string hex8(std::uint8_t value)
{
    std::ostringstream out;
    out << '$' << std::uppercase << std::hex << std::setw(2) << std::setfill('0')
        << static_cast<unsigned>(value);
    return out.str();
}

std::string hex16(std::uint16_t value)
{
    std::ostringstream out;
    out << '$' << std::uppercase << std::hex << std::setw(4) << std::setfill('0')
        << static_cast<unsigned>(value);
    return out.str();
}

std::string tsv_hex8(std::uint8_t value)
{
    std::ostringstream out;
    out << "0x" << std::uppercase << std::hex << std::setw(2) << std::setfill('0')
        << static_cast<unsigned>(value);
    return out.str();
}

std::string sanitize_path_component(std::string_view text)
{
    std::string sanitized;
    sanitized.reserve(text.size());
    bool last_was_separator = false;
    for (const char ch : text) {
        const auto unsigned_ch = static_cast<unsigned char>(ch);
        if (std::isalnum(unsigned_ch)) {
            sanitized.push_back(static_cast<char>(std::tolower(unsigned_ch)));
            last_was_separator = false;
            continue;
        }
        if (!last_was_separator) {
            sanitized.push_back('_');
            last_was_separator = true;
        }
    }

    while (!sanitized.empty() && sanitized.back() == '_') {
        sanitized.pop_back();
    }
    while (!sanitized.empty() && sanitized.front() == '_') {
        sanitized.erase(sanitized.begin());
    }
    return sanitized.empty() ? "unknown" : sanitized;
}

std::string default_detail_log_path(const SuiteContext& context)
{
    return "interrupt_precision_" + sanitize_path_component(context.core_name) + "_" +
           sanitize_path_component(context.model_name) + "_" +
           sanitize_path_component(context.suite_name) + ".tsv";
}

std::string resolve_detail_log_path(const SuiteContext& context)
{
    return context.detail_log_path.empty() ? default_detail_log_path(context) : context.detail_log_path;
}

const char* yes_no(bool value) noexcept
{
    return value ? "yes" : "no";
}

const char* pass_fail(bool passed) noexcept
{
    return passed ? "passed" : "failed";
}


namespace {

bool same_public_bus(const tools6502::CpuTraceEntry& left,
                     const tools6502::CpuTraceEntry& right) noexcept
{
    return left.write == right.write
        && left.address == right.address
        && left.data == right.data;
}

void add_secondary_from_entry_pair(SecondaryMismatchStats& stats,
                                   const tools6502::CpuTraceEntry& left,
                                   const tools6502::CpuTraceEntry& right) noexcept
{
    // Only classify secondary/metadata differences after the public bus identity
    // has matched. If rw/address/data differ, that is a primary bus mismatch.
    if (!same_public_bus(left, right)) {
        return;
    }

    bool any = false;
    if (left.opcode_fetch != right.opcode_fetch) {
        ++stats.opcode_fetch;
        any = true;
    }
    if (left.irq_asserted != right.irq_asserted) {
        ++stats.irq_line;
        any = true;
    }
    if (left.nmi_asserted != right.nmi_asserted) {
        ++stats.nmi_line;
        any = true;
    }
    if (any) {
        ++stats.total;
    }
}

} // namespace

namespace {

void print_entry(std::ostream& out,
                 const tools6502::CpuTraceEntry& entry)
{
    out << "cycle=" << std::dec << entry.cycle
        << ' ' << (entry.opcode_fetch ? "FETCH" : "     ")
        << ' ' << (entry.write ? 'W' : 'R')
        << " addr=" << hex16(entry.address)
        << " data=" << hex8(entry.data)
        << " irq=" << (entry.irq_asserted ? '1' : '0')
        << " nmi=" << (entry.nmi_asserted ? '1' : '0');
}

} // namespace


SecondaryMismatchStats count_secondary_mismatches(const tools6502::LockstepRunResult& result)
{
    SecondaryMismatchStats stats{};
    const std::size_t trace_size = std::min(result.left_trace.size(), result.right_trace.size());
    for (std::size_t index = 0u; index < trace_size; ++index) {
        add_secondary_from_entry_pair(stats, result.left_trace[index], result.right_trace[index]);
    }
    return stats;
}

SecondaryMismatchStats count_secondary_mismatches(const tools6502::LockstepScenarioResult& result)
{
    SecondaryMismatchStats stats{};
    for (const auto& command_result : result.results) {
        add_secondary_mismatches(stats, count_secondary_mismatches(command_result));
    }
    return stats;
}

void add_secondary_mismatches(SecondaryMismatchStats& destination,
                              const SecondaryMismatchStats& source) noexcept
{
    destination.total += source.total;
    destination.opcode_fetch += source.opcode_fetch;
    destination.irq_line += source.irq_line;
    destination.nmi_line += source.nmi_line;
}

void print_failure_trace(std::ostream& out,
                         const tools6502::LockstepRunResult& result,
                         std::size_t context_before,
                         std::size_t context_after)
{
    if (!result.first_mismatch) {
        out << "no trace mismatch index available\n";
        return;
    }

    const std::size_t mismatch = *result.first_mismatch;
    const std::size_t trace_size = std::min(result.left_trace.size(), result.right_trace.size());
    const std::size_t begin = mismatch > context_before ? mismatch - context_before : 0u;
    const std::size_t end = std::min(trace_size, mismatch + context_after + 1u);

    for (std::size_t index = begin; index < end; ++index) {
        out << (index == mismatch ? "  -> " : "     ") << "left  ";
        print_entry(out, result.left_trace[index]);
        out << '\n';
        out << (index == mismatch ? "  -> " : "     ") << "right ";
        print_entry(out, result.right_trace[index]);
        out << '\n';
    }
}

std::vector<tools6502::LockstepCommand> make_pulse_script(bool nmi,
                                                          std::size_t pulse_start_cycle,
                                                          std::size_t pulse_length,
                                                          std::size_t total_steps)
{
    std::vector<tools6502::LockstepCommand> script;
    if (pulse_start_cycle > 0u) {
        script.push_back(tools6502::StepCycles{pulse_start_cycle});
    }

    script.push_back(nmi
        ? tools6502::LockstepCommand{tools6502::NmiAssert{}}
        : tools6502::LockstepCommand{tools6502::IrqAssert{}});

    if (pulse_length > 0u) {
        script.push_back(tools6502::StepCycles{pulse_length});
    }

    script.push_back(nmi
        ? tools6502::LockstepCommand{tools6502::NmiDeassert{}}
        : tools6502::LockstepCommand{tools6502::IrqDeassert{}});

    const std::size_t used = pulse_start_cycle + pulse_length;
    if (used < total_steps) {
        script.push_back(tools6502::StepCycles{total_steps - used});
    }

    return script;
}

} // namespace interrupt_precision
