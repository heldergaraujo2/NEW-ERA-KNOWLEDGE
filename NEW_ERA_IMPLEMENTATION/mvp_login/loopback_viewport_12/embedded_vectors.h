// NEW-ERA — loopback_viewport_12 — embedded_vectors.h (1.3-G, C2 0x12 CHARACTER)
// FONTE: NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_12/viewport_12_vectors.json
// SHA256 do json: 17d8b1af0b0ea571dbb30bcd3c590471ea58257731f3379082e35b190bde98ea
// Gerado programaticamente (sem digitacao). RESP = C2 79 B (2 entidades,
// strides 36 e 38 :2376; Path dual dir/PK :2237/:2271; equip opaco).
// NOTA: TotalSize do comando (115) divergia da soma dos componentes
// (4+1+36+38=79) — usado 79 (evidência prevalece; anti-invenção).
#pragma once
#include <array>
#include <stdint.h>
#include <stddef.h>
namespace newera { namespace loopback_vp12 {
constexpr size_t kRespSize = 79;   // [C2][szH][szL][0x12][count=2]+A(36B)+B(38B)
constexpr int      kExpectedCount = 2;
constexpr uint16_t kExpectedKeyA  = 256;   // 0x0100
constexpr uint16_t kExpectedKeyB  = 512;   // 0x0200
static const char* kExpectedIdA   = "HeroA";
static const char* kExpectedIdB   = "HeroB";
constexpr int      kExpectedDirA  = 1;   // 1 => 0 deg
constexpr int      kExpectedDirB  = 2;   // 2 => 45 deg
constexpr int      kExpectedPkA   = 3;    // 3 (low nibble :2237)
constexpr int      kExpectedPkB   = 0;    // 0
constexpr int      kExpectedAngA  = 0;
constexpr int      kExpectedAngB  = 45;

constexpr std::array<uint8_t, 79> kRespServer = {
    0xC2, 0x00, 0x4F, 0x12, 0x02, 0x01, 0x00, 0x0A, 0x14, 0x03, 0x00, 0x01,
    0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
    0x0E, 0x0F, 0x10, 0x48, 0x65, 0x72, 0x6F, 0x41, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0B, 0x15, 0x13, 0x00, 0x02, 0x00, 0x1E, 0x28, 0x01, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0x48, 0x65, 0x72, 0x6F, 0x42, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x1F, 0x29, 0x20, 0x02, 0x55, 0x66
};

constexpr std::array<uint8_t, 2> kExpectedBuffsB = {
    0x55, 0x66
};

} } // namespace
