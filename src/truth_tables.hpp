#pragma once

#include <Arduino.h>
#include <unordered_map>

enum class eGateType {
    AND = 0,
    OR,
    XOR,
    NAND,
    NOR,
    XNOR
};

String GateTypeToString(eGateType type) {
    switch(type) {
        case eGateType::AND: return "AND";
        case eGateType::OR: return "OR";
        case eGateType::XOR: return "XOR";
        case eGateType::NAND: return "NAND";
        case eGateType::NOR: return "NOR";
        case eGateType::XNOR: return "XNOR";
        default: return "UNK";
    }
}

// key is - 6 bits are logic type, 2 remaining are logic inputs
static std::unordered_map<uint8_t, bool> truthLUT = {
    // AND
    { 0b00000000, 0 },
    { 0b00000001, 0 },
    { 0b00000010, 0 },
    { 0b00000011, 1 },

    // OR
    { 0b00000100, 0 },
    { 0b00000101, 1 },
    { 0b00000110, 1 },
    { 0b00000111, 1 },

    // XOR
    { 0b00001000, 0 },
    { 0b00001001, 1 },
    { 0b00001010, 1 },
    { 0b00001011, 0 },

    // NAND
    { 0b00001100, 1 },
    { 0b00001101, 1 },
    { 0b00001110, 1 },
    { 0b00001111, 0 },

    // NOR
    { 0b00010000, 1 },
    { 0b00010001, 0 },
    { 0b00010010, 0 },
    { 0b00010011, 0 },

    // XNOR
    { 0b00010100, 1 },
    { 0b00010101, 0 },
    { 0b00010110, 0 },
    { 0b00010111, 1 },
};
