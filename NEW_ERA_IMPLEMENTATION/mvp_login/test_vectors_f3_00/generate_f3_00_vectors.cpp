// NEW-ERA — 1.2-A1 — generate_f3_00_vectors.cpp (WIRE-REAL C1)
// Golden determinístico F3:0x00 (Character List) no formato REAL do wire:
//   REQ  = C1 PLAIN 5 B [C1][05][F3][00][byLanguage=1] + Xor32 [3..5)
//          (SendRequestCharactersList :288-:296; Send() default FALSE).
//   RESP = C1 PLAIN 40 B (1 slot "Hero1"/345 — layout 1.1-B/§54).
// SEM crypto/keys neste gerador (nada de C3). Asserts de roundtrip + parse.
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
    const uint8_t kLang = 1;
    const char* kName = "Hero1";
    const uint16_t kLevel = 345;

    // ---------- REQ C→S: WIRE REAL (C1 plain 5 B + Xor32 [3..5)) ----------
    auto req = mvp::BuildC1_F3_00_RequestCharListWire(kLang);
    assert(req.size() == 5 && req[0] == 0xC1 && req[1] == 5 && req[2] == 0xF3);
    assert(req[3] != 0x00);  // subcode XORed no wire (0x00^0xF3^Filter[3])

    // Roundtrip REQ: decode C1 c/ unXor => subcode/byLanguage legíveis
    crypto::PacketCryptoSM smAny;  // não usada no caminho C1
    const uint8_t ver[5] = { 1, 2, 3, 4, 5 };
    mvp::ParsedMvp oq; std::string eq; std::vector<uint8_t> plainReq;
    assert(mvp::DecodeAndParseMvpPacket(req, smAny, ver, oq, eq, /*streamXored=*/true, &plainReq));
    assert(plainReq.size() == 5 && plainReq[3] == 0x00 && plainReq[4] == kLang);

    // ---------- RESP S→C: C1 plain 40 B (mesmo payload do 1.1-C) ----------
    std::vector<uint8_t> resp(40, 0);
    resp[0] = 0xC1; resp[1] = 40; resp[2] = 0xF3; resp[3] = 0x00;
    resp[4] = 3;    // MaxClass
    resp[5] = 0;    // MoveCount
    resp[6] = 1;    // Count (= Value :532/:537)
    resp[7 + 0] = 0;                              // Index
    std::memcpy(&resp[7 + 1], kName, 5);          // ID[10] zero-padded
    std::memcpy(&resp[7 + 11], &kLevel, 2);       // Level WORD LE
    // CtlCode/Class/Equipment[17]/byGuildStatus = 0

    // Roundtrip RESP: decode C1 (sem xor — GS TX plain p/ C1) + parse char list
    mvp::ParsedMvp orx; std::string erx; std::vector<uint8_t> plainC1;
    assert(mvp::DecodeAndParseMvpPacket(resp, smAny, ver, orx, erx, /*streamXored=*/false, &plainC1));
    assert(orx.head == 0xF3 && orx.sub == 0x00 && plainC1.size() == 40);
    mvp::ParsedCharList cl; std::string ecl;
    assert(mvp::ParseC1_F3_00_CharacterListPlain(plainC1, cl, ecl));
    assert(cl.hdr.count == 1 && cl.hdr.maxClass == 3 && cl.hdr.moveCount == 0);
    assert(cl.chars.size() == 1 && cl.chars[0].index == 0);
    assert(std::string(cl.chars[0].id) == kName && cl.chars[0].level == kLevel);

    // ---------- Saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_00/f3_00_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_00/f3_00_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"F3_00_charlist_wire_c1\",\n";
      j << "  \"req_c1_hex\": \"" << Hex(req) << "\",\n";
      j << "  \"resp_c1_hex\": \"" << Hex(resp) << "\",\n";
      j << "  \"expected_parse\": { \"count\": 1, \"maxClass\": 3, \"moveCount\": 0,\n";
      j << "    \"chars\": [ { \"index\": 0, \"id\": \"" << kName << "\", \"level\": " << kLevel
        << ", \"ctlCode\": 0, \"serverClass\": 0, \"guildStatus\": 0 } ] }\n}\n"; }
    { std::ofstream m(mpath);
      m << "# F3:0x00 CharList — golden vectors (1.2-A1, WIRE-REAL C1)\n\n";
      m << "- REQ C->S **C1 plain** (5 B, byLanguage=1): `" << Hex(req) << "`\n";
      m << "- RESP S->C **C1 plain** (40 B, 1 slot): `" << Hex(resp) << "`\n";
      m << "- expected: count=1, id=`" << kName << "`, level=" << kLevel << ", maxClass=3\n";
      m << "- Wire real: SendRequestCharactersList :288-:296 c/ Send()=FALSE (C1, sem C3/crypto).\n"; }

    printf("GEN F3:00 WIRE OK: req C1 %zu B, resp C1 %zu B, parse 1/%s/%u\n",
           req.size(), resp.size(), cl.chars[0].id, cl.chars[0].level);
    return 0;
}
