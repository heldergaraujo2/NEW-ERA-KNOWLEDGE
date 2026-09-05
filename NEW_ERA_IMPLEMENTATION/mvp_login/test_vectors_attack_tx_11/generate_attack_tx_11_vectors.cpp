// NEW-ERA — 1.3-N — generate_attack_tx_11_vectors.cpp (TX normal attack C1 0x11)
// Golden determinístico do 1º TX de combate C->S + RESP damage (ciclo mínimo):
//   REQ  C1 7 B  [C1][07][11][KeyH][KeyL][0x78 AT_ATTACK1][Dir]
//   (macro SendRequestAttack :518-:527; AT_ATTACK1=120 enum_h :1497;
//    Key BE SEM máscara :524; spe.Send() defaults => C1 plain)
//   RESP C1 10 B = attack_ok do golden 1.3-M (damage_11_vectors.json —
//   LIDO da fonte primária, zero digitação): raw 0x8100 => key 0x0100,
//   Damage 0x0123, DT 0xC5, Shield 0x0044 => lastDamage=0x0123.
// Prova também o round-trip do helper de framing 0x000C (Build->Extract).
// TWO-TU: espelho ODR token-idêntico (NÃO EDITAR sem editar o core) +
// prototypes; link com mvp_login_client.cpp (-DNEWERA_MVP_NO_MAIN).
#include <cstdint>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

// ---- espelho ODR (cópia token-idêntica do core — não editar sem o core) ----
namespace newera { namespace mvp {
enum class EntityKind : uint8_t { Character = 0, Monster = 1 };

struct EntityRecord {
    uint16_t    key = 0;      // 0..0x7FFF (:2177/:2605)
    EntityKind  kind = EntityKind::Monster;
    uint8_t     x = 0, y = 0;
    uint8_t     targetX = 0, targetY = 0;   // provados nos dois pacotes
    uint8_t     dir = 0;                    // Path>>4 (:2271/:2637)
    int         angleDeg = 0;               // ((dir-1)*45)
    uint8_t     pk = 0;        // só Character (Path&0xF :2237)
    char        id[11] = {};   // só Character (ID[10]+NUL :2353-:2354)
    uint8_t     classByte = 0; // só Character (:2235; pose=&0x07 :2240)
    uint16_t    type = 0;      // só Monster (10 bits :2597)
    std::vector<uint8_t> buffs;             // s_BuffEffectState :2361/:2614
    // 1.3-M (damage RX 0x11): HP/Shield NÃO vêm no viewport (0x12/0x13 sem
    // Life) => opcionais, só populados por pacote futuro de stats; lastDamage
    // espelha c->Hit = Damage (:3188 — todos os caminhos do handler)
    std::optional<int> hp;      // ausente até stats (não recuperado no MVP)
    std::optional<int> shield;  // idem
    int lastDamage = -1;        // último dano recebido (c->Hit :3188)
};

struct WorldState {
    std::unordered_map<uint16_t, EntityRecord> entities;

    size_t countByKind(EntityKind k) const {   // counters derivados (sem cache)
        size_t n = 0;
        for (const auto& kv : entities) if (kv.second.kind == k) ++n;
        return n;
    }
    void clear() { entities.clear(); }
};

// ---- prototypes (link: mvp_login_client.cpp; SEM default args aqui) ----
bool BuildC1_AttackRequestWire(uint16_t targetKey, uint8_t dir,
                                std::vector<uint8_t>& out, std::string& err);
bool ApplyFrame_DamageRx_C1(const std::vector<uint8_t>& frame,
                            WorldState& ws, std::string& err,
                            size_t* missed);
bool ExtractClassicFromBothMessage_Asio(const std::vector<uint8_t>& frame,
                                        std::vector<uint8_t>& outClassic,
                                        std::string& err);
bool BuildAsio_BOTH_MESSAGE_FromClassicC1(const std::vector<uint8_t>& innerC1,
                                          std::vector<uint8_t>& out,
                                          std::string& err);
}} // namespace newera::mvp
// ------------------------------ fim do espelho ------------------------------

using namespace newera;

