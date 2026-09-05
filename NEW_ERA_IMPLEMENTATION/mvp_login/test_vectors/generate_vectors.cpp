// NEW-ERA — MVP LOGIN — generate_vectors.cpp (1.0-F)
// Golden test vectors DETERMINÍSTICOS (sem rede, sem sockets, sem RNG).
// Compilar: ver compile_and_run.sh (usa -DNEWERA_MVP_NO_MAIN e inclui este
// arquivo o TU inteiro de mvp_login_client.cpp).
// Casos:
//   T1: login request C→S (ID="111", PASS="111", Tick=0x11223344,
//       Version=[1..5], Serial[16]=00..0F, serialPkt=0x01 — 1ª chamada do
//       builder, static inicial 0). Roundtrip cripto com Dec1 DERIVADO
//       (inverso modular de Enc1 — relação PROVADA no §48).
//   R1: handshake F1:00 simulado com Xor+Encrypt (modelo stream do cliente)
//       — valida o caminho RX com streamXored=true.
//   R2: login result F1:01 no estilo REAL do GS (Encrypt SEM Xor — DataSend
//       :443-:470) — valida RX com streamXored=false.
// Chaves de S→C simuladas: Enc2-derivado = inverso(Dec2) (§48).
#include "mvp_login_client.cpp"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace newera;

static std::string Hex(const uint8_t* p, size_t n) {
    std::string s;
    char b[3];
    for (size_t i = 0; i < n; ++i) { snprintf(b, sizeof(b), "%02x", p[i]); s += b; }
    return s;
}
static std::string Hex(const std::vector<uint8_t>& v) { return Hex(v.data(), v.size()); }

// Inverso modular (euclides estendido) — usado p/ DERIVAR Dec1 de Enc1 e
// Enc2 de Dec2 (relação §48). Determinístico.
static uint64_t ModInv(uint64_t a, uint64_t m) {
    int64_t g0 = 0, g1 = 1; uint64_t r0 = m, r1 = a % m;
    while (r1 != 0) {
        uint64_t q = r0 / r1;
        uint64_t rt = r0 - q * r1; r0 = r1; r1 = rt;
        int64_t gt = g0 - (int64_t)q * g1; g0 = g1; g1 = gt;
    }
    return (uint64_t)((g0 % (int64_t)m) < 0 ? g0 % (int64_t)m + (int64_t)m : g0 % (int64_t)m);
}
static crypto::PacketCryptoSM::Keys InvertKeys(const crypto::PacketCryptoSM::Keys& k) {
    crypto::PacketCryptoSM::Keys r = k;
    for (int i = 0; i < 4; ++i) r.key[i] = (uint32_t)ModInv(k.key[i], k.modulus[i]);
    return r;
}

static bool LoadSm(crypto::PacketCryptoSM& sm, const char* p1, const char* p2, int type, std::string& err) {
    if (sm.LoadKeysFromFile(p1, &err, type)) return true;
    return sm.LoadKeysFromFile(p2, &err, type);
}

struct CaseOut { std::string name, plain_hex, c3_hex, back_hex; std::string notes; };

