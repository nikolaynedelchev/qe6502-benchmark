#pragma once

#include <array>
#include <cstdint>

namespace benchmark6502 {

enum class addressing_mode {
    illegal,
    implied,
    accumulator,
    immediate,
    zero_page,
    zero_page_x,
    zero_page_y,
    relative,
    absolute,
    absolute_x,
    absolute_y,
    indirect,
    indexed_indirect,
    indirect_indexed,
};

struct opcode_metadata {
    std::uint8_t opcode = 0;
    const char* mnemonic = "???";
    addressing_mode mode = addressing_mode::illegal;
    std::uint8_t bytes = 1;
    std::uint8_t cycles = 0;
    bool legal_nmos = false;
    bool page_cross_extra_cycle = false;
    bool branch_extra_cycle = false;
};

constexpr const char* addressing_mode_name(const addressing_mode mode)
{
    switch (mode) {
    case addressing_mode::illegal: return "illegal";
    case addressing_mode::implied: return "implied";
    case addressing_mode::accumulator: return "accumulator";
    case addressing_mode::immediate: return "immediate";
    case addressing_mode::zero_page: return "zero page";
    case addressing_mode::zero_page_x: return "zero page,X";
    case addressing_mode::zero_page_y: return "zero page,Y";
    case addressing_mode::relative: return "relative";
    case addressing_mode::absolute: return "absolute";
    case addressing_mode::absolute_x: return "absolute,X";
    case addressing_mode::absolute_y: return "absolute,Y";
    case addressing_mode::indirect: return "indirect";
    case addressing_mode::indexed_indirect: return "(zero page,X)";
    case addressing_mode::indirect_indexed: return "(zero page),Y";
    }
    return "illegal";
}

constexpr std::array<opcode_metadata, 256> make_nmos_opcode_metadata_table()
{
    return {{
        {0x00u, "BRK", addressing_mode::implied, 1u, 7u, true, false, false},
        {0x01u, "ORA", addressing_mode::indexed_indirect, 2u, 6u, true, false, false},
        {0x02u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x03u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x04u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x05u, "ORA", addressing_mode::zero_page, 2u, 3u, true, false, false},
        {0x06u, "ASL", addressing_mode::zero_page, 2u, 5u, true, false, false},
        {0x07u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x08u, "PHP", addressing_mode::implied, 1u, 3u, true, false, false},
        {0x09u, "ORA", addressing_mode::immediate, 2u, 2u, true, false, false},
        {0x0Au, "ASL", addressing_mode::accumulator, 1u, 2u, true, false, false},
        {0x0Bu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x0Cu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x0Du, "ORA", addressing_mode::absolute, 3u, 4u, true, false, false},
        {0x0Eu, "ASL", addressing_mode::absolute, 3u, 6u, true, false, false},
        {0x0Fu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x10u, "BPL", addressing_mode::relative, 2u, 2u, true, false, true},
        {0x11u, "ORA", addressing_mode::indirect_indexed, 2u, 5u, true, true, false},
        {0x12u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x13u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x14u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x15u, "ORA", addressing_mode::zero_page_x, 2u, 4u, true, false, false},
        {0x16u, "ASL", addressing_mode::zero_page_x, 2u, 6u, true, false, false},
        {0x17u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x18u, "CLC", addressing_mode::implied, 1u, 2u, true, false, false},
        {0x19u, "ORA", addressing_mode::absolute_y, 3u, 4u, true, true, false},
        {0x1Au, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x1Bu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x1Cu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x1Du, "ORA", addressing_mode::absolute_x, 3u, 4u, true, true, false},
        {0x1Eu, "ASL", addressing_mode::absolute_x, 3u, 7u, true, false, false},
        {0x1Fu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x20u, "JSR", addressing_mode::absolute, 3u, 6u, true, false, false},
        {0x21u, "AND", addressing_mode::indexed_indirect, 2u, 6u, true, false, false},
        {0x22u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x23u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x24u, "BIT", addressing_mode::zero_page, 2u, 3u, true, false, false},
        {0x25u, "AND", addressing_mode::zero_page, 2u, 3u, true, false, false},
        {0x26u, "ROL", addressing_mode::zero_page, 2u, 5u, true, false, false},
        {0x27u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x28u, "PLP", addressing_mode::implied, 1u, 4u, true, false, false},
        {0x29u, "AND", addressing_mode::immediate, 2u, 2u, true, false, false},
        {0x2Au, "ROL", addressing_mode::accumulator, 1u, 2u, true, false, false},
        {0x2Bu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x2Cu, "BIT", addressing_mode::absolute, 3u, 4u, true, false, false},
        {0x2Du, "AND", addressing_mode::absolute, 3u, 4u, true, false, false},
        {0x2Eu, "ROL", addressing_mode::absolute, 3u, 6u, true, false, false},
        {0x2Fu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x30u, "BMI", addressing_mode::relative, 2u, 2u, true, false, true},
        {0x31u, "AND", addressing_mode::indirect_indexed, 2u, 5u, true, true, false},
        {0x32u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x33u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x34u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x35u, "AND", addressing_mode::zero_page_x, 2u, 4u, true, false, false},
        {0x36u, "ROL", addressing_mode::zero_page_x, 2u, 6u, true, false, false},
        {0x37u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x38u, "SEC", addressing_mode::implied, 1u, 2u, true, false, false},
        {0x39u, "AND", addressing_mode::absolute_y, 3u, 4u, true, true, false},
        {0x3Au, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x3Bu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x3Cu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x3Du, "AND", addressing_mode::absolute_x, 3u, 4u, true, true, false},
        {0x3Eu, "ROL", addressing_mode::absolute_x, 3u, 7u, true, false, false},
        {0x3Fu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x40u, "RTI", addressing_mode::implied, 1u, 6u, true, false, false},
        {0x41u, "EOR", addressing_mode::indexed_indirect, 2u, 6u, true, false, false},
        {0x42u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x43u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x44u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x45u, "EOR", addressing_mode::zero_page, 2u, 3u, true, false, false},
        {0x46u, "LSR", addressing_mode::zero_page, 2u, 5u, true, false, false},
        {0x47u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x48u, "PHA", addressing_mode::implied, 1u, 3u, true, false, false},
        {0x49u, "EOR", addressing_mode::immediate, 2u, 2u, true, false, false},
        {0x4Au, "LSR", addressing_mode::accumulator, 1u, 2u, true, false, false},
        {0x4Bu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x4Cu, "JMP", addressing_mode::absolute, 3u, 3u, true, false, false},
        {0x4Du, "EOR", addressing_mode::absolute, 3u, 4u, true, false, false},
        {0x4Eu, "LSR", addressing_mode::absolute, 3u, 6u, true, false, false},
        {0x4Fu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x50u, "BVC", addressing_mode::relative, 2u, 2u, true, false, true},
        {0x51u, "EOR", addressing_mode::indirect_indexed, 2u, 5u, true, true, false},
        {0x52u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x53u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x54u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x55u, "EOR", addressing_mode::zero_page_x, 2u, 4u, true, false, false},
        {0x56u, "LSR", addressing_mode::zero_page_x, 2u, 6u, true, false, false},
        {0x57u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x58u, "CLI", addressing_mode::implied, 1u, 2u, true, false, false},
        {0x59u, "EOR", addressing_mode::absolute_y, 3u, 4u, true, true, false},
        {0x5Au, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x5Bu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x5Cu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x5Du, "EOR", addressing_mode::absolute_x, 3u, 4u, true, true, false},
        {0x5Eu, "LSR", addressing_mode::absolute_x, 3u, 7u, true, false, false},
        {0x5Fu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x60u, "RTS", addressing_mode::implied, 1u, 6u, true, false, false},
        {0x61u, "ADC", addressing_mode::indexed_indirect, 2u, 6u, true, false, false},
        {0x62u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x63u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x64u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x65u, "ADC", addressing_mode::zero_page, 2u, 3u, true, false, false},
        {0x66u, "ROR", addressing_mode::zero_page, 2u, 5u, true, false, false},
        {0x67u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x68u, "PLA", addressing_mode::implied, 1u, 4u, true, false, false},
        {0x69u, "ADC", addressing_mode::immediate, 2u, 2u, true, false, false},
        {0x6Au, "ROR", addressing_mode::accumulator, 1u, 2u, true, false, false},
        {0x6Bu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x6Cu, "JMP", addressing_mode::indirect, 3u, 5u, true, false, false},
        {0x6Du, "ADC", addressing_mode::absolute, 3u, 4u, true, false, false},
        {0x6Eu, "ROR", addressing_mode::absolute, 3u, 6u, true, false, false},
        {0x6Fu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x70u, "BVS", addressing_mode::relative, 2u, 2u, true, false, true},
        {0x71u, "ADC", addressing_mode::indirect_indexed, 2u, 5u, true, true, false},
        {0x72u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x73u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x74u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x75u, "ADC", addressing_mode::zero_page_x, 2u, 4u, true, false, false},
        {0x76u, "ROR", addressing_mode::zero_page_x, 2u, 6u, true, false, false},
        {0x77u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x78u, "SEI", addressing_mode::implied, 1u, 2u, true, false, false},
        {0x79u, "ADC", addressing_mode::absolute_y, 3u, 4u, true, true, false},
        {0x7Au, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x7Bu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x7Cu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x7Du, "ADC", addressing_mode::absolute_x, 3u, 4u, true, true, false},
        {0x7Eu, "ROR", addressing_mode::absolute_x, 3u, 7u, true, false, false},
        {0x7Fu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x80u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x81u, "STA", addressing_mode::indexed_indirect, 2u, 6u, true, false, false},
        {0x82u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x83u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x84u, "STY", addressing_mode::zero_page, 2u, 3u, true, false, false},
        {0x85u, "STA", addressing_mode::zero_page, 2u, 3u, true, false, false},
        {0x86u, "STX", addressing_mode::zero_page, 2u, 3u, true, false, false},
        {0x87u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x88u, "DEY", addressing_mode::implied, 1u, 2u, true, false, false},
        {0x89u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x8Au, "TXA", addressing_mode::implied, 1u, 2u, true, false, false},
        {0x8Bu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x8Cu, "STY", addressing_mode::absolute, 3u, 4u, true, false, false},
        {0x8Du, "STA", addressing_mode::absolute, 3u, 4u, true, false, false},
        {0x8Eu, "STX", addressing_mode::absolute, 3u, 4u, true, false, false},
        {0x8Fu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x90u, "BCC", addressing_mode::relative, 2u, 2u, true, false, true},
        {0x91u, "STA", addressing_mode::indirect_indexed, 2u, 6u, true, false, false},
        {0x92u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x93u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x94u, "STY", addressing_mode::zero_page_x, 2u, 4u, true, false, false},
        {0x95u, "STA", addressing_mode::zero_page_x, 2u, 4u, true, false, false},
        {0x96u, "STX", addressing_mode::zero_page_y, 2u, 4u, true, false, false},
        {0x97u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x98u, "TYA", addressing_mode::implied, 1u, 2u, true, false, false},
        {0x99u, "STA", addressing_mode::absolute_y, 3u, 5u, true, false, false},
        {0x9Au, "TXS", addressing_mode::implied, 1u, 2u, true, false, false},
        {0x9Bu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x9Cu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x9Du, "STA", addressing_mode::absolute_x, 3u, 5u, true, false, false},
        {0x9Eu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0x9Fu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xA0u, "LDY", addressing_mode::immediate, 2u, 2u, true, false, false},
        {0xA1u, "LDA", addressing_mode::indexed_indirect, 2u, 6u, true, false, false},
        {0xA2u, "LDX", addressing_mode::immediate, 2u, 2u, true, false, false},
        {0xA3u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xA4u, "LDY", addressing_mode::zero_page, 2u, 3u, true, false, false},
        {0xA5u, "LDA", addressing_mode::zero_page, 2u, 3u, true, false, false},
        {0xA6u, "LDX", addressing_mode::zero_page, 2u, 3u, true, false, false},
        {0xA7u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xA8u, "TAY", addressing_mode::implied, 1u, 2u, true, false, false},
        {0xA9u, "LDA", addressing_mode::immediate, 2u, 2u, true, false, false},
        {0xAAu, "TAX", addressing_mode::implied, 1u, 2u, true, false, false},
        {0xABu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xACu, "LDY", addressing_mode::absolute, 3u, 4u, true, false, false},
        {0xADu, "LDA", addressing_mode::absolute, 3u, 4u, true, false, false},
        {0xAEu, "LDX", addressing_mode::absolute, 3u, 4u, true, false, false},
        {0xAFu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xB0u, "BCS", addressing_mode::relative, 2u, 2u, true, false, true},
        {0xB1u, "LDA", addressing_mode::indirect_indexed, 2u, 5u, true, true, false},
        {0xB2u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xB3u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xB4u, "LDY", addressing_mode::zero_page_x, 2u, 4u, true, false, false},
        {0xB5u, "LDA", addressing_mode::zero_page_x, 2u, 4u, true, false, false},
        {0xB6u, "LDX", addressing_mode::zero_page_y, 2u, 4u, true, false, false},
        {0xB7u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xB8u, "CLV", addressing_mode::implied, 1u, 2u, true, false, false},
        {0xB9u, "LDA", addressing_mode::absolute_y, 3u, 4u, true, true, false},
        {0xBAu, "TSX", addressing_mode::implied, 1u, 2u, true, false, false},
        {0xBBu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xBCu, "LDY", addressing_mode::absolute_x, 3u, 4u, true, true, false},
        {0xBDu, "LDA", addressing_mode::absolute_x, 3u, 4u, true, true, false},
        {0xBEu, "LDX", addressing_mode::absolute_y, 3u, 4u, true, true, false},
        {0xBFu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xC0u, "CPY", addressing_mode::immediate, 2u, 2u, true, false, false},
        {0xC1u, "CMP", addressing_mode::indexed_indirect, 2u, 6u, true, false, false},
        {0xC2u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xC3u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xC4u, "CPY", addressing_mode::zero_page, 2u, 3u, true, false, false},
        {0xC5u, "CMP", addressing_mode::zero_page, 2u, 3u, true, false, false},
        {0xC6u, "DEC", addressing_mode::zero_page, 2u, 5u, true, false, false},
        {0xC7u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xC8u, "INY", addressing_mode::implied, 1u, 2u, true, false, false},
        {0xC9u, "CMP", addressing_mode::immediate, 2u, 2u, true, false, false},
        {0xCAu, "DEX", addressing_mode::implied, 1u, 2u, true, false, false},
        {0xCBu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xCCu, "CPY", addressing_mode::absolute, 3u, 4u, true, false, false},
        {0xCDu, "CMP", addressing_mode::absolute, 3u, 4u, true, false, false},
        {0xCEu, "DEC", addressing_mode::absolute, 3u, 6u, true, false, false},
        {0xCFu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xD0u, "BNE", addressing_mode::relative, 2u, 2u, true, false, true},
        {0xD1u, "CMP", addressing_mode::indirect_indexed, 2u, 5u, true, true, false},
        {0xD2u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xD3u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xD4u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xD5u, "CMP", addressing_mode::zero_page_x, 2u, 4u, true, false, false},
        {0xD6u, "DEC", addressing_mode::zero_page_x, 2u, 6u, true, false, false},
        {0xD7u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xD8u, "CLD", addressing_mode::implied, 1u, 2u, true, false, false},
        {0xD9u, "CMP", addressing_mode::absolute_y, 3u, 4u, true, true, false},
        {0xDAu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xDBu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xDCu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xDDu, "CMP", addressing_mode::absolute_x, 3u, 4u, true, true, false},
        {0xDEu, "DEC", addressing_mode::absolute_x, 3u, 7u, true, false, false},
        {0xDFu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xE0u, "CPX", addressing_mode::immediate, 2u, 2u, true, false, false},
        {0xE1u, "SBC", addressing_mode::indexed_indirect, 2u, 6u, true, false, false},
        {0xE2u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xE3u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xE4u, "CPX", addressing_mode::zero_page, 2u, 3u, true, false, false},
        {0xE5u, "SBC", addressing_mode::zero_page, 2u, 3u, true, false, false},
        {0xE6u, "INC", addressing_mode::zero_page, 2u, 5u, true, false, false},
        {0xE7u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xE8u, "INX", addressing_mode::implied, 1u, 2u, true, false, false},
        {0xE9u, "SBC", addressing_mode::immediate, 2u, 2u, true, false, false},
        {0xEAu, "NOP", addressing_mode::implied, 1u, 2u, true, false, false},
        {0xEBu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xECu, "CPX", addressing_mode::absolute, 3u, 4u, true, false, false},
        {0xEDu, "SBC", addressing_mode::absolute, 3u, 4u, true, false, false},
        {0xEEu, "INC", addressing_mode::absolute, 3u, 6u, true, false, false},
        {0xEFu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xF0u, "BEQ", addressing_mode::relative, 2u, 2u, true, false, true},
        {0xF1u, "SBC", addressing_mode::indirect_indexed, 2u, 5u, true, true, false},
        {0xF2u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xF3u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xF4u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xF5u, "SBC", addressing_mode::zero_page_x, 2u, 4u, true, false, false},
        {0xF6u, "INC", addressing_mode::zero_page_x, 2u, 6u, true, false, false},
        {0xF7u, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xF8u, "SED", addressing_mode::implied, 1u, 2u, true, false, false},
        {0xF9u, "SBC", addressing_mode::absolute_y, 3u, 4u, true, true, false},
        {0xFAu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xFBu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xFCu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
        {0xFDu, "SBC", addressing_mode::absolute_x, 3u, 4u, true, true, false},
        {0xFEu, "INC", addressing_mode::absolute_x, 3u, 7u, true, false, false},
        {0xFFu, "???", addressing_mode::illegal, 1u, 0u, false, false, false},
    }};
}

inline constexpr auto nmos_opcode_metadata_table = make_nmos_opcode_metadata_table();

constexpr const opcode_metadata& nmos_opcode_metadata(const std::uint8_t opcode)
{
    return nmos_opcode_metadata_table[opcode];
}

constexpr bool is_legal_nmos_opcode(const std::uint8_t opcode)
{
    return nmos_opcode_metadata(opcode).legal_nmos;
}

constexpr int nmos_legal_opcode_count()
{
    int count = 0;
    for (const auto& opcode : nmos_opcode_metadata_table) {
        if (opcode.legal_nmos) {
            ++count;
        }
    }
    return count;
}

static_assert(nmos_legal_opcode_count() == 151, "Documented NMOS 6502 opcode count must stay 151");
static_assert(is_legal_nmos_opcode(0x00), "BRK must be legal on NMOS 6502");
static_assert(is_legal_nmos_opcode(0xEA), "NOP must be legal on NMOS 6502");
static_assert(!is_legal_nmos_opcode(0x02), "0x02 must remain illegal on documented NMOS 6502");
static_assert(nmos_opcode_metadata(0xA9).bytes == 2, "LDA immediate is two bytes");
static_assert(nmos_opcode_metadata(0x6C).cycles == 5, "JMP indirect is five cycles");

} // namespace benchmark6502
