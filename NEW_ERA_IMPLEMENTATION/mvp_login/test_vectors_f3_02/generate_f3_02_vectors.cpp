// NEW-ERA — 1.2-A2 — generate_f3_02_vectors.cpp (WIRE-REAL C1)
// Golden determinístico F3:0x02 (Delete) no formato REAL do wire:
//   REQ  = C1 PLAIN 34 B [C1][22][F3][02][ID10 "Hero1"][Resident20] + Xor32 [3..34)
//          (SendRequestDeleteCharacter :310-:320; Send() default FALSE).
//   RESP = C1 PLAIN 5 B [C1][05][F3][02][result=1 SUCCESS].
// SEM crypto/keys (nada de C3). Asserts de roundtrip + parse.
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
    const std::string kId = "Hero1";
    std::array<uint8_t, 20> kRes{};                        // "12345678901234567890"
    const char* kResStr = "12345678901234567890";
    for (int i = 0; i < 20; ++i) kRes[i] = static_cast<uint8_t>(kResStr[i]);

    // ---------- REQ C→S: WIRE REAL (C1 34 B + Xor32 [3..34)) ----------
    auto req = mvp::BuildC1_F3_02_DeleteRequestWire(kId, kRes);
    assert(req.size() == 34 && req[0] == 0xC1 && req[1] == 34 && req[2] == 0xF3);
    assert(req[3] != 0x02);  // subcode XORed no wire

    // Roundtrip REQ: decode C1 c/ unXor => ID + Resident20 intactos
    crypto::PacketCryptoSM smAny;
    const uint8_t ver[5] = { 1, 2, 3, 4, 5 };
    mvp::ParsedMvp oq; std::string eq; std::vector<uint8_t> plainReq;
    assert(mvp::DecodeAndParseMvpPacket(req, smAny, ver, oq, eq, /*streamXored=*/true, &plainReq));
    assert(plainReq.size() == 34 && plainReq[3] == 0x02);
    assert(std::memcmp(&plainReq[4], kId.data(), 5) == 0 && plainReq[9] == 0);
    assert(std::memcmp(&plainReq[14], kRes.data(), 20) == 0);

    // ---------- RESP S→C: C1 plain 5 B result=1 ----------
    std::vector<uint8_t> resp = { 0xC1, 0x05, 0xF3, 0x02, 0x01 };

    mvp::ParsedMvp orx; std::string erx; std::vector<uint8_t> plainC1;
    assert(mvp::DecodeAndParseMvpPacket(resp, smAny, ver, orx, erx, /*streamXored=*/false, &plainC1));
    assert(orx.head == 0xF3 && orx.sub == 0x02 && plainC1.size() == 5);
    uint8_t result = 0xFF;
    assert(mvp::ParseC1_F3_02_DeleteResponsePlain(plainC1, result, erx));
    assert(result == 1);

    // ---------- Saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_02/f3_02_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_02/f3_02_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"F3_02_delete_wire_c1\",\n";
      j << "  \"req_c1_hex\": \"" << Hex(req) << "\",\n";
      j << "  \"resp_c1_hex\": \"" << Hex(resp) << "\",\n";
      j << "  \"expected_parse\": { \"result\": 1, \"meaning\": \"SUCCESS\",\n";
      j << "    \"req_id\": \"" << kId << "\", \"req_resident\": \"" << kResStr << "\" }\n}\n"; }
    { std::ofstream m(mpath);
      m << "# F3:0x02 Delete — golden vectors (1.2-A2, WIRE-REAL C1)\n\n";
      m << "- REQ C->S **C1 plain** (34 B, Resident[20]): `" << Hex(req) << "`\n";
      m << "- RESP S->C **C1 plain** (5 B): `" << Hex(resp) << "`\n";
      m << "- expected: result=1 (SUCCESS); id=" << kId << " resident=" << kResStr << "\n";
      m << "- Wire real: SendRequestDeleteCharacter :310-:320 c/ Send()=FALSE (C1, sem C3/crypto).\n"; }

    printf("GEN F3:02 WIRE OK: req C1 %zu B, resp C1 %zu B, result=%u\n",
           req.size(), resp.size(), result);
    return 0;
}
