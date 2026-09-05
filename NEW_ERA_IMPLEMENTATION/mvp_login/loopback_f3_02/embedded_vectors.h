// NEW-ERA — loopback_f3_02 — embedded_vectors.h (1.1-D)
// FONTE: NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_02/f3_02_vectors.json
// SHA256 do json: acdaffccb33acb14a2667494ecf80058b994b3d08e0fb48b742aade7fe4521c2
// Gerado programaticamente (sem digitacao) a partir do golden 1.1-D.
#pragma once
#include <array>
#include <stdint.h>
#include <stddef.h>
namespace newera { namespace loopback_f3d {
constexpr size_t kReqSize  = 35;   // request C->S C3 (esperado no server)
constexpr size_t kRespSize = 13;   // response S->C C3 (server stub envia)
constexpr int      kExpectedResult = 1;   // 1 = SUCCESS (:677)
static const char* kExpectedId      = "Hero1";

constexpr std::array<uint8_t, 35> kReqExpected = {
    0xC3, 0x23, 0x1C, 0xB6, 0x38, 0x5E, 0x17, 0xCE, 0x83, 0xD5, 0x6D, 0x34,
    0x01, 0x44, 0x02, 0x1E, 0x53, 0xCA, 0xB3, 0x74, 0x14, 0x10, 0xCB, 0xFE,
    0x04, 0x31, 0x63, 0x85, 0x86, 0x30, 0x30, 0x15, 0xD0, 0x97, 0xA2
};

constexpr std::array<uint8_t, 13> kRespServer = {
    0xC3, 0x0D, 0x0A, 0x57, 0x2A, 0x7C, 0xD7, 0xDA, 0xC2, 0x78, 0x18, 0x30,
    0x09
};

} } // namespace
