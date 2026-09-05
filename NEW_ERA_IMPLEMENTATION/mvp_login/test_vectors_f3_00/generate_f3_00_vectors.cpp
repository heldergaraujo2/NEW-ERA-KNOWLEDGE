// NEW-ERA — 1.1-C — generate_f3_00_vectors.cpp
// Golden vectors determinísticos para F3:0x00 (Character List):
//   REQ  (C→S): BuildC3_F3_00_RequestCharListEncrypted() — builder REAL (Enc1;
//               serial 0x01 na 1ª chamada) ⇒ C3 13 B.
//   RESP (S→C, SIMULADO/GOLDEN-STUB): C1 plain 40 B (1 slot de 33 B — layout
//               provado 1.1-B: MAX_ID_SIZE=10, EQUIPMENT_LENGTH=17) cifrado
//               ESTILO GS (Encrypt SEM Xor — DataSend :443-:470) com
//               Enc2-derivado = inverso(Dec2) (§48) ⇒ C3 57 B.
// Valida na geração: roundtrip RX do REQ (unXor + Dec1-derivado) e do RESP
// (Dec2), parse da char list e asserts campo-a-campo. SEM sockets/RNG.
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

// Inverso modular das chaves (relação §48 — igual ao generate_vectors.cpp 1.0-F).
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

    // ---------- REQ C→S: builder REAL (carrega Enc1; serial estático 0x01) ----------
    auto req = mvp::BuildC3_F3_00_RequestCharListEncrypted(&e);
    assert(!req.empty() && "builder falhou");
    assert(req[0] == 0xC3 && req.size() == 13 && req.size() < 256);

    // Roundtrip do REQ: Dec1-derivado = inverso(Enc1); unXor (builder aplicou Xor)
    crypto::PacketCryptoSM smEnc1;  assert(mvp::TryLoadLoginKeys(smEnc1, e));
    crypto::PacketCryptoSM smReqRx; smReqRx.SetKeys(InvertKeys(smEnc1.DebugKeys()));
    const uint8_t ver[5] = { 1, 2, 3, 4, 5 };
    mvp::ParsedMvp oq; std::string eq; std::vector<uint8_t> plainReq;
    assert(mvp::DecodeAndParseMvpPacket(req, smReqRx, ver, oq, eq, /*streamXored=*/true, &plainReq));
    assert(plainReq.size() == 4 && plainReq[2] == 0xF3 && plainReq[3] == 0x00);  // unXor reverteu 0x7A→0x00

    // ---------- RESP S→C (golden-stub): C1 plain 40 B → Encrypt Enc2-derivado SEM Xor ----------
    const char* kName = "Hero1";                 // 5 chars + zero-pad (MAX_ID_SIZE=10)
    const uint16_t kLevel = 345;                 // 0x0159
    std::vector<uint8_t> plain(40, 0);
    plain[0] = 0xC1; plain[1] = 40; plain[2] = 0xF3; plain[3] = 0x00;
    plain[4] = 3;    // MaxClass
    plain[5] = 0;    // MoveCount
    plain[6] = 1;    // Count (= Value, :532/:537)
    plain[7 + 0] = 0;                                       // Index
    std::memcpy(&plain[7 + 1], kName, 5);                   // ID[10] zero-padded
    std::memcpy(&plain[7 + 11], &kLevel, 2);                // Level WORD LE
    plain[7 + 13] = 0;                                      // CtlCode
    plain[7 + 14] = 0;                                      // Class (server-type bruta)
    // Equipment[17] @7+15 e byGuildStatus @7+32 já são 0

    crypto::PacketCryptoSM smCliRx;  assert(mvp::TryLoadReceiveKeys(smCliRx, e));   // Dec2.dat
    crypto::PacketCryptoSM smSrv;    smSrv.SetKeys(InvertKeys(smCliRx.DebugKeys())); // Enc2-derivado

    uint8_t inner[39];                                       // serial + [2..40) do plain
    inner[0] = 0x01;                                         // serial RX fixo (golden)
    std::memcpy(&inner[1], plain.data() + 2, 38);
    uint8_t ct[55];
    int n = smSrv.Encrypt(ct, inner, 39);
    assert(n == 55 && "39 B -> 5 blocos (1 parcial de 7) -> 55 B");
    std::vector<uint8_t> resp = { 0xC3, static_cast<uint8_t>(2 + 55) };
    resp.insert(resp.end(), ct, ct + 55);
    assert(resp[0] == 0xC3 && resp.size() == 57 && resp.size() < 256);

    // Roundtrip do RESP + parse da char list (fim-a-fim na geração)
    mvp::ParsedMvp orx; std::string erx; std::vector<uint8_t> plainC1;
    assert(mvp::DecodeAndParseMvpPacket(resp, smCliRx, ver, orx, erx, /*streamXored=*/false, &plainC1));
    assert(orx.head == 0xF3 && orx.sub == 0x00 && plainC1.size() == 40);
    mvp::ParsedCharList cl; std::string ecl;
    assert(mvp::ParseC1_F3_00_CharacterListPlain(plainC1, cl, ecl));
    assert(cl.hdr.count == 1 && cl.hdr.maxClass == 3 && cl.hdr.moveCount == 0);
    assert(cl.chars.size() == 1 && cl.chars[0].index == 0);
    assert(std::string(cl.chars[0].id) == kName && cl.chars[0].level == kLevel);
    assert(cl.chars[0].ctlCode == 0 && cl.chars[0].serverClass == 0 && cl.chars[0].guildStatus == 0);

    // ---------- Saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_00/f3_00_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_00/f3_00_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"F3_00_charlist\",\n";
      j << "  \"req_c3_hex\": \"" << Hex(req) << "\",\n";
      j << "  \"resp_c3_hex\": \"" << Hex(resp) << "\",\n";
      j << "  \"expected_parse\": { \"count\": 1, \"maxClass\": 3, \"moveCount\": 0,\n";
      j << "    \"chars\": [ { \"index\": 0, \"id\": \"" << kName << "\", \"level\": " << kLevel
        << ", \"ctlCode\": 0, \"serverClass\": 0, \"guildStatus\": 0 } ] }\n}\n"; }
    { std::ofstream m(mpath);
      m << "# F3:0x00 CharList — golden vectors (1.1-C)\n\n";
      m << "- REQ C->S (C3, 13 B): `" << Hex(req) << "`\n";
      m << "- RESP S->C stub GS-style SEM Xor (C3, 57 B): `" << Hex(resp) << "`\n";
      m << "- expected: count=1, id=`" << kName << "`, level=" << kLevel << ", maxClass=3\n";
      m << "- Chaves: Enc1 (req) / Enc2-derivado=inverso(Dec2) (resp, §48). Serials 0x01.\n"; }

    printf("GEN F3:00 OK: req %zu B, resp %zu B, parse 1/%s/%u\n",
           req.size(), resp.size(), cl.chars[0].id, cl.chars[0].level);
    return 0;
}
