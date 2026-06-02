#include <benchmark6502/opcode_metadata.hpp>

#include <iostream>

int main()
{
    if (benchmark6502::nmos_legal_opcode_count() != 151) {
        std::cerr << "Expected 151 documented NMOS opcodes\n";
        return 1;
    }

    if (!benchmark6502::is_legal_nmos_opcode(0xA9)) {
        std::cerr << "LDA immediate must be legal\n";
        return 1;
    }

    if (!benchmark6502::is_legal_nmos_opcode(0x6C)) {
        std::cerr << "JMP indirect must be legal\n";
        return 1;
    }

    if (benchmark6502::is_legal_nmos_opcode(0x02)) {
        std::cerr << "0x02 must not be documented NMOS legal\n";
        return 1;
    }

    const auto& lda_imm = benchmark6502::nmos_opcode_metadata(0xA9);
    if (lda_imm.mnemonic[0] != 'L' || lda_imm.bytes != 2 || lda_imm.cycles != 2) {
        std::cerr << "Unexpected LDA immediate metadata\n";
        return 1;
    }

    std::cout << "NMOS opcode metadata smoke test passed\n";
    return 0;
}
