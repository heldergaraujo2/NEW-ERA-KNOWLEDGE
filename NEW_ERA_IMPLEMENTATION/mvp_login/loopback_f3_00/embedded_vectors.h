// NEW-ERA — loopback_f3_00 — embedded_vectors.h (1.2-A1, WIRE-REAL C1)
// FONTE: NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_00/f3_00_vectors.json
// SHA256 do json: fafe052fd26dcb57215e0ad6f85b4b3506e605e611f4ff273bf0ceb2c766d250
// Gerado programaticamente (sem digitacao). ATENCAO: REQ e RESP sao C1 PLAIN.
#pragma once
#include <array>
#include <stdint.h>
#include <stddef.h>
namespace newera { namespace loopback_f3 {
constexpr size_t kReqSize  = 5;   // request C->S C1 PLAIN (wire real)
constexpr size_t kRespSize = 40;   // response S->C C1 PLAIN (server envia)
constexpr int      kExpectedCount    = 1;
constexpr int      kExpectedMaxClass = 3;
constexpr uint16_t kExpectedLevel    = 345;
static const char* kExpectedName     = "Hero1";

constexpr std::array<uint8_t, 5> kReqExpected = {
    0xC1, 0x05, 0xF3, 0x7A, 0xC7
};

constexpr std::array<uint8_t, 40> kRespServer = {
    0xC1, 0x28, 0xF3, 0x00, 0x03, 0x00, 0x01, 0x00, 0x48, 0x65, 0x72, 0x6F,
    0x31, 0x00, 0x00, 0x00, 0x00, 0x00, 0x59, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

} } // namespace
