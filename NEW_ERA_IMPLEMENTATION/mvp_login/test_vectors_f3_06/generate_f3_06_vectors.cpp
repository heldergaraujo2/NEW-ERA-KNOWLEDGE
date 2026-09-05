// NEW-ERA — 1.3-A — generate_f3_06_vectors.cpp (WIRE-REAL C1)
// Golden determinístico F3:0x06 (Add Point) no formato REAL do wire:
//   REQ  = C1 PLAIN 5 B [C1][05][F3][06][pointType=2 (VIT)] + Xor32 [3..5)
//          (SendRequestAddPoint :1189-:1195; Send() default FALSE).
//   RESP = C1 PLAIN 11 B [C1][0B][F3][06][Result=0x12][Max:W][ShieldMax:W][SkillManaMax:W]
//          (PRECEIVE_ADD_POINT :882-:890; Result nibble: high=1 sucesso, low=2 VIT;
//           Max = dual-use LifeMax p/ VIT :6216). SEM crypto/keys.
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
    const uint8_t  kPointType = 0x02;   // VIT (correlação switch RX :6214-:6216)
    const uint8_t  kResult    = 0x12;   // high=1 sucesso (:6203/:6205); low=2 VIT (:6206)
    const uint16_t kMax       = 1000;   // LifeMax no caso VIT (:6216)
    const uint16_t kShield    = 3000;   // :6227
    const uint16_t kSkill     = 4000;   // :6226

    // ---------- REQ C→S: WIRE REAL (C1 5 B + Xor32 [3..5)) ----------
    auto req = mvp::BuildC1_F3_06_AddPointRequestWire(kPointType);
    assert(req.size() == 5 && req[0] == 0xC1 && req[1] == 5 && req[2] == 0xF3);
    assert(req[3] != 0x06);  // subcode XORed no wire

    // Roundtrip REQ: decode C1 c/ unXor => pointType legível
    crypto::PacketCryptoSM smAny;
    const uint8_t ver[5] = { 1, 2, 3, 4, 5 };
    mvp::ParsedMvp oq; std::string eq; std::vector<uint8_t> plainReq;
    assert(mvp::DecodeAndParseMvpPacket(req, smAny, ver, oq, eq, /*streamXored=*/true, &plainReq));
    assert(plainReq.size() == 5 && plainReq[3] == 0x06 && plainReq[4] == kPointType);

    // ---------- RESP S→C: C1 plain 11 B (layout PRECEIVE_ADD_POINT) ----------
    std::vector<uint8_t> resp = { 0xC1, 0x0B, 0xF3, 0x06, kResult, 0, 0, 0, 0, 0, 0 };
    std::memcpy(&resp[5], &kMax, 2);
    std::memcpy(&resp[7], &kShield, 2);
    std::memcpy(&resp[9], &kSkill, 2);
    assert(resp.size() == 11);

    mvp::ParsedMvp orx; std::string erx; std::vector<uint8_t> plainC1;
    assert(mvp::DecodeAndParseMvpPacket(resp, smAny, ver, orx, erx, /*streamXored=*/false, &plainC1));
    assert(orx.head == 0xF3 && orx.sub == 0x06 && plainC1.size() == 11);
    mvp::ParsedAddPoint ap; std::string perr;
    assert(mvp::ParseC1_F3_06_AddPointResponsePlain(plainC1, ap, perr));
    assert(ap.ok && ap.result == kResult);
    assert(ap.statId == kPointType);        // stat == pointType (VIT)
    assert(ap.max == kMax);                 // LifeMax (VIT :6216)
    assert(ap.shieldMax == kShield && ap.skillManaMax == kSkill);

    // ---------- Saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_06/f3_06_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_06/f3_06_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"F3_06_addpoint_wire_c1\",\n";
      j << "  \"req_c1_hex\": \"" << Hex(req) << "\",\n";
      j << "  \"resp_c1_hex\": \"" << Hex(resp) << "\",\n";
      j << "  \"expected_parse\": { \"result\": 18, \"success\": true, \"stat\": 2,\n";
      j << "    \"max\": 1000, \"shieldMax\": 3000, \"skillManaMax\": 4000 }\n}\n"; }
    { std::ofstream m(mpath);
      m << "# F3:0x06 AddPoint — golden vectors (1.3-A, WIRE-REAL C1)\n\n";
      m << "- REQ C->S **C1 plain** (5 B, pointType=2 VIT): `" << Hex(req) << "`\n";
      m << "- RESP S->C **C1 plain** (11 B): `" << Hex(resp) << "`\n";
      m << "- expected: result=0x12 (ok|VIT), stat=2, max=1000 (LifeMax p/ VIT :6216), shield=3000, skillMana=4000\n";
      m << "- Wire real: SendRequestAddPoint :1189-:1195 c/ Send()=FALSE (C1; sem C3/crypto).\n"; }

    printf("GEN F3:06 WIRE OK: req C1 %zu B, resp C1 %zu B, ok=%d stat=%u max=%u\n",
           req.size(), resp.size(), ap.ok, ap.statId, ap.max);
    return 0;
}
