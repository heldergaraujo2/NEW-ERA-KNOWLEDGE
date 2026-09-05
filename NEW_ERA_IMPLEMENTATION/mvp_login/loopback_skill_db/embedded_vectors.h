// NEW-ERA — loopback_skill_db — embedded_vectors.h (1.3-O: TX skill C1 0xDB + RESP damage)
// FONTE: NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_skill_db/skill_db_vectors.json
// SHA256 do json: 8626d77e56dcb64f8430f3eb74a6227602ef11bf9856bd85e7378a37517f72e0
// Gerado programaticamente (sem digitacao). Troca RAW (2 mensagens, sem framing):
// CLIENT envia kReq (C1 12 B skill); SERVER valida memcmp e responde kRespDamage (C1 10 B).
#pragma once
#include <array>
#include <cstdint>
#include <stddef.h>
namespace newera { namespace loopback_skill {
constexpr size_t kReqLen  = 12;
constexpr size_t kRespLen = 10;
constexpr uint16_t kSkillType  = 9;
constexpr uint8_t  kX          = 50;
constexpr uint8_t  kY          = 60;
constexpr uint8_t  kSerial     = 1;
constexpr uint8_t  kCount      = 1;
constexpr uint16_t kTargetKey  = 257;   // 0x0101
constexpr uint8_t  kSkillSerial= 2;
constexpr uint16_t kDummyKey   = 256;   // 0x0100 (raw 0x8100 &0x7FFF)
constexpr int      kLastDamage = 291;  // 0x0123

constexpr std::array<uint8_t, 12> kReq = {
    0xC1, 0x0C, 0xDB, 0x00, 0x09, 0x32, 0x3C, 0x01, 0x01, 0x01, 0x01, 0x02
};

constexpr std::array<uint8_t, 10> kRespDamage = {
    0xC1, 0x0A, 0x11, 0x81, 0x00, 0x01, 0x23, 0xC5, 0x00, 0x44
};

} } // namespace
