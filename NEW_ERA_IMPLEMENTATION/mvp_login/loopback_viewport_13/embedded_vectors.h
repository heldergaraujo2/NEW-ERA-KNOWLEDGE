// NEW-ERA — loopback_viewport_13 — embedded_vectors.h (1.3-E, C2 head 0x13 SPAWN)
// FONTE: NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_13/viewport_13_vectors.json
// SHA256 do json: 8ab230289442a6fbc2c5878fd445d4f1035c11531b066137ce2a467791995c97
// Gerado programaticamente (sem digitacao). RESP = C2 15 B (1 entidade, 0 buffs;
// S->C — pacote de spawn, sem request). NOTA: "ViewPortNumber" do exemplo do
// comando NAO EXISTE em PCREATE_MONSTER (:588-:600) — nao inventado.
#pragma once
#include <array>
#include <stdint.h>
#include <stddef.h>
namespace newera { namespace loopback_vp13 {
constexpr size_t kRespSize = 15;   // C2 [C2][szH][szL][0x13][count]+entidade 10 B
constexpr int      kExpectedCount = 1;
constexpr uint16_t kExpectedKey   = 291;   // 0x0123 (flags 0)
constexpr uint16_t kExpectedType  = 682;  // 0x02AA (10-bit)
constexpr int      kExpectedX     = 100;
constexpr int      kExpectedY     = 200;
constexpr int      kExpectedDir   = 2;
constexpr int      kExpectedAngle = 45;

constexpr std::array<uint8_t, 15> kRespServer = {
    0xC2, 0x00, 0x0F, 0x13, 0x01, 0x01, 0x23, 0x02, 0xAA, 0x64, 0xC8, 0x65,
    0xC9, 0x20, 0x00
};

} } // namespace
