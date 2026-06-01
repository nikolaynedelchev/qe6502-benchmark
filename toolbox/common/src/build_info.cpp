#include <benchmark6502/build_info.hpp>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#if defined(__APPLE__)
#include <sys/sysctl.h>
#endif

#ifndef BENCHMARK6502_CXX_COMPILER_ID
#define BENCHMARK6502_CXX_COMPILER_ID "unknown"
#endif

#ifndef BENCHMARK6502_CXX_COMPILER_VERSION
#define BENCHMARK6502_CXX_COMPILER_VERSION "unknown"
#endif

#ifndef BENCHMARK6502_CXX_FLAGS
#define BENCHMARK6502_CXX_FLAGS ""
#endif

#ifndef BENCHMARK6502_BUILD_TYPE
#define BENCHMARK6502_BUILD_TYPE "unknown"
#endif

namespace benchmark6502 {
namespace {

std::string trim(std::string value)
{
    const std::string whitespace = " \t\r\n";
    const std::size_t first = value.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        return {};
    }
    const std::size_t last = value.find_last_not_of(whitespace);
    return value.substr(first, last - first + 1u);
}

std::string detected_operating_system()
{
#if defined(_WIN32)
    return "Windows";
#elif defined(__APPLE__)
    return "macOS";
#elif defined(__linux__)
    return "Linux";
#elif defined(__FreeBSD__)
    return "FreeBSD";
#elif defined(__OpenBSD__)
    return "OpenBSD";
#elif defined(__NetBSD__)
    return "NetBSD";
#else
    return "unknown";
#endif
}

std::string detected_architecture()
{
#if defined(__x86_64__) || defined(_M_X64)
    return "x86_64";
#elif defined(__i386__) || defined(_M_IX86)
    return "x86";
#elif defined(__aarch64__) || defined(_M_ARM64)
    return "arm64";
#elif defined(__arm__) || defined(_M_ARM)
    return "arm";
#elif defined(__powerpc64__) || defined(__ppc64__)
    return "ppc64";
#elif defined(__powerpc__) || defined(__ppc__)
    return "ppc";
#else
    return "unknown-arch";
#endif
}

#if defined(__linux__)
std::string linux_cpu_model()
{
    std::ifstream in("/proc/cpuinfo");
    std::string line;
    while (std::getline(in, line)) {
        const std::size_t colon = line.find(':');
        if (colon == std::string::npos) {
            continue;
        }
        const std::string key = trim(line.substr(0, colon));
        if (key == "model name" || key == "Hardware" || key == "Processor") {
            const std::string value = trim(line.substr(colon + 1u));
            if (!value.empty()) {
                return value;
            }
        }
    }
    return {};
}
#endif

#if defined(__APPLE__)
std::string macos_sysctl_string(const char* const name)
{
    std::size_t size = 0;
    if (sysctlbyname(name, nullptr, &size, nullptr, 0) != 0 || size == 0u) {
        return {};
    }
    std::string value(size, '\0');
    if (sysctlbyname(name, value.data(), &size, nullptr, 0) != 0 || size == 0u) {
        return {};
    }
    if (!value.empty() && value.back() == '\0') {
        value.pop_back();
    }
    return value;
}
#endif

#if defined(_WIN32)
std::string windows_cpu_model()
{
    const char* const env_model = std::getenv("PROCESSOR_IDENTIFIER");
    if (env_model != nullptr && env_model[0] != '\0') {
        return env_model;
    }
    SYSTEM_INFO info{};
    GetNativeSystemInfo(&info);
    std::ostringstream out;
    out << "architecture=" << info.wProcessorArchitecture
        << " level=" << info.wProcessorLevel
        << " revision=" << info.wProcessorRevision;
    return out.str();
}
#endif

std::string detected_cpu_model()
{
#if defined(__linux__)
    const std::string model = linux_cpu_model();
    if (!model.empty()) {
        return model;
    }
#elif defined(__APPLE__)
    const std::string brand = macos_sysctl_string("machdep.cpu.brand_string");
    if (!brand.empty()) {
        return brand;
    }
    const std::string hw_model = macos_sysctl_string("hw.model");
    if (!hw_model.empty()) {
        return hw_model;
    }
#elif defined(_WIN32)
    const std::string model = windows_cpu_model();
    if (!model.empty()) {
        return model;
    }
#endif
    return detected_architecture();
}

} // namespace

build_runtime_info collect_build_runtime_info()
{
    build_runtime_info info;
    info.compiler = BENCHMARK6502_CXX_COMPILER_ID;
    info.compiler_version = BENCHMARK6502_CXX_COMPILER_VERSION;
    info.cxx_flags = trim(BENCHMARK6502_CXX_FLAGS);
    if (info.cxx_flags.empty()) {
        info.cxx_flags = "<none reported by CMake>";
    }
    info.build_type = BENCHMARK6502_BUILD_TYPE;
    info.operating_system = detected_operating_system();
    info.cpu_model = detected_cpu_model();
    return info;
}

std::string format_build_runtime_info(const build_runtime_info& info)
{
    std::ostringstream out;
    out << "Build/runtime environment:\n"
        << "  compiler: " << info.compiler << ' ' << info.compiler_version << "\n"
        << "  build type: " << info.build_type << "\n"
        << "  cxx flags: " << info.cxx_flags << "\n"
        << "  operating system: " << info.operating_system << "\n"
        << "  cpu model: " << info.cpu_model << "\n";
    return out.str();
}

std::string format_current_build_runtime_info()
{
    return format_build_runtime_info(collect_build_runtime_info());
}

void print_current_build_runtime_info()
{
    std::cout << format_current_build_runtime_info();
}

} // namespace benchmark6502
