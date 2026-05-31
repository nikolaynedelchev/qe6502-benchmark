#include <benchmark6502/singlestep.hpp>
#include <benchmark6502/opcode_metadata.hpp>

#include <nlohmann/json.hpp>

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#ifndef BENCHMARK6502_SINGLESTEP_LIGHT_ROOT
#define BENCHMARK6502_SINGLESTEP_LIGHT_ROOT ""
#endif

namespace benchmark6502 {
namespace {

bool file_exists(const std::string& path)
{
    std::ifstream in{path};
    return static_cast<bool>(in);
}

std::uint8_t json_u8(const nlohmann::json& value)
{
    return static_cast<std::uint8_t>(value.get<std::uint32_t>() & 0xffu);
}

std::uint16_t json_u16(const nlohmann::json& value)
{
    return static_cast<std::uint16_t>(value.get<std::uint32_t>() & 0xffffu);
}

singlestep_memory_operation parse_operation(const std::string& text)
{
    if (text == "read") {
        return singlestep_memory_operation::read;
    }
    if (text == "write") {
        return singlestep_memory_operation::write;
    }
    throw std::runtime_error("unknown SingleStep memory operation: " + text);
}

std::vector<singlestep_memory_cell> parse_ram(const nlohmann::json& ram)
{
    if (!ram.is_array()) {
        throw std::runtime_error("SingleStep state ram is not an array");
    }

    std::vector<singlestep_memory_cell> out;
    out.reserve(ram.size());
    for (const auto& entry : ram) {
        if (!entry.is_array() || entry.size() != 2u) {
            throw std::runtime_error("SingleStep ram entry is not [address, value]");
        }
        out.push_back({json_u16(entry.at(0u)), json_u8(entry.at(1u))});
    }
    return out;
}

singlestep_cpu_state parse_state(const nlohmann::json& state)
{
    singlestep_cpu_state out;
    out.pc = json_u16(state.at("pc"));
    out.s = json_u8(state.at("s"));
    out.a = json_u8(state.at("a"));
    out.x = json_u8(state.at("x"));
    out.y = json_u8(state.at("y"));
    out.p = json_u8(state.at("p"));
    out.ram = parse_ram(state.at("ram"));
    return out;
}

std::vector<singlestep_cycle> parse_cycles(const nlohmann::json& cycles)
{
    if (!cycles.is_array()) {
        throw std::runtime_error("SingleStep cycles is not an array");
    }

    std::vector<singlestep_cycle> out;
    out.reserve(cycles.size());
    for (const auto& entry : cycles) {
        if (!entry.is_array() || entry.size() != 3u) {
            throw std::runtime_error("SingleStep cycle entry is not [address, value, operation]");
        }
        out.push_back({json_u16(entry.at(0u)), json_u8(entry.at(1u)), parse_operation(entry.at(2u).get<std::string>())});
    }
    return out;
}

singlestep_case parse_case(const nlohmann::json& value)
{
    singlestep_case out;
    out.name = value.contains("name") ? value.at("name").get<std::string>() : std::string{"<unnamed>"};
    out.initial = parse_state(value.at("initial"));
    out.final = parse_state(value.at("final"));
    out.cycles = parse_cycles(value.at("cycles"));
    return out;
}

std::string model_relative_path(const singlestep_model model)
{
    switch (model) {
    case singlestep_model::nmos6502:
        return "6502/v1";
    }
    return "6502/v1";
}

std::string select_model_path(const singlestep_model model, const std::string& root)
{
    const std::string filename = opcode_json_filename(0x00u);
    const std::string model_path = join_path(root, model_relative_path(model));
    if (file_exists(join_path(model_path, filename))) {
        return model_path;
    }
    if (file_exists(join_path(root, filename))) {
        return root;
    }
    throw std::runtime_error("SingleStep corpus not found under '" + root + "' for model " + singlestep_model_name(model));
}

void record_level(singlestep_level_summary& summary, const singlestep_opcode_result& opcode, const singlestep_opcode_level_result& level)
{
    if (!level.supported || opcode.cases_run == 0u) {
        return;
    }

    const bool legal = is_legal_nmos_opcode(opcode.opcode);
    summary.supported = true;
    summary.opcodes_total++;
    if (level.failed) {
        summary.opcodes_failed++;
    }
    if (legal) {
        summary.legal_total++;
        if (level.failed) {
            summary.legal_failed++;
        }
    } else {
        summary.illegal_total++;
        if (level.failed) {
            summary.illegal_failed++;
        }
    }
}

void print_level(const char* name, const singlestep_level_summary& level)
{
    std::cout << "  " << std::left << std::setw(18) << name;
    if (!level.supported) {
        std::cout << "unsupported\n";
        return;
    }

    std::cout << "total " << level.opcodes_failed << '/' << level.opcodes_total
              << " failed, " << std::fixed << std::setprecision(2) << level.failed_percent() << "%"
              << "  legal " << level.legal_failed << '/' << level.legal_total
              << ", " << std::fixed << std::setprecision(2) << level.legal_failed_percent() << "%"
              << "  illegal " << level.illegal_failed << '/' << level.illegal_total
              << ", " << std::fixed << std::setprecision(2) << level.illegal_failed_percent() << "%\n";
}

} // namespace

std::uint64_t singlestep_corpus::total_cases() const
{
    std::uint64_t total = 0;
    for (const auto& opcode : opcodes) {
        total += static_cast<std::uint64_t>(opcode.cases.size());
    }
    return total;
}

std::uint64_t singlestep_result::cases_run() const
{
    std::uint64_t total = 0;
    for (const auto& opcode : opcodes) {
        total += opcode.cases_run;
    }
    return total;
}

double singlestep_level_summary::failed_percent() const
{
    return opcodes_total == 0u ? 0.0 : 100.0 * static_cast<double>(opcodes_failed) / static_cast<double>(opcodes_total);
}

double singlestep_level_summary::legal_failed_percent() const
{
    return legal_total == 0u ? 0.0 : 100.0 * static_cast<double>(legal_failed) / static_cast<double>(legal_total);
}

double singlestep_level_summary::illegal_failed_percent() const
{
    return illegal_total == 0u ? 0.0 : 100.0 * static_cast<double>(illegal_failed) / static_cast<double>(illegal_total);
}

std::string join_path(const std::string& dir, const std::string& child)
{
    if (dir.empty()) {
        return child;
    }
    if (dir.back() == '/' || dir.back() == '\\') {
        return dir + child;
    }
    return dir + "/" + child;
}

std::string opcode_json_filename(const std::uint8_t opcode)
{
    constexpr char digits[] = "0123456789abcdef";
    std::string out;
    out.push_back(digits[(opcode >> 4u) & 0x0fu]);
    out.push_back(digits[opcode & 0x0fu]);
    out += ".json";
    return out;
}

std::string default_singlestep_light_root()
{
    return BENCHMARK6502_SINGLESTEP_LIGHT_ROOT;
}

singlestep_corpus_options parse_singlestep_corpus_options(const int argc, char** argv)
{
    singlestep_corpus_options out;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if ((arg == "--singlestep-dir" || arg == "--path") && i + 1 < argc) {
            out.explicit_root = argv[++i];
        }
    }
    return out;
}

