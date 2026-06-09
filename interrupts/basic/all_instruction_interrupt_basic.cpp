#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>
#include <cstdlib>

namespace {

struct Target {
    std::string_view display_name;
    std::string_view executable_name;
};

std::filesystem::path executable_dir(const char* argv0)
{
    std::error_code ec;
    auto path = std::filesystem::absolute(argv0, ec);
    if (ec) {
        path = std::filesystem::path(argv0);
    }
    return path.parent_path();
}

std::string quote_path(const std::filesystem::path& path)
{
    std::string text = path.string();
    std::string quoted = "\"";
    for (const char ch : text) {
        if (ch == '"' || ch == '\\') quoted.push_back('\\');
        quoted.push_back(ch);
    }
    quoted.push_back('"');
    return quoted;
}

int run_one(const std::filesystem::path& dir, const Target& target)
{
    const auto executable = dir / target.executable_name;
    std::cout << "\n=== " << target.display_name << " ===\n";
    if (!std::filesystem::exists(executable)) {
        std::cerr << "missing executable: " << executable << "\n";
        std::cout << "=== " << target.display_name << ": FAIL ===\n";
        return 1;
    }

    const int status = std::system(quote_path(executable).c_str());
    if (status == 0) {
        std::cout << "=== " << target.display_name << ": PASS ===\n";
        return 0;
    }

    std::cout << "=== " << target.display_name << ": FAIL ===\n";
    return 1;
}

} // namespace

int main(int argc, char** argv)
{
    const auto dir = executable_dir(argc > 0 ? argv[0] : "all_instruction_interrupt_basic");
    const std::vector<Target> targets{
        {"qe6502", "qe6502_instruction_interrupt_basic"},
        {"floooh/chips", "floooh_chips_instruction_interrupt_basic"},
        {"fake6502", "fake6502_instruction_interrupt_basic"},
        {"AppleWin", "applewin_instruction_interrupt_basic"},
        {"MAME", "mame_instruction_interrupt_basic"},
        {"CLK P6502", "clk_instruction_interrupt_basic"},
        {"Peddle", "peddle_instruction_interrupt_basic"},
        {"O2", "o2_instruction_interrupt_basic"},
        {"vrEmu6502", "vremu6502_instruction_interrupt_basic"},
        {"chris_pikul_mos6502", "chris_pikul_mos6502_instruction_interrupt_basic"},
        {"olcNES", "olcNES_instruction_interrupt_basic"},
        {"ares", "ares_instruction_interrupt_basic"},
        {"sflib6502", "sflib6502_instruction_interrupt_basic"},
        {"sflib65c02", "sflib65c02_instruction_interrupt_basic"},
        {"fceux", "fceux_instruction_interrupt_basic"},
        {"gianlucag_mos6502", "gianlucag_mos6502_instruction_interrupt_basic"},
    };

    int result = 0;
    for (const auto& target : targets) {
        result |= run_one(dir, target);
    }
    return result;
}
