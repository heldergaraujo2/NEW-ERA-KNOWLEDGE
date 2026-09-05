// NEW-ERA — loopback_attack_tx_11 — embedded_vectors.h (1.3-N: TX normal attack C1 0x11 + RESP damage)
// FONTE: NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_attack_tx_11/attack_tx_11_vectors.json
// SHA256 do json: 4b8c5bfda64299160e899d73ac11f143036286269e6de7e3c4ef15cc35b9c1be
// Gerado programaticamente (sem digitacao). Troca RAW (2 mensagens, sem framing):
// CLIENT envia kReq (C1 7 B); SERVER valida memcmp e responde kRespDamage (C1 10 B).
#pragma once
#include <array>
#include <cstdint>
#include <stddef.h>
namespace newera { namespace loopback_atx {
constexpr size_t kReqLen  = 7;
constexpr size_t kRespLen = 10;
constexpr uint16_t kTargetKey  = 257;   // 0x0101
constexpr uint8_t  kDir        = 3;
constexpr uint16_t kDummyKey   = 256;   // 0x0100 (raw 0x8100 &0x7FFF)
constexpr int      kLastDamage = 291;  // 0x0123

constexpr std::array<uint8_t, 7> kReq = {
    0xC1, 0x07, 0x11, 0x01, 0x01, 0x78, 0x03
};

constexpr std::array<uint8_t, 10> kRespDamage = {
    0xC1, 0x0A, 0x11, 0x81, 0x00, 0x01, 0x23, 0xC5, 0x00, 0x44
};

} } // namespace
