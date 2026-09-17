#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace newera::both_attack {

inline constexpr uint16_t kBothAttack1 = 0x0008;
inline constexpr uint16_t kBothAttack2 = 0x0009;

inline void AppendU16LE(std::vector<uint8_t>& out, uint16_t v) {
    out.push_back(static_cast<uint8_t>(v & 0xFF));
    out.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
}

inline void AppendU32LE(std::vector<uint8_t>& out, uint32_t v) {
    out.push_back(static_cast<uint8_t>(v & 0xFF));
    out.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
    out.push_back(static_cast<uint8_t>((v >> 16) & 0xFF));
    out.push_back(static_cast<uint8_t>((v >> 24) & 0xFF));
}

inline bool BuildAttack1(uint16_t index, uint8_t action, uint8_t dir,
                         std::vector<uint8_t>& out, std::string& error) {
    out.clear();
    if (index == 0) { error = "index=0"; return false; }
    if (dir > 7) { error = "dir>7"; return false; }
    AppendU16LE(out, kBothAttack1);
    AppendU32LE(out, 7);
    out.insert(out.end(), {0xC1, 0x07, 0x11,
                           static_cast<uint8_t>(index >> 8),
                           static_cast<uint8_t>(index & 0xFF), action, dir});
    return true;
}

inline bool BuildAttack2(uint16_t skill, uint8_t x, uint8_t y, uint8_t serial,
                         uint8_t count, std::vector<uint8_t>& out,
                         std::string& error) {
    out.clear();
    if (count < 1) { error = "count<1"; return false; }
    if (serial < 1 || serial > 50) { error = "serial out of range (1..50)"; return false; }
    AppendU16LE(out, kBothAttack2);
    AppendU32LE(out, 9);
    out.insert(out.end(), {0xC3, 0x09, 0xDB,
                           static_cast<uint8_t>(skill >> 8), count,
                           static_cast<uint8_t>(skill & 0xFF), x, serial, y});
    return true;
}

} // namespace newera::both_attack
