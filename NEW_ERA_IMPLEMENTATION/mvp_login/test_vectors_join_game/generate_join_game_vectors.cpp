// NEW-ERA — 1.3-C — generate_join_game_vectors.cpp (ASIO TX + C1 F3:03 RX)
// Golden determinístico do Join Game no formato REAL do wire:
//   REQ  = frame olc::net 16 B [id:u16 LE=0x000B][size:u32 LE=10][ID10 "Hero1"]
//          (SendRequestJoinMapServer wsclientinline :322-:330 ->
//           gProtocolSend.SendPacket(BOTH_CONNECT_JOIN_GAME, nameBuf, 10);
//           ProtocolSend.h :145; framing ProtocolAsio.h :33-:37/:378-:390).
//          SEM C1/XOR — sistema novo.
//   RESP = C1 PLAIN 66 B (0x42) [C1][42][F3][03] + PRECEIVE_JOIN_MAP_SERVER
//          pack(1) 65 B (WSclient.h :405-:452). SEM crypto/keys
//          (check COMENTADO WSclient.cpp :873-:880).
#include "mvp_login_client.cpp"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

using namespace newera;

static std::string Hex(const std::vector<uint8_t>& v) {
    static const char* d = "0123456789abcdef";
    std::string s;
    for (uint8_t b : v) { s += d[b >> 4]; s += d[b & 0xF]; }
    return s;
}

int main() {
    const std::string kId = "Hero1";

    // ---------- REQ C->S: frame olc::net 16 B (id/size/body) ----------
    auto req = mvp::BuildAsio_BOTH_CONNECT_JOIN_GAME_Request(kId);
    assert(req.size() == 16);                                  // 2+4+10
    assert(req[0] == 0x0B && req[1] == 0x00);                  // id=0x000B LE
    assert(req[2] == 0x0A && req[3] == 0 && req[4] == 0 && req[5] == 0);  // size=10 LE
    assert(std::memcmp(&req[6], kId.data(), 5) == 0);          // "Hero1"
    for (int i = 11; i < 16; ++i) assert(req[i] == 0);         // zero-pad
    assert(mvp::BuildAsio_BOTH_CONNECT_JOIN_GAME_Request("ABCDEFGHIJK").empty());  // >10

    // ---------- RESP S->C: C1 plain 66 B (PRECEIVE_JOIN_MAP_SERVER) ----------
    std::vector<uint8_t> resp(66, 0);
    resp[0] = 0xC1; resp[1] = 0x42; resp[2] = 0xF3; resp[3] = 0x03;
    resp[4] = 100;                                             // PositionX
    resp[5] = 200;                                             // PositionY
    resp[6] = 3;                                               // Map
    resp[7] = 2;                                               // Angle byte => 45° (:980)
    for (int i = 0; i < 8; ++i) resp[8 + i]  = (uint8_t)(i + 1);      // Exp 01..08 BE
    for (int i = 0; i < 8; ++i) resp[16 + i] = (uint8_t)(0x11 + i);   // NextExp 11..18 BE
    const uint16_t kLvlUp = 10;
    std::memcpy(&resp[24], &kLvlUp, 2);
    const uint16_t st[12] = { 25, 20, 22, 15, 100, 110, 20, 60, 5, 10, 7, 14 };
    std::memcpy(&resp[26], st, sizeof st);                     // Str..SkillManaMax (LE)
    const uint32_t kGold = 123456;
    std::memcpy(&resp[50], &kGold, 4);                         // Gold LE :964
    resp[54] = 0;                                              // PK
    resp[55] = 0;                                              // CtlCode
    const int16_t kAdd = 3, kMaxAdd = 32;
    std::memcpy(&resp[56], &kAdd, 2);
    std::memcpy(&resp[58], &kMaxAdd, 2);
    // 60..65 = 0 (Charisma / wMinusPoint / wMaxMinusPoint)

    // ---------- Parse + asserts ----------
    mvp::ParsedJoinMapServer jm; std::string perr;
    assert(mvp::ParseC1_F3_03_JoinMapServerResponsePlain(resp, jm, perr));
    assert(jm.map == 3 && jm.x == 100 && jm.y == 200);         // campos criticos
    assert(jm.angleByte == 2 && jm.angleDeg == 45);            // (2-1)*45 :980
    assert(jm.gold == 123456);
    assert(jm.exp == 0x0102030405060708ULL);                   // BE 8xB :886-:902
    assert(jm.nextExp == 0x1112131415161718ULL);
    assert(jm.levelUpPoint == 10);
    assert(jm.stat[0] == 25 && jm.stat[4] == 100 && jm.stat[5] == 110 &&
           jm.stat[11] == 14);                                 // ordem do struct
    assert(jm.pk == 0 && jm.ctlCode == 0);
    assert(jm.addPoint == 3 && jm.maxAddPoint == 32);
    assert(jm.charisma == 0 && jm.minusPoint == 0 && jm.maxMinusPoint == 0);

    // Negativos: erro claro (tamanho/head)
    std::vector<uint8_t> bad = resp; bad.resize(65);
    assert(!mvp::ParseC1_F3_03_JoinMapServerResponsePlain(bad, jm, perr) &&
           perr.rfind("F3:03:", 0) == 0);
    bad = resp; bad[2] = 0xF4;
    assert(!mvp::ParseC1_F3_03_JoinMapServerResponsePlain(bad, jm, perr));

    // ---------- Saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_join_game/join_game_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_join_game/join_game_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"join_game_asio_tx_c1_f3_03_rx\",\n";
      j << "  \"req_asio_hex\": \"" << Hex(req) << "\",\n";
      j << "  \"resp_c1_hex\": \"" << Hex(resp) << "\",\n";
      j << "  \"expected_parse\": { \"id\": \"" << kId << "\", \"map\": " << (int)jm.map
        << ", \"x\": " << (int)jm.x << ", \"y\": " << (int)jm.y
        << ", \"angleByte\": " << (int)jm.angleByte << ", \"angleDeg\": " << jm.angleDeg
        << ", \"gold\": " << jm.gold << ", \"exp\": " << jm.exp
        << ", \"levelUpPoint\": " << jm.levelUpPoint
        << ", \"strength\": " << jm.stat[0] << ", \"lifeMax\": " << jm.stat[5] << " }\n}\n"; }
    { std::ofstream m(mpath);
      m << "# Join Game — golden vectors (1.3-C, ASIO TX + C1 F3:03 RX)\n\n";
      m << "- REQ C->S **olc::net frame** (16 B, id=0x000B/size=10 LE): `" << Hex(req) << "`\n";
      m << "- RESP S->C **C1 plain** (66 B / 0x42): `" << Hex(resp) << "`\n";
      m << "- expected: id=" << kId << ", map=3, x=100, y=200, angleByte=2 (45°), gold=123456,\n";
      m << "  exp=0x0102030405060708 (BE), levelUpPoint=10, Str=25, LifeMax=110\n";
      m << "- Wire: SendRequestJoinMapServer :322-:330 (gProtocolSend/BOTH_CONNECT_JOIN_GAME;\n";
      m << "  sem C1/XOR) · RX PRECEIVE_JOIN_MAP_SERVER :405-:452 (cast :883; W/DW LE).\n"; }

    printf("GEN JOIN_GAME OK: req asio %zu B (id=0x000B), resp C1 %zu B (0x42): "
           "map=%u x=%u y=%u angle=%d gold=%u exp=0x%016llX\n",
           req.size(), resp.size(), jm.map, jm.x, jm.y, jm.angleDeg, jm.gold,
           (unsigned long long)jm.exp);
    return 0;
}
