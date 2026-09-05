// NEW-ERA — loopback_f3_06 — embedded_vectors.h (1.3-A, WIRE-REAL C1)
// FONTE: NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_06/f3_06_vectors.json
// SHA256 do json: bdababe0373ffa362b51e5037ca10baef5d12f3d82694a1f2c92f363b0089e36
// Gerado programaticamente (sem digitacao). ATENCAO: REQ e RESP sao C1 PLAIN.
#pragma once
#include <array>
#include <stdint.h>
#include <stddef.h>
namespace newera { namespace loopback_f3a {
constexpr size_t kReqSize  = 5;   // request C->S C1 PLAIN (wire real)
constexpr size_t kRespSize = 11;   // response S->C C1 PLAIN (server envia)
constexpr int      kExpectedStat      = 2;       // 2 = VIT (:6214)
constexpr uint16_t kExpectedMax       = 1000;        // LifeMax p/ VIT (:6216)
constexpr uint16_t kExpectedShield    = 3000;
constexpr uint16_t kExpectedSkillMana = 4000;

constexpr std::array<uint8_t, 5> kReqExpected = {
    0xC1, 0x05, 0xF3, 0x7C, 0xC2
};

constexpr std::array<uint8_t, 11> kRespServer = {
    0xC1, 0x0B, 0xF3, 0x06, 0x12, 0xE8, 0x03, 0xB8, 0x0B, 0xA0, 0x0F
};

} } // namespace