int main() {
    std::string err;
    const char* K1 = "NEW_ERA_IMPLEMENTATION/mvp_login/keys/Enc1.dat";
    const char* K1b = "keys/Enc1.dat";
    const char* K2 = "NEW_ERA_IMPLEMENTATION/mvp_login/keys/Dec2.dat";
    const char* K2b = "keys/Dec2.dat";

    // ---------- T1: login request ----------
    crypto::PacketCryptoSM smTx;   // cliente TX (Enc1)
    if (!LoadSm(smTx, K1, K1b, 0, err)) { printf("ERRO T1: %s\n", err.c_str()); return 1; }
    const uint8_t ver[5] = { 1, 2, 3, 4, 5 };
    uint8_t serial16[16]; for (int i = 0; i < 16; ++i) serial16[i] = (uint8_t)i;
    auto c3 = mvp::BuildC3_F1_01_LoginRequestEncrypted("111", "111", 0x11223344u, ver, serial16, smTx, &err);
    assert(!c3.empty() && c3.size() == 68);
    assert(c3[0] == 0xC3 && c3[1] == 68);

    // plain de referência (reconstrução manual p/ golden independente)
    auto plain = mvp::BuildC1_F1_01_LoginRequestPlain("111", "111", 0x11223344u, ver, serial16);
    // inner TX: [serial=0x01][plain[2..49))
    uint8_t inner[48]; inner[0] = 0x01; std::memcpy(&inner[1], plain.data() + 2, 47);

    // roundtrip cripto: Dec1 DERIVADO (inverso de Enc1) deve recuperar inner
    crypto::PacketCryptoSM smRx1; smRx1.SetKeys(InvertKeys(smTx.DebugKeys()));
    uint8_t back[64];
    int bn = smRx1.Decrypt(back, c3.data() + 2, 66);
    assert(bn == 48);
    assert(std::memcmp(back, inner, 48) == 0);

    // ---------- R1: F1:00 simulado (com Xor, modelo stream) ----------
    uint8_t f0[12] = { 0xC1, 0x0C, 0xF1, 0x00, 0x01, 0x12, 0x34, 0, 0, 0, 0, 0 };
    for (int i = 0; i < 5; ++i) f0[7 + i] = (uint8_t)(ver[i] - (i + 1)); // VerEnc = 0,0,0,0,0
    crypto::ApplyStreamXorLikeEvidence(f0);                            // [3..12)
    uint8_t inR1[11]; inR1[0] = 0x01; std::memcpy(&inR1[1], f0 + 2, 10); // serial+head+... (11 B)
    crypto::PacketCryptoSM smSrv;                                       // "server" = Enc2-derivado = inv(Dec2)
    crypto::PacketCryptoSM smCliRx;                                     // cliente RX = Dec2
    std::string e1, e2;
    if (!LoadSm(smCliRx, K2, K2b, 1, e2)) { printf("ERRO R1: %s\n", e2.c_str()); return 1; }
    smSrv.SetKeys(InvertKeys(smCliRx.DebugKeys()));
    uint8_t ctR1[22];
    int n1 = smSrv.Encrypt(ctR1, inR1, 11);
    assert(n1 == 22);
    std::vector<uint8_t> pktR1 = { 0xC3, (uint8_t)(2 + 22) };
    pktR1.insert(pktR1.end(), ctR1, ctR1 + 22);
    mvp::ParsedMvp out1;
    bool ok1 = mvp::DecodeAndParseMvpPacket(pktR1, smCliRx, ver, out1, e1, /*streamXored=*/true);
    assert(ok1 && out1.isJoin && out1.join.ok);
    assert(out1.head == 0xF1 && out1.sub == 0x00);
    assert(out1.join.result == 0x01 && out1.join.heroKey == 0x1234);
    assert(out1.join.version[0] == 0 && out1.versionOk);
    assert(out1.serial == 0x01);

    // ---------- R2: F1:01 estilo GS REAL (Encrypt SEM Xor) ----------
    uint8_t inR2[4] = { 0x02, 0xF1, 0x01, 0x01 };                       // serial, head, sub, Value=1
    uint8_t ctR2[11];
    int n2 = smSrv.Encrypt(ctR2, inR2, 4);
    assert(n2 == 11);
    std::vector<uint8_t> pktR2 = { 0xC3, (uint8_t)(2 + 11) };
    pktR2.insert(pktR2.end(), ctR2, ctR2 + 11);
    mvp::ParsedMvp out2;
    bool ok2 = mvp::DecodeAndParseMvpPacket(pktR2, smCliRx, ver, out2, e2, /*streamXored=*/false);
    assert(ok2 && out2.isLoginResult && out2.loginValue == 0x01 && out2.serial == 0x02);

    // ---------- golden files ----------
    std::ofstream j("NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors/login_vectors.json");
    j << "{\n  \"gerado\": \"1.0-F (deterministico, sem rede/sockets/RNG)\",\n";
    j << "  \"casos\": {\n";
    j << "    \"T1_login_request\": {\n      \"inputs\": { \"id\": \"111\", \"pass\": \"111\", \"tick\": \"0x11223344\",\n";
    j << "        \"version_raw\": [1,2,3,4,5], \"serial16\": \"" << Hex(serial16, 16) << "\", \"serialPkt\": 1 },\n";
    j << "      \"plain49_hex\": \"" << Hex(plain.data(), plain.size()) << "\",\n";
    j << "      \"inner48_hex\": \"" << Hex(inner, 48) << "\",\n";
    j << "      \"c3_68B_hex\": \"" << Hex(c3) << "\",\n";
    j << "      \"roundtrip\": \"decrypt_derivedDec1(c3)==inner48 OK\" },\n";
    j << "    \"R1_join_handshake_sim_streamxor\": {\n      \"frame12_pre_xor_hex\": \"c10cf1000112340000000000\",\n";
    j << "      \"c3_24B_hex\": \"" << Hex(pktR1) << "\",\n";
    j << "      \"parsed\": { \"head\": \"0xF1\", \"sub\": \"0x00\", \"result\": 1, \"heroKey\": \"0x1234\", \"versionOk\": true, \"serial\": 1 } },\n";
    j << "    \"R2_login_result_gs_style_noxor\": {\n      \"inner4_hex\": \"02f10101\",\n";
    j << "      \"c3_13B_hex\": \"" << Hex(pktR2) << "\",\n";
    j << "      \"parsed\": { \"head\": \"0xF1\", \"sub\": \"0x01\", \"value\": 1, \"serial\": 2 } }\n";
    j << "  }\n}\n";
    j.close();

    std::ofstream m("NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors/login_vectors.md");
    m << "# Golden test vectors — MVP login 0xF1 (1.0-F)\n\n";
    m << "Determinístico (sem rede/sockets/RNG). Chaves reais: keys/Enc1.dat (TX) e keys/Dec2.dat (RX);\n";
    m << "Dec1/Enc2-derivados por INVERSO MODULAR (relação §48).\n\n";
    m << "## T1 — login request (C→S)\n- plain49 (pós XorData32): `" << Hex(plain.data(), plain.size()) << "`\n";
    m << "- inner48 (serial+plain[2..49)): `" << Hex(inner, 48) << "`\n";
    m << "- envelope C3 (68 B): `" << Hex(c3) << "`\n- assert roundtrip: OK\n\n";
    m << "## R1 — F1:00 simulado (stream-Xor)\n- C3 (24 B): `" << Hex(pktR1) << "`\n";
    m << "- parse: head=F1 sub=00 result=01 heroKey=0x1234 versionOk=true serial=01 — OK\n\n";
    m << "## R2 — F1:01 estilo GS real (Encrypt sem Xor — DataSend :443-:470)\n";
    m << "- C3 (13 B): `" << Hex(pktR2) << "`\n- parse: value=01 serial=02 — OK\n";
    m.close();

    printf("GENVEC OK: T1 plain=%zuB c3=%zuB | R1 c3=%zuB | R2 c3=%zuB\n",
           plain.size(), c3.size(), pktR1.size(), pktR2.size());
    printf("C3[0..31]: %s\n", Hex(c3.data(), 32).c_str());
    return 0;
}
