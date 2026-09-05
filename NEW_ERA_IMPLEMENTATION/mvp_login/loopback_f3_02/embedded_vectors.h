// NEW-ERA — loopback_f3_02 — embedded_vectors.h (1.2-A2, WIRE-REAL C1)
// FONTE: NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_02/f3_02_vectors.json
// SHA256 do json: 3158be0b5eea56c01738b7f95f4cd056058c2da238ef1e2641f52655ad15fed1
// Gerado programaticamente (sem digitacao). ATENCAO: REQ e RESP sao C1 PLAIN.
#pragma once
#include <array>
#include <stdint.h>
#include <stddef.h>
namespace newera { namespace loopback_f3d {
constexpr size_t kReqSize  = 34;   // request C->S C1 PLAIN (wire real, Resident[20])
constexpr size_t kRespSize = 5;   // response S->C C1 PLAIN (server envia)
constexpr int      kExpectedResult = 1;   // 1 = SUCCESS (:677)
static const char* kExpectedId      = "Hero1";
static const char* kExpectedResident= "12345678901234567890";

constexpr std::array<uint8_t, 34> kReqExpected = {
    0xC1, 0x22, 0xF3, 0x78, 0x8C, 0x5B, 0xB6, 0xAA, 0xB8, 0x10, 0xEE, 0x58,
    0x11, 0x4C, 0x44, 0x2B, 0x92, 0x6D, 0x3B, 0x80, 0x5D, 0x18, 0x0A, 0x65,
    0x97, 0x14, 0xCE, 0x79, 0x65, 0x02, 0xDD, 0xB3, 0x6D, 0x30
};

constexpr std::array<uint8_t, 5> kRespServer = {
    0xC1, 0x05, 0xF3, 0x02, 0x01
};

} } // namespace
