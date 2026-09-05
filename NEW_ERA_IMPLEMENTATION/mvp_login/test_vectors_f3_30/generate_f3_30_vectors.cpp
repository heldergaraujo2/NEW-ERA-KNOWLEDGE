// NEW-ERA — 1.2-A3 — generate_f3_30_vectors.cpp (WIRE-REAL C1)
// Golden determinístico F3:0x30 (Option/HotKey) no formato REAL do wire:
//   REQ  = C1 PLAIN 34 B [C1][22][F3][30][option[30]] + Xor32 [3..34)
//          (SendRequestHotKey :1597-:1603; Send() default FALSE).
//   RESP = C1 PLAIN 34 B (PRECEIVE_OPTION; mesmos bytes de option).
// SEM crypto/keys (nada de C3). Asserts de roundtrip + parse (swap :9398).
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
    // option[30] fixa (igual ao 1.1-E para comparabilidade)
    std::array<uint8_t, 30> option{};
    option[0] = 0x12; option[1] = 0x34;                    // hotKey0 = 0x1234 (swap :9398)
    for (int i = 1; i < 10; ++i) { option[2*i] = 0xFF; option[2*i+1] = 0xFF; }  // vazio
    option[20] = 0xA5;                                      // GameOption
    option[21] = 'Q'; option[22] = 'W'; option[23] = 'E';   // KeyQWE
    option[24] = 0x01;                                      // ChatLogBox
    option[25] = 0x02;                                      // KeyR
    const int32_t kQwer = 100;
    std::memcpy(&option[26], &kQwer, 4);                    // QWERLevel

    // ---------- REQ C→S: WIRE REAL (C1 34 B + Xor32 [3..34)) ----------
    auto req = mvp::BuildC1_F3_30_OptionRequestWire(option);
    assert(req.size() == 34 && req[0] == 0xC1 && req[1] == 34 && req[2] == 0xF3);
    assert(req[3] != 0x30);  // subcode XORed no wire

    // Roundtrip REQ: decode C1 c/ unXor => option intacta
    crypto::PacketCryptoSM smAny;
    const uint8_t ver[5] = { 1, 2, 3, 4, 5 };
    mvp::ParsedMvp oq; std::string eq; std::vector<uint8_t> plainReq;
    assert(mvp::DecodeAndParseMvpPacket(req, smAny, ver, oq, eq, /*streamXored=*/true, &plainReq));
    assert(plainReq.size() == 34 && plainReq[3] == 0x30);
    assert(std::memcmp(&plainReq[4], option.data(), 30) == 0);

    // ---------- RESP S→C: C1 plain 34 B (mesma estrutura/payload) ----------
    std::vector<uint8_t> resp(34, 0);
    resp[0] = 0xC1; resp[1] = 34; resp[2] = 0xF3; resp[3] = 0x30;
    std::memcpy(&resp[4], option.data(), 30);

    mvp::ParsedMvp orx; std::string erx; std::vector<uint8_t> plainC1;
    assert(mvp::DecodeAndParseMvpPacket(resp, smAny, ver, orx, erx, /*streamXored=*/false, &plainC1));
    assert(orx.head == 0xF3 && orx.sub == 0x30 && plainC1.size() == 34);
    mvp::ParsedOption opt; std::string perr;
    assert(mvp::ParseC1_F3_30_OptionResponsePlain(plainC1, opt, perr));
    assert(opt.hotKeys[0] == 0x1234);
    for (int i = 1; i < 10; ++i) assert(opt.hotKeys[i] == 0xFFFF);
    assert(opt.gameOption == 0xA5 && opt.keyQWE[0] == 'Q' && opt.keyQWE[2] == 'E');
    assert(opt.chatLogBox == 1 && opt.keyR == 2 && opt.qwerLevel == 100);

    // ---------- Saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_30/f3_30_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_30/f3_30_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"F3_30_option_wire_c1\",\n";
      j << "  \"req_c1_hex\": \"" << Hex(req) << "\",\n";
      j << "  \"resp_c1_hex\": \"" << Hex(resp) << "\",\n";
      j << "  \"expected_parse\": { \"hotKey0\": 4660, \"hotKeysEmpty\": 9,\n";
      j << "    \"gameOption\": 165, \"keyQWE\": \"QWE\", \"chatLogBox\": 1, \"keyR\": 2, \"qwerLevel\": 100 }\n}\n"; }
    { std::ofstream m(mpath);
      m << "# F3:0x30 Option — golden vectors (1.2-A3, WIRE-REAL C1)\n\n";
      m << "- REQ C->S **C1 plain** (34 B): `" << Hex(req) << "`\n";
      m << "- RESP S->C **C1 plain** (34 B): `" << Hex(resp) << "`\n";
      m << "- expected: hotKey0=0x1234 (swap :9398), 9x 0xFFFF, gameOption=0xA5, QWE, chatLog=1, keyR=2, qwer=100\n";
      m << "- Wire real: SendRequestHotKey :1597-:1603 c/ Send()=FALSE (C1; C3 era só teste antigo).\n"; }

    printf("GEN F3:30 WIRE OK: req C1 %zu B, resp C1 %zu B, hotKey0=0x%04x qwer=%d\n",
           req.size(), resp.size(), opt.hotKeys[0], opt.qwerLevel);
    return 0;
}
