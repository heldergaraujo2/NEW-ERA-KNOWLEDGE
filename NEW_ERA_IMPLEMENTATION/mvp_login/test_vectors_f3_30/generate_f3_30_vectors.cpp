// NEW-ERA — 1.1-E — generate_f3_30_vectors.cpp
// Golden determinístico F3:0x30 (Option Data):
//   REQ  C3 57 B: builder REAL (Enc1; serial 0x01) — option[30] fixa (ver abaixo).
//   RESP C3 57 B: stub GS-style SEM Xor (Enc2-derivado=inverso(Dec2) §48).
// option[30]: HotKey par0={0x12,0x34}(->0x1234 c/ swap :9398), pares 1-9=0xFFFF
// (vazio); GameOption=0xA5; KeyQWE={'Q','W','E'}; ChatLogBox=1; KeyR=2; QWERLevel=100.
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

    // option[30] fixa (payload espelhado request<->response — spec §2/§3)
    uint8_t option[30] = { 0 };
    option[0] = 0x12; option[1] = 0x34;                    // hotKey0 = 0x1234 (swap :9398)
    for (int i = 1; i < 10; ++i) { option[2*i] = 0xFF; option[2*i+1] = 0xFF; }  // vazio
    option[20] = 0xA5;                                      // GameOption
    option[21] = 'Q'; option[22] = 'W'; option[23] = 'E';   // KeyQWE
    option[24] = 0x01;                                      // ChatLogBox
    option[25] = 0x02;                                      // KeyR
    const int32_t kQwer = 100;                              // QWERLevel (int LE)
    std::memcpy(&option[26], &kQwer, 4);

    // ---------- REQ C→S: builder REAL ----------
    auto req = mvp::BuildC3_F3_30_OptionRequestEncrypted(option, &e);
    assert(!req.empty() && "builder falhou");
    assert(req[0] == 0xC3 && req.size() == 57 && req.size() < 256);

    // Roundtrip REQ: unXor + Dec1-derivado => option intacta
    crypto::PacketCryptoSM smEnc1;  assert(mvp::TryLoadLoginKeys(smEnc1, e));
    crypto::PacketCryptoSM smReqRx; smReqRx.SetKeys(InvertKeys(smEnc1.DebugKeys()));
    const uint8_t ver[5] = { 1, 2, 3, 4, 5 };
    mvp::ParsedMvp oq; std::string eq; std::vector<uint8_t> plainReq;
    assert(mvp::DecodeAndParseMvpPacket(req, smReqRx, ver, oq, eq, /*streamXored=*/true, &plainReq));
    assert(plainReq.size() == 34 && plainReq[2] == 0xF3 && plainReq[3] == 0x30);
    assert(std::memcmp(&plainReq[4], option, 30) == 0);

    // ---------- RESP S→C (stub): mesmo payload -> Encrypt Enc2-derivado SEM Xor ----------
    crypto::PacketCryptoSM smCliRx;  assert(mvp::TryLoadReceiveKeys(smCliRx, e));
    crypto::PacketCryptoSM smSrv;    smSrv.SetKeys(InvertKeys(smCliRx.DebugKeys()));
    std::vector<uint8_t> plain(34, 0);
    plain[0] = 0xC1; plain[1] = 34; plain[2] = 0xF3; plain[3] = 0x30;
    std::memcpy(&plain[4], option, 30);
    uint8_t inner[33];
    inner[0] = 0x01;                                        // serial RX fixo (golden)
    std::memcpy(&inner[1], plain.data() + 2, 32);
    uint8_t ct[55];
    int n = smSrv.Encrypt(ct, inner, 33);
    assert(n == 55 && "33 B -> 4 cheios + parcial 1 -> 55 B");
    std::vector<uint8_t> resp = { 0xC3, static_cast<uint8_t>(2 + 55) };
    resp.insert(resp.end(), ct, ct + 55);
    assert(resp[0] == 0xC3 && resp.size() == 57 && resp.size() < 256);

    // Roundtrip RESP + parse (swap :9398 e campos crus)
    mvp::ParsedMvp orx; std::string erx; std::vector<uint8_t> plainC1;
    assert(mvp::DecodeAndParseMvpPacket(resp, smCliRx, ver, orx, erx, /*streamXored=*/false, &plainC1));
    assert(orx.head == 0xF3 && orx.sub == 0x30 && plainC1.size() == 34);
    mvp::ParsedOption opt; std::string perr;
    assert(mvp::ParseC1_F3_30_OptionResponsePlain(plainC1, opt, perr));
    assert(opt.hotKeys[0] == 0x1234);
    for (int i = 1; i < 10; ++i) assert(opt.hotKeys[i] == 0xFFFF);
    assert(opt.gameOption == 0xA5 && opt.keyQWE[0] == 'Q' && opt.keyQWE[1] == 'W' && opt.keyQWE[2] == 'E');
    assert(opt.chatLogBox == 1 && opt.keyR == 2 && opt.qwerLevel == 100);

    // ---------- Saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_30/f3_30_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_30/f3_30_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"F3_30_option\",\n";
      j << "  \"req_c3_hex\": \"" << Hex(req) << "\",\n";
      j << "  \"resp_c3_hex\": \"" << Hex(resp) << "\",\n";
      j << "  \"expected_parse\": { \"hotKey0\": 4660, \"hotKeysEmpty\": 9,\n";
      j << "    \"gameOption\": 165, \"keyQWE\": \"QWE\", \"chatLogBox\": 1, \"keyR\": 2, \"qwerLevel\": 100 }\n}\n"; }
    { std::ofstream m(mpath);
      m << "# F3:0x30 Option — golden vectors (1.1-E)\n\n";
      m << "- REQ C->S (C3, 57 B): `" << Hex(req) << "`\n";
      m << "- RESP S->C stub GS-style SEM Xor (C3, 57 B): `" << Hex(resp) << "`\n";
      m << "- expected: hotKey0=0x1234 (swap :9398), 9x 0xFFFF, gameOption=0xA5, QWE, chatLog=1, keyR=2, qwer=100\n";
      m << "- Wire real: C1 PLAIN 34 B (Send() default FALSE — spec §2); C3 é convenção NEW-ERA.\n"; }

    printf("GEN F3:30 OK: req %zu B, resp %zu B, hotKey0=0x%04x qwer=%d\n",
           req.size(), resp.size(), opt.hotKeys[0], opt.qwerLevel);
    return 0;
}
