// NEW-ERA — loopback_f3_52 — embedded_vectors.h (1.1-F)
// FONTE: NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_52/f3_52_vectors.json
// SHA256 do json: a50223b79b396d7831133e7c297877f695beb55689bc31bfaa41554808085e92
// Gerado programaticamente (sem digitacao) a partir do golden 1.1-F.
// ATENCAO: REQ e WIRE REAL C1 PLAIN de 8 B (nao C3).
#pragma once
#include <array>
#include <stdint.h>
#include <stddef.h>
namespace newera { namespace loopback_f3m {
constexpr size_t kReqSize  = 8;   // request C->S C1 PLAIN (wire real; esperado no server)
constexpr size_t kRespSize = 24;   // response S->C C3 (server stub envia)
constexpr int      kExpectedResult    = 1;
constexpr int      kExpectedMlPoint   = 5;
constexpr int32_t  kExpectedSkillNum  = 123456;
constexpr int32_t  kExpectedSkillLevel= 3;

constexpr std::array<uint8_t, 8> kReqExpected = {
    0xC1, 0x08, 0xF3, 0x28, 0xD4, 0x84, 0x1A, 0x69
};

constexpr std::array<uint8_t, 24> kRespServer = {
    0xC3, 0x18, 0xB4, 0x14, 0x0E, 0x36, 0xD9, 0xFF, 0xA1, 0x4C, 0xEC, 0xCB,
    0xFE, 0x0C, 0x75, 0x3E, 0xB5, 0x52, 0x97, 0x96, 0x3E, 0xFD, 0xC1, 0xFA
};

} } // namespace
