// NEW-ERA — 1.1-D — generate_f3_02_vectors.cpp
// Golden determinístico F3:0x02 (Delete Character):
//   REQ  C3 35 B: builder REAL (Enc1; serial 0x01) — id "Hero1", resident "1234567890".
//   RESP C3 13 B: stub GS-style SEM Xor (Enc2-derivado=inverso(Dec2) §48) — result=1 SUCCESS.
// Valida na geração: roundtrip REQ (unXor+Dec1-derivado; confere id/resident) e
// RESP (Dec2; parse result=1). SEM sockets/RNG.
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
    const char* kId = "Hero1";
    const char* kResident = "1234567890";

    // ---------- REQ C→S: builder REAL ----------
    auto req = mvp::BuildC3_F3_02_DeleteRequestEncrypted(kId, kResident, &e);
    assert(!req.empty() && "builder falhou");
    assert(req[0] == 0xC3 && req.size() == 35 && req.size() < 256);

    // Roundtrip REQ: Dec1-derivado + unXor => C1 25 B com id/resident intactos
    crypto::PacketCryptoSM smEnc1;  assert(mvp::TryLoadLoginKeys(smEnc1, e));
    crypto::PacketCryptoSM smReqRx; smReqRx.SetKeys(InvertKeys(smEnc1.DebugKeys()));
    const uint8_t ver[5] = { 1, 2, 3, 4, 5 };
    mvp::ParsedMvp oq; std::string eq; std::vector<uint8_t> plainReq;
    assert(mvp::DecodeAndParseMvpPacket(req, smReqRx, ver, oq, eq, /*streamXored=*/true, &plainReq));
    assert(plainReq.size() == 25 && plainReq[2] == 0xF3 && plainReq[3] == 0x02);
    assert(std::memcmp(&plainReq[4], kId, 5) == 0 && plainReq[9] == 0);          // ID zero-pad
    assert(std::memcmp(&plainReq[14], kResident, 10) == 0);                      // Resident[10]

    // ---------- RESP S→C (stub): C1 5 B result=1 -> Encrypt Enc2-derivado SEM Xor ----------
    crypto::PacketCryptoSM smCliRx;  assert(mvp::TryLoadReceiveKeys(smCliRx, e));    // Dec2.dat
    crypto::PacketCryptoSM smSrv;    smSrv.SetKeys(InvertKeys(smCliRx.DebugKeys()));
    uint8_t inner[4] = { 0x01, 0xF3, 0x02, 0x01 };   // serial, head, sub, result=1 (SUCCESS)
    uint8_t ct[11];
    int n = smSrv.Encrypt(ct, inner, 4);
    assert(n == 11 && "4 B -> 1 bloco parcial -> 11 B");
    std::vector<uint8_t> resp = { 0xC3, static_cast<uint8_t>(2 + 11) };
    resp.insert(resp.end(), ct, ct + 11);
    assert(resp[0] == 0xC3 && resp.size() == 13 && resp.size() < 256);

    // Roundtrip RESP + parse
    mvp::ParsedMvp orx; std::string erx; std::vector<uint8_t> plainC1;
    assert(mvp::DecodeAndParseMvpPacket(resp, smCliRx, ver, orx, erx, /*streamXored=*/false, &plainC1));
    assert(orx.head == 0xF3 && orx.sub == 0x02 && plainC1.size() == 5);
    uint8_t result = 0xFF;
    assert(mvp::ParseC1_F3_02_DeleteResponsePlain(plainC1, result, erx));
    assert(result == 1);

    // ---------- Saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_02/f3_02_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_02/f3_02_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"F3_02_delete\",\n";
      j << "  \"req_c3_hex\": \"" << Hex(req) << "\",\n";
      j << "  \"resp_c3_hex\": \"" << Hex(resp) << "\",\n";
      j << "  \"expected_parse\": { \"result\": 1, \"meaning\": \"SUCCESS\",\n";
      j << "    \"req_id\": \"" << kId << "\", \"req_resident\": \"" << kResident << "\" }\n}\n"; }
    { std::ofstream m(mpath);
      m << "# F3:0x02 Delete — golden vectors (1.1-D)\n\n";
      m << "- REQ C->S (C3, 35 B): `" << Hex(req) << "`\n";
      m << "- RESP S->C stub GS-style SEM Xor (C3, 13 B): `" << Hex(resp) << "`\n";
      m << "- expected: result=1 (SUCCESS); req id=" << kId << " resident=" << kResident << "\n";
      m << "- Chaves: Enc1 (req) / Enc2-derivado=inverso(Dec2) (resp, §48). Serials 0x01.\n"; }

    printf("GEN F3:02 OK: req %zu B, resp %zu B, result=%u\n", req.size(), resp.size(), result);
    return 0;
}
