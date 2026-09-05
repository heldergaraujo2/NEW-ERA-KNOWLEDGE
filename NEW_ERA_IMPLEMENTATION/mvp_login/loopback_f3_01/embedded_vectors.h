// NEW-ERA — loopback_f3_01 — embedded_vectors.h (1.3-B, WIRE-REAL C1)
// FONTE: NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_01/f3_01_vectors.json
// SHA256 do json: a68184f38a2f24c803223c484aabdf54bec24a25b85c3ec18f6f5ff7de86e38a
// Gerado programaticamente (sem digitacao). ATENCAO: REQ e RESP sao C1 PLAIN.
#pragma once
#include <array>
#include <stdint.h>
#include <stddef.h>
namespace newera { namespace loopback_f3c {
constexpr size_t kReqSize  = 15;   // request C->S C1 PLAIN (wire real)
constexpr size_t kRespSize = 19;   // response S->C C1 PLAIN (server envia)
constexpr int      kExpectedResult = 1;      // 1 = SUCCESS (:625)
constexpr int      kExpectedIndex  = 0;
constexpr uint16_t kExpectedLevel  = 1;
static const char* kExpectedId     = "HeroNew";
constexpr uint8_t  kExpectedClass  = 3;

constexpr std::array<uint8_t, 15> kReqExpected = {
    0xC1, 0x0F, 0xF3, 0x7B, 0x8F, 0x58, 0xB5, 0xA9, 0xC4, 0x09, 0x80, 0x36,
    0x7F, 0x22, 0x29
};

constexpr std::array<uint8_t, 19> kRespServer = {
    0xC1, 0x13, 0xF3, 0x01, 0x01, 0x48, 0x65, 0x72, 0x6F, 0x4E, 0x65, 0x77,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03
};

} } // namespace