singlestep_corpus load_singlestep_corpus(const singlestep_model model, const singlestep_corpus_options& options)
{
    singlestep_corpus out;
    out.model = model;

    if (!options.explicit_root.empty()) {
        out.root_path = options.explicit_root;
        out.external = true;
    } else if (const char* env = std::getenv("QE6502_SINGLESTEP_TESTS_DIR")) {
        out.root_path = env;
        out.external = true;
    } else {
        out.root_path = default_singlestep_light_root();
        out.external = false;
    }

    if (out.root_path.empty()) {
        throw std::runtime_error("no SingleStep corpus root configured");
    }

    out.model_path = select_model_path(model, out.root_path);

    for (unsigned opcode_value = 0; opcode_value <= 0xffu; ++opcode_value) {
        const auto opcode = static_cast<std::uint8_t>(opcode_value);
        singlestep_opcode_tests& opcode_tests = out.opcodes[opcode_value];
        opcode_tests.opcode = opcode;

        const std::string filename = join_path(out.model_path, opcode_json_filename(opcode));
        std::ifstream in{filename};
        if (!in) {
            throw std::runtime_error("failed to open SingleStep JSON file: " + filename);
        }

        if (in.peek() == std::ifstream::traits_type::eof()) {
            continue;
        }

        const nlohmann::json root = nlohmann::json::parse(in);
        if (!root.is_array()) {
            throw std::runtime_error(filename + ": root JSON value is not an array");
        }

        opcode_tests.cases.reserve(root.size());
        for (const auto& entry : root) {
            opcode_tests.cases.push_back(parse_case(entry));
        }
    }

    return out;
}

const char* singlestep_model_name(const singlestep_model model)
{
    switch (model) {
    case singlestep_model::nmos6502:
        return "6502 NMOS";
    }
    return "unknown";
}

const char* memory_operation_name(const singlestep_memory_operation operation)
{
    switch (operation) {
    case singlestep_memory_operation::read:
        return "read";
    case singlestep_memory_operation::write:
        return "write";
    }
    return "unknown";
}

singlestep_summary summarize_singlestep_result(const singlestep_result& result)
{
    singlestep_summary summary;
    for (const auto& opcode : result.opcodes) {
        record_level(summary.instruction, opcode, opcode.instruction);
        record_level(summary.cycle_count, opcode, opcode.cycle_count);
        record_level(summary.bus_trace, opcode, opcode.bus_trace);
    }
    return summary;
}

void print_singlestep_result(const singlestep_corpus& corpus, const singlestep_result& result)
{
    std::cout << result.core_name << ' ' << result.model_name << " SingleStep:\n";
    std::cout << "  corpus: " << (corpus.external ? "external " : "bundled light ") << corpus.model_path << "\n";
    std::cout << "  cases:  " << result.cases_run() << "\n";

    if (result.harness_error) {
        std::cout << "  harness error: " << result.error_message << "\n";
        return;
    }

    const singlestep_summary summary = summarize_singlestep_result(result);
    print_level("instruction:", summary.instruction);
    print_level("cycle-count:", summary.cycle_count);
    print_level("bus-trace:", summary.bus_trace);
}

} // namespace benchmark6502
