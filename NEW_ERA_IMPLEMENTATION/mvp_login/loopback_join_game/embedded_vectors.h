// NEW-ERA — loopback_join_game — embedded_vectors.h (1.3-C, ASIO TX + C1 F3:03 RX)
// FONTE: NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_join_game/join_game_vectors.json
// SHA256 do json: c912dbb1c8fc8710d3575a4015640a4d3b2d9a664e1411d1bb8c7b7ce9ad0451
// Gerado programaticamente (sem digitacao). REQ = frame olc::net (id 0x000B,
// SEM C1/XOR); RESP = C1 PLAIN 66 B (server envia; check crypto comentado :873-:880).
#pragma once
#include <array>
#include <stdint.h>
#include <stddef.h>
namespace newera { namespace loopback_join {
constexpr size_t kReqSize  = 16;   // request C->S olc::net [id:u16][size:u32][ID10]
constexpr size_t kRespSize = 66;   // response S->C C1 PLAIN (F3:03 JoinMapServer)
static const char* kExpectedId        = "Hero1";
constexpr int      kExpectedMap       = 3;
constexpr int      kExpectedX         = 100;
constexpr int      kExpectedY         = 200;
constexpr int      kExpectedAngleDeg  = 45;
constexpr uint32_t kExpectedGold      = 123456;
constexpr uint64_t kExpectedExp       = 72623859790382856;

constexpr std::array<uint8_t, 16> kReqExpected = {
    0x0B, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x48, 0x65, 0x72, 0x6F, 0x31, 0x00,
    0x00, 0x00, 0x00, 0x00
};

constexpr std::array<uint8_t, 66> kRespServer = {
    0xC1, 0x42, 0xF3, 0x03, 0x64, 0xC8, 0x03, 0x02, 0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x07, 0x08, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x0A, 0x00, 0x19, 0x00, 0x14, 0x00, 0x16, 0x00, 0x0F, 0x00, 0x64, 0x00,
    0x6E, 0x00, 0x14, 0x00, 0x3C, 0x00, 0x05, 0x00, 0x0A, 0x00, 0x07, 0x00,
    0x0E, 0x00, 0x40, 0xE2, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x20, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

} } // namespace
