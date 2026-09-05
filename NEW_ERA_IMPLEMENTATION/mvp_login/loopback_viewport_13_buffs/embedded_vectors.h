// NEW-ERA — loopback_viewport_13_buffs — embedded_vectors.h (1.3-F, C2 0x13 + BUFFS)
// FONTE: NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_13_buffs/viewport_13_buffs_vectors.json
// SHA256 do json: 8cc2bcf7d008f830393b0250a2691062449e9014a314613896dc8222b027c034
// Gerado programaticamente (sem digitacao). RESP = C2 29 B (2 entidades,
// strides 11 e 13 :2710; buffs A=[AA], B=[01 02 03]). S->C — sem request.
#pragma once
#include <array>
#include <stdint.h>
#include <stddef.h>
namespace newera { namespace loopback_vp13b {
constexpr size_t kRespSize = 29;   // [C2][szH][szL][0x13][count=2]+A(11B)+B(13B)
constexpr int      kExpectedCount = 2;
constexpr uint16_t kExpectedKeyA  = 257;   // 0x0101
constexpr uint16_t kExpectedKeyB  = 514;   // 0x0202
constexpr int      kExpectedDirA  = 1;   // 1 => 0 deg
constexpr int      kExpectedDirB  = 2;   // 2 => 45 deg
constexpr int      kExpectedAngA  = 0;
constexpr int      kExpectedAngB  = 45;

constexpr std::array<uint8_t, 29> kRespServer = {
    0xC2, 0x00, 0x1D, 0x13, 0x02, 0x01, 0x01, 0x00, 0x11, 0x0A, 0x14, 0x0B,
    0x15, 0x10, 0x01, 0xAA, 0x02, 0x02, 0x00, 0x22, 0x1E, 0x28, 0x1F, 0x29,
    0x20, 0x03, 0x01, 0x02, 0x03
};

constexpr std::array<uint8_t, 1> kExpectedBuffsA = {
    0xAA
};

constexpr std::array<uint8_t, 3> kExpectedBuffsB = {
    0x01, 0x02, 0x03
};

} } // namespace