static bool ReadHexField(const char* path, const char* field, std::vector<uint8_t>& out) {
    std::ifstream f(path);
    if (!f) { printf("FALHA: nao abriu %s\n", path); return false; }
    std::string json((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
    size_t p = json.find(std::string("\"") + field + "\"");
    if (p == std::string::npos) return false;
    p = json.find(':', p);   if (p == std::string::npos) return false;
    p = json.find('"', p);   if (p == std::string::npos) return false;
    ++p;
    std::string hex;
    while (p < json.size() && json[p] != '"') { hex += json[p]; ++p; }
    if (hex.size() % 2 || hex.empty()) return false;
    out.clear();
    for (size_t i = 0; i < hex.size(); i += 2)
        out.push_back((uint8_t)strtoul(hex.substr(i, 2).c_str(), nullptr, 16));
    return true;
}

static std::string Hex(const std::vector<uint8_t>& v) {
    static const char* d = "0123456789abcdef";
    std::string s;
    for (uint8_t b : v) { s += d[b >> 4]; s += d[b & 0xF]; }
    return s;
}

int main() {
    // ---------- REQ: builder real (targetKey 0x0101, dir 3) ----------
    const uint16_t targetKey = 0x0101;               // monster A do frame13 golden
    const uint8_t  dir       = 3;
    std::vector<uint8_t> req; std::string err;
    if (!mvp::BuildC1_AttackRequestWire(targetKey, dir, req, err)) {
        printf("FALHA builder: %s\n", err.c_str()); return 1; }
    const uint8_t expect[7] = { 0xC1, 0x07, 0x11, 0x01, 0x01, 0x78, 0x03 };
    assert(req.size() == 7 && std::memcmp(req.data(), expect, 7) == 0);

    // ---------- RESP: attack_ok do golden 1.3-M (fonte primária) ----------
    std::vector<uint8_t> resp;
    if (!ReadHexField("NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_damage_11/damage_11_vectors.json",
                      "attack_ok_c1_hex", resp))
        return 1;
    assert(resp.size() == 10);

    // ---------- apply do RESP em ws com dummy 0x0100 (ciclo mínimo) ----------
    mvp::WorldState ws; ws.clear();
    mvp::EntityRecord dummy{};                       // dummy p/ key do damage
    dummy.key = 0x0100; dummy.kind = mvp::EntityKind::Character;
    ws.entities[0x0100] = dummy;
    size_t missed = 0;
    assert(mvp::ApplyFrame_DamageRx_C1(resp, ws, err, &missed));
    assert(missed == 0);
    assert(ws.entities.at(0x0100).lastDamage == 0x0123);

    // ---------- round-trip do helper de framing 0x000C (bonus) ----------
    std::vector<uint8_t> tunnel, back;
    assert(mvp::BuildAsio_BOTH_MESSAGE_FromClassicC1(req, tunnel, err));
    assert(tunnel.size() == 13 && tunnel[0] == 0x0C && tunnel[1] == 0x00);
    assert(mvp::ExtractClassicFromBothMessage_Asio(tunnel, back, err));
    assert(back == req);

    // ---------- saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_attack_tx_11/attack_tx_11_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_attack_tx_11/attack_tx_11_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"attack_tx_0x11_req_resp\",\n";
      j << "  \"req_c1_hex\": \"" << Hex(req) << "\",\n";
      j << "  \"resp_damage_c1_hex\": \"" << Hex(resp) << "\",\n";
      j << "  \"expected\": { \"req_targetKey\": 257, \"req_targetKey_hex\": \"0x0101\","
        << " \"req_dir\": 3, \"at_attack1\": 120, \"resp_key\": 256,"
        << " \"resp_key_hex\": \"0x0100\", \"lastDamage\": 291,"
        << " \"lastDamage_hex\": \"0x0123\", \"missed\": 0 },\n";
      j << "  \"fonte_resp\": \"test_vectors_damage_11/damage_11_vectors.json#attack_ok_c1_hex\"\n}\n"; }
    { std::ofstream m(mpath);
      m << "# ATTACK TX 0x11 (SendRequestAttack) + RESP damage — golden vectors (1.3-N)\n\n";
      m << "- req **C1 7 B**: `" << Hex(req) << "` (targetKey 0x0101, dir 3, AT_ATTACK1=0x78; builder real)\n";
      m << "- resp_damage **C1 10 B**: `" << Hex(resp) << "` (= attack_ok do golden 1.3-M; raw 0x8100 ⇒ key 0x0100, Damage 0x0123)\n";
      m << "- Ciclo mínimo provado no golden: apply resp em ws c/ dummy 0x0100 ⇒ lastDamage=0x0123 (missed=0)\n";
      m << "- Bonus: round-trip BuildAsio_BOTH_MESSAGE_FromClassicC1 -> Extract (13 B; uso C->S NÃO evidenciado)\n";
      m << "- Wire TX: macro :518-:527 · AT_ATTACK1=120 :1497 · Key BE sem máscara :524 · C1 plain (Send defaults)\n"; }

    printf("GEN ATTACK_TX_11 OK: req=%s (7 B exatos); resp aplicado => 0x0100 lastDamage=0x0123; roundtrip 0x000C OK\n",
           Hex(req).c_str());
    return 0;
}
