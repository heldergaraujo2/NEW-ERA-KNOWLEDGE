#pragma once

#include <cstdint>
#include <string>

namespace newera::combat_rx {

// Upstream evidence: WSclient.h defines PRECEIVE_ACTION as
// PBMSG_HEADER + KeyH/KeyL + Angle + Action + TargetKeyH/TargetKeyL.
// Therefore the native C1 wire frame is exactly 9 bytes.
struct ActionEvent {
    uint16_t key = 0;
    uint8_t angle = 0;
    uint8_t action = 0;
    uint16_t target = 0;
};

// Upstream evidence: WSclient.h defines PRECEIVE_MAGIC as
// PBMSG_HEADER + MagicH/MagicL + SourceKeyH/SourceKeyL + TargetKeyH/TargetKeyL.
// Therefore the native C1 wire frame is exactly 9 bytes.
struct SkillEvent {
    uint16_t magic = 0;
    uint16_t source = 0;
    uint16_t target = 0;
};

inline bool ParseActionC1(
    const uint8_t* packet,
    std::size_t size,
    ActionEvent& out,
    std::string& error) {
    if (size != 9) {
        error = "0x18 action: expected 9-byte C1";
        return false;
    }
    if (packet[0] != 0xC1 || packet[1] != 9 || packet[2] != 0x18) {
        error = "0x18 action: invalid C1 header";
        return false;
    }

    out.key = static_cast<uint16_t>(
        (static_cast<uint16_t>(packet[3]) << 8) | packet[4]);
    out.angle = packet[5];
    out.action = packet[6];
    out.target = static_cast<uint16_t>(
        (static_cast<uint16_t>(packet[7]) << 8) | packet[8]);
    return true;
}

inline bool ParseSkillC1(
    const uint8_t* packet,
    std::size_t size,
    SkillEvent& out,
    std::string& error) {
    if (size != 9) {
        error = "0x19 magic: expected 9-byte C1";
        return false;
    }
    if (packet[0] != 0xC1 || packet[1] != 9 || packet[2] != 0x19) {
        error = "0x19 magic: invalid C1 header";
        return false;
    }

    out.magic = static_cast<uint16_t>(
        (static_cast<uint16_t>(packet[3]) << 8) | packet[4]);
    out.source = static_cast<uint16_t>(
        (static_cast<uint16_t>(packet[5]) << 8) | packet[6]);
    out.target = static_cast<uint16_t>(
        (static_cast<uint16_t>(packet[7]) << 8) | packet[8]);
    return true;
}

} // namespace newera::combat_rx
