// NEW-ERA — 1.3-B — generate_f3_01_vectors.cpp (WIRE-REAL C1)
// Golden determinístico F3:0x01 (Create Character) no formato REAL do wire:
//   REQ  = C1 PLAIN 15 B [C1][0F][F3][01][ID10 "HeroNew"][classSkin=0x32]
//          + Xor32 [3..15) (SendRequestCreateCharacter :298-:308; Send() FALSE).
//          NOTA: no wire o último byte viaja XORed; 0x32 é conferido no plain
//          pós-decode (roundtrip).
//   RESP = C1 PLAIN 19 B [C1][13][F3][01][Result=1][ID10][Index=0][Level=1][Class=3]
//          (PRECEIVE_CREATE_CHARACTER :376-:386). SEM crypto/keys.
#include "mvp_login_client.cpp"

#include <cassert>
#include <cstdio>
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
    const std::string kId = "HeroNew";
    const uint8_t kClass = 3, kSkin = 2;         // classSkin = (3<<4)|2 = 0x32
    const uint8_t kResult = 1, kIndex = 0, kClassResp = 3;
    const uint16_t kLevel = 1;

    // ---------- REQ C→S: WIRE REAL (C1 15 B + Xor32 [3..15)) ----------
    auto req = mvp::BuildC1_F3_01_CreateCharRequestWire(kId, kClass, kSkin);
    assert(req.size() == 15 && req[0] == 0xC1 && req[1] == 15 && req[2] == 0xF3);
    assert(req[14] != 0x32);  // classSkin XORed no wire (conferido no plain abaixo)

    crypto::PacketCryptoSM smAny;
    const uint8_t ver[5] = { 1, 2, 3, 4, 5 };
    mvp::ParsedMvp oq; std::string eq; std::vector<uint8_t> plainReq;
    assert(mvp::DecodeAndParseMvpPacket(req, smAny, ver, oq, eq, /*streamXored=*/true, &plainReq));
    assert(plainReq.size() == 15 && plainReq[3] == 0x01);
    assert(plainReq[14] == 0x32);                                   // (3<<4)|2 :306
    assert(std::memcmp(&plainReq[4], kId.data(), 7) == 0 && plainReq[11] == 0);  // zero-pad

    // ---------- RESP S→C: C1 plain 19 B (PRECEIVE_CREATE_CHARACTER) ----------
    std::vector<uint8_t> resp(19, 0);
    resp[0] = 0xC1; resp[1] = 19; resp[2] = 0xF3; resp[3] = 0x01;
    resp[4] = kResult;
    std::memcpy(&resp[5], kId.data(), 7);       // ID[10] zero-padded
    resp[15] = kIndex;
    std::memcpy(&resp[16], &kLevel, 2);
    resp[18] = kClassResp;

    mvp::ParsedMvp orx; std::string erx; std::vector<uint8_t> plainC1;
    assert(mvp::DecodeAndParseMvpPacket(resp, smAny, ver, orx, erx, /*streamXored=*/false, &plainC1));
    assert(orx.head == 0xF3 && orx.sub == 0x01 && plainC1.size() == 19);
    mvp::ParsedCreateChar cc; std::string perr;
    assert(mvp::ParseC1_F3_01_CreateCharResponsePlain(plainC1, cc, perr));
    assert(cc.result == 1 && cc.index == 0 && cc.level == 1);
    assert(std::string(cc.id) == kId && cc.serverClass == kClassResp);

    // ---------- Saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_01/f3_01_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_01/f3_01_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"F3_01_createchar_wire_c1\",\n";
      j << "  \"req_c1_hex\": \"" << Hex(req) << "\",\n";
      j << "  \"resp_c1_hex\": \"" << Hex(resp) << "\",\n";
      j << "  \"expected_parse\": { \"result\": 1, \"id\": \"" << kId << "\",\n";
      j << "    \"index\": 0, \"level\": 1, \"serverClass\": 3, \"classSkin\": 50 }\n}\n"; }
    { std::ofstream m(mpath);
      m << "# F3:0x01 CreateCharacter — golden vectors (1.3-B, WIRE-REAL C1)\n\n";
      m << "- REQ C->S **C1 plain** (15 B, id/classSkin=0x32): `" << Hex(req) << "`\n";
      m << "- RESP S->C **C1 plain** (19 B): `" << Hex(resp) << "`\n";
      m << "- expected: result=1, id=" << kId << ", index=0, level=1, serverClass=3\n";
      m << "- Wire: SendRequestCreateCharacter :298-:308 c/ Send()=FALSE (C1; sem C3).\n"; }

    printf("GEN F3:01 WIRE OK: req C1 %zu B, resp C1 %zu B, result=%u id=%s lvl=%u\n",
           req.size(), resp.size(), cc.result, cc.id, cc.level);
    return 0;
}
