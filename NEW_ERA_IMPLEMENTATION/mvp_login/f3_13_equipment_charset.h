#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace newera::f3_13 {

inline constexpr std::size_t kFrameSize = 24;
inline constexpr std::size_t kCharSetSize = 18;

// Normative F3:13 response reconstructed from the upstream evidence.
// Wire layout (C1, 24 bytes):
//   [0]    0xC1
//   [1]    0x18 (24)
//   [2]    0xF3
//   [3]    0x13
//   [4..5] character index, big-endian (NumberH/NumberL)
//   [6..23] CharSet[18]
struct EquipmentCharSet {
    uint16_t index = 0;
    std::array<uint8_t, kCharSetSize> charset{};
};

inline std::vector<uint8_t> Build(
    uint16_t index,
    const std::array<uint8_t, kCharSetSize>& charset) {
    std::vector<uint8_t> p(kFrameSize, 0);
    p[0] = 0xC1;
    p[1] = static_cast<uint8_t>(kFrameSize);
    p[2] = 0xF3;
    p[3] = 0x13;
    p[4] = static_cast<uint8_t>(index >> 8);
    p[5] = static_cast<uint8_t>(index & 0xFF);
    for (std::size_t i = 0; i < kCharSetSize; ++i) {
        p[6 + i] = charset[i];
    }
    return p;
}

inline bool Parse(
    const std::vector<uint8_t>& packet,
    EquipmentCharSet& out,
    std::string& err) {
    if (packet.size() != kFrameSize) {
        err = "F3:13: expected exactly 24 bytes";
        return false;
    }
    if (packet[0] != 0xC1 || packet[1] != kFrameSize ||
        packet[2] != 0xF3 || packet[3] != 0x13) {
        err = "F3:13: invalid C1 header/subcode";
        return false;
    }
    out.index = static_cast<uint16_t>(
        (static_cast<uint16_t>(packet[4]) << 8) | packet[5]);
    for (std::size_t i = 0; i < kCharSetSize; ++i) {
        out.charset[i] = packet[6 + i];
    }
    return true;
}

} // namespace newera::f3_13
