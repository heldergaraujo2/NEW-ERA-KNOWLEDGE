#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace newera::login_rx {

struct ServerVersion {
    uint8_t result = 0;
    uint16_t hero_key = 0;
    uint8_t version[5]{};
};

inline bool BuildServerVersionC1(
    uint8_t result,
    uint16_t hero_key,
    const uint8_t version[5],
    std::vector<uint8_t>& out) {
    out.resize(12);
    out[0] = 0xC1;
    out[1] = 0x0C;
    out[2] = 0xF1;
    out[3] = 0x00;
    out[4] = result;
    out[5] = static_cast<uint8_t>(hero_key >> 8);
    out[6] = static_cast<uint8_t>(hero_key & 0xFF);
    for (std::size_t i = 0; i < 5; ++i) {
        out[7 + i] = version[i];
    }
    return true;
}

inline bool ParseServerVersionC1(
    const uint8_t* packet,
    std::size_t size,
    ServerVersion& out,
    std::string& error) {
    if (size != 12) {
        error = "F1:00: expected 12-byte C1";
        return false;
    }
    if (packet[0] != 0xC1 || packet[1] != 0x0C ||
        packet[2] != 0xF1 || packet[3] != 0x00) {
        error = "F1:00: invalid C1 header";
        return false;
    }
    out.result = packet[4];
    out.hero_key = static_cast<uint16_t>(
        (static_cast<uint16_t>(packet[5]) << 8) | packet[6]);
    for (std::size_t i = 0; i < 5; ++i) {
        out.version[i] = packet[7 + i];
    }
    return true;
}

} // namespace newera::login_rx
