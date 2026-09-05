// NEW-ERA — 1.1-F — generate_f3_52_vectors.cpp
// Golden determinístico F3:0x52 (Master Skill):
//   REQ  = WIRE REAL C1 PLAIN 8 B (SendRequestMasterLevelSkill :2286-:2292,
//          Send() default FALSE): [C1][08][F3][52][skillNum i32 LE] + Xor32 [3..8).
//   RESP = C3 24 B stub GS-style SEM Xor (Enc2-derivado=inverso(Dec2) §48):
//          C1 15 B [F3][52][btResult=1][nMLPoint=5][nSkillNum=123456][nSkillLevel=3].
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
static crypto::PacketCryptoSM::Keys InvertKeys(const crypto::PacketCryptoSM::Keys& k) {
    crypto::PacketCryptoSM::Keys r{};
    auto inv = [](uint32_t a, uint32_t m) {
        int64_t t = 0, nt = 1, r0 = m, r1 = a;
        while (r1) { int64_t q = r0 / r1, tmp = t - q * nt; t = nt; nt = tmp;
                     tmp = r0 - q * r1; r0 = r1; r1 = tmp; }
        return t < 0 ? t + m : t;
    };
    for (int i = 0; i < 4; ++i) {
        r.modulus[i] = k.modulus[i];
        r.key[i] = static_cast<uint32_t>(inv(k.key[i], k.modulus[i]));
        r.xor_[i]  = k.xor_[i];
    }
    return r;
}

int main() {
    std::string e;
    const int32_t kSkillNum = 123456;        // 0x0001E240
    const uint8_t kResult = 1;
    const int16_t kMlPoint = 5;
    const int32_t kSkillLevel = 3;

    // ---------- REQ C→S: wire REAL (C1 plain) ----------
    auto req = mvp::BuildC1_F3_52_MasterSkillRequestPlain(kSkillNum);
    assert(req.size() == 8 && req[0] == 0xC1 && req[2] == 0xF3);   // [3] vai XORed (ver abaixo)
    assert(req[3] != 0x52);  // subcode foi XORed (0x52^0xF3^Filter[3]) — wire real
    // Roundtrip: Decode (C1 direto) com unXor => skillNum legível
    const uint8_t ver[5] = { 1, 2, 3, 4, 5 };
    crypto::PacketCryptoSM smAny;  // não usada no caminho C1
    mvp::ParsedMvp oq; std::string eq; std::vector<uint8_t> plainReq;
    assert(mvp::DecodeAndParseMvpPacket(req, smAny, ver, oq, eq, /*streamXored=*/true, &plainReq));
    assert(plainReq.size() == 8 && plainReq[3] == 0x52);
    int32_t got = 0; std::memcpy(&got, &plainReq[4], 4);
    assert(got == kSkillNum);

    // Também exercita o wrapper C3 (opcional de pipeline): 13 B
    auto reqC3 = mvp::BuildC3_F3_52_MasterSkillRequestEncrypted(kSkillNum, &e);
    assert(!reqC3.empty() && reqC3.size() == 13);

    // ---------- RESP S→C (stub): C1 15 B -> Encrypt Enc2-derivado SEM Xor ----------
    crypto::PacketCryptoSM smCliRx;  assert(mvp::TryLoadReceiveKeys(smCliRx, e));
    crypto::PacketCryptoSM smSrv;    smSrv.SetKeys(InvertKeys(smCliRx.DebugKeys()));
    std::vector<uint8_t> plain(15, 0);
    plain[0] = 0xC1; plain[1] = 15; plain[2] = 0xF3; plain[3] = 0x52;
    plain[4] = kResult;
    std::memcpy(&plain[5], &kMlPoint, 2);
    std::memcpy(&plain[7], &kSkillNum, 4);
    std::memcpy(&plain[11], &kSkillLevel, 4);
    uint8_t inner[14];
    inner[0] = 0x01;                                        // serial RX fixo (golden)
    std::memcpy(&inner[1], plain.data() + 2, 13);
    uint8_t ct[22];
    int n = smSrv.Encrypt(ct, inner, 14);
    assert(n == 22 && "14 B -> 8 cheio + 6 parcial -> 22 B");
    std::vector<uint8_t> resp = { 0xC3, static_cast<uint8_t>(2 + 22) };
    resp.insert(resp.end(), ct, ct + 22);
    assert(resp[0] == 0xC3 && resp.size() == 24 && resp.size() < 256);

    // Roundtrip RESP + parse
    mvp::ParsedMvp orx; std::string erx; std::vector<uint8_t> plainC1;
    assert(mvp::DecodeAndParseMvpPacket(resp, smCliRx, ver, orx, erx, /*streamXored=*/false, &plainC1));
    assert(orx.head == 0xF3 && orx.sub == 0x52 && plainC1.size() == 15);
    mvp::ParsedMasterSkill ms; std::string perr;
    assert(mvp::ParseC1_F3_52_MasterSkillResponsePlain(plainC1, ms, perr));
    assert(ms.result == kResult && ms.mlPoint == kMlPoint &&
           ms.skillNum == kSkillNum && ms.skillLevel == kSkillLevel);

    // ---------- Saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_52/f3_52_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_52/f3_52_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"F3_52_masterskill\",\n";
      j << "  \"req_c1_hex\": \"" << Hex(req) << "\",\n";
      j << "  \"req_c3_hex\": \"" << Hex(reqC3) << "\",\n";
      j << "  \"resp_c3_hex\": \"" << Hex(resp) << "\",\n";
      j << "  \"expected_parse\": { \"result\": 1, \"mlPoint\": 5,\n";
      j << "    \"skillNum\": 123456, \"skillLevel\": 3 }\n}\n"; }
    { std::ofstream m(mpath);
      m << "# F3:0x52 MasterSkill — golden vectors (1.1-F)\n\n";
      m << "- REQ C->S **WIRE REAL C1 plain** (8 B): `" << Hex(req) << "`\n";
      m << "- REQ C3 opcional de pipeline (13 B): `" << Hex(reqC3) << "`\n";
      m << "- RESP S->C stub GS-style SEM Xor (C3, 24 B): `" << Hex(resp) << "`\n";
      m << "- expected: result=1, mlPoint=5, skillNum=123456, skillLevel=3\n";
      m << "- Serial 0x01 (resp). Loopback usa o REQ C1 real (primeiro golden wire-format).\n"; }

    printf("GEN F3:52 OK: req C1 %zu B (wire real), reqC3 %zu B, resp %zu B, skill=%d\n",
           req.size(), reqC3.size(), resp.size(), ms.skillNum);
    return 0;
}
