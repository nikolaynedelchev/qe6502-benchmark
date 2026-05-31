#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace benchmark6502 {

enum class singlestep_model {
    nmos6502,
};

enum class singlestep_memory_operation {
    read,
    write,
};

struct singlestep_memory_cell {
    std::uint16_t address = 0;
    std::uint8_t value = 0;
};

struct singlestep_cycle {
    std::uint16_t address = 0;
    std::uint8_t value = 0;
    singlestep_memory_operation operation = singlestep_memory_operation::read;
};

struct singlestep_cpu_state {
    std::uint16_t pc = 0;
    std::uint8_t s = 0;
    std::uint8_t a = 0;
    std::uint8_t x = 0;
    std::uint8_t y = 0;
    std::uint8_t p = 0;
    std::vector<singlestep_memory_cell> ram;
};

struct singlestep_case {
    std::string name;
    singlestep_cpu_state initial;
    singlestep_cpu_state final;
    std::vector<singlestep_cycle> cycles;
};

struct singlestep_opcode_tests {
    std::uint8_t opcode = 0;
    std::vector<singlestep_case> cases;
};

struct singlestep_corpus {
    singlestep_model model = singlestep_model::nmos6502;
    std::string root_path;
    std::string model_path;
    bool external = false;
    std::array<singlestep_opcode_tests, 256> opcodes{};

    std::uint64_t total_cases() const;
};

struct singlestep_corpus_options {
    std::string explicit_root;
};

struct singlestep_opcode_level_result {
    bool supported = false;
    bool failed = false;
    std::uint64_t failed_cases = 0;
};

struct singlestep_opcode_result {
    std::uint8_t opcode = 0;
    std::uint64_t cases_run = 0;
    singlestep_opcode_level_result instruction;
    singlestep_opcode_level_result cycle_count;
    singlestep_opcode_level_result bus_trace;
};

struct singlestep_result {
    std::string core_name;
    std::string model_name;
    std::array<singlestep_opcode_result, 256> opcodes{};
    bool harness_error = false;
    std::string error_message;

    std::uint64_t cases_run() const;
};

struct singlestep_level_summary {
    bool supported = false;
    std::uint32_t opcodes_total = 0;
    std::uint32_t opcodes_failed = 0;
    std::uint32_t legal_total = 0;
    std::uint32_t legal_failed = 0;
    std::uint32_t illegal_total = 0;
    std::uint32_t illegal_failed = 0;

    double failed_percent() const;
    double legal_failed_percent() const;
    double illegal_failed_percent() const;
};

struct singlestep_summary {
    singlestep_level_summary instruction;
    singlestep_level_summary cycle_count;
    singlestep_level_summary bus_trace;
};

std::string join_path(const std::string& dir, const std::string& child);
std::string opcode_json_filename(std::uint8_t opcode);
std::string default_singlestep_light_root();

singlestep_corpus_options parse_singlestep_corpus_options(int argc, char** argv);
singlestep_corpus load_singlestep_corpus(singlestep_model model, const singlestep_corpus_options& options);

const char* singlestep_model_name(singlestep_model model);
const char* memory_operation_name(singlestep_memory_operation operation);

singlestep_summary summarize_singlestep_result(const singlestep_result& result);
void print_singlestep_result(const singlestep_corpus& corpus, const singlestep_result& result);
std::string default_singlestep_detail_log_path(const singlestep_result& result);
void write_singlestep_detail_log(const singlestep_corpus& corpus, const singlestep_result& result, const std::string& path);
std::string write_singlestep_detail_log(const singlestep_corpus& corpus, const singlestep_result& result);

} // namespace benchmark6502
