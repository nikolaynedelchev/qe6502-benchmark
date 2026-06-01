#pragma once

#include <string>

namespace benchmark6502 {

struct build_runtime_info {
    std::string compiler;
    std::string compiler_version;
    std::string cxx_flags;
    std::string build_type;
    std::string operating_system;
    std::string cpu_model;
};

build_runtime_info collect_build_runtime_info();
std::string format_build_runtime_info(const build_runtime_info& info);
std::string format_current_build_runtime_info();
void print_current_build_runtime_info();

} // namespace benchmark6502
