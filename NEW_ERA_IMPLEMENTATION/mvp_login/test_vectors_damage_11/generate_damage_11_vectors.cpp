// NEW-ERA — 1.3-M — generate_damage_11_vectors.cpp (PACKET_ATTACK 0x11 + túnel BOTH_MESSAGE 0x000C)
// Golden determinístico do 1º "combat signal" S->C:
//   C1 10 B fixo [C1][0A][11][KeyH][KeyL][DamH][DamL][DT][SH][SL]
//   (PRECEIVE_ATTACK :674-:685; ReceiveAttackDamage :2984-:3191;
//    dispatch case PACKET_ATTACK :13143-:13144; PACKET_ATTACK=0x11 :26)
//   + transporte moderno: olc 0x000C BOTH_MESSAGE :99-:137 = TÚNEL de pacote
//   clássico cru -> TranslateProtocol :135 (aqui: inner 0x11 somente).
//   attack_ok   10 B: c10a11 8100 0123 c5 0044 (raw 0x8100: Success=1, key
//   0x0100; Damage 0x0123; DT 0xC5 => type=5/double/combo; Shield 0x0044)
//   attack_miss 10 B: key 0x3333 inexistente; attack_trunc 9 B (sem SL);
//   both_ok 16 B = 0c00 0a000000 + attack_ok; both_trunc 15 B (body 9 B).
// TWO-TU: espelho ODR token-idêntico (NÃO EDITAR sem editar o core) +
// prototypes; link com mvp_login_client.cpp (-DNEWERA_MVP_NO_MAIN).
#include <cassert>
#include <cstdint>
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

struct DamageEvent {
    uint16_t key = 0;           // pós-máscara &0x7FFF (:2997)
    bool     success = false;   // b15 do raw (:2996)
    uint16_t damage = 0;        // WORD BE (:3002)
    uint16_t shieldDamage = 0;  // WORD BE (:3013)
    uint8_t  damageTypeRaw = 0; // byte cru (:3009)
    int      type = 0;          // &0x3F (:3009)
    bool     doubleEnable = false; // >>6 &1 (:3011)
    bool     comboEnable = false;  // >>7 &1 (:3012)
};

// ---- prototypes (link: mvp_login_client.cpp; SEM default args aqui) ----
bool ApplyFrame_C2_12_Characters(const std::vector<uint8_t>& frame,
                                 WorldState& ws, std::string& err);
bool ApplyFrame_C2_13_Monsters(const std::vector<uint8_t>& frame,
                               WorldState& ws, std::string& err);
bool ParseDamageRxPlain_C1(const std::vector<uint8_t>& frame,
                           DamageEvent& out, std::string& err);
bool ApplyFrame_DamageRx_C1(const std::vector<uint8_t>& frame,
                            WorldState& ws, std::string& err,
                            size_t* missed);
bool ExtractClassicFromBothMessage_Asio(const std::vector<uint8_t>& frame,
                                        std::vector<uint8_t>& outClassic,
                                        std::string& err);
bool ApplyFrame_BOTH_MESSAGE_Tunnel_DamageOnly(const std::vector<uint8_t>& frame,
                                               WorldState& ws, std::string& err,
                                               size_t* missed);
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

// comparação CAMPO-A-CAMPO (inclui hp/shield/lastDamage do 1.3-M)
static bool SameEntity(const mvp::EntityRecord& a, const mvp::EntityRecord& b) {
    if (a.key != b.key || a.kind != b.kind) return false;
    if (a.x != b.x || a.y != b.y) return false;
    if (a.targetX != b.targetX || a.targetY != b.targetY) return false;
    if (a.dir != b.dir || a.angleDeg != b.angleDeg) return false;
    if (a.pk != b.pk || a.classByte != b.classByte || a.type != b.type) return false;
    if (std::memcmp(a.id, b.id, sizeof(a.id)) != 0) return false;
    if (a.buffs != b.buffs) return false;
    if (a.hp != b.hp || a.shield != b.shield) return false;
    return a.lastDamage == b.lastDamage;
}
static bool SameWorld(const mvp::WorldState& A, const mvp::WorldState& B) {
    if (A.entities.size() != B.entities.size()) return false;
    for (const auto& kv : A.entities) {
        auto it = B.entities.find(kv.first);
        if (it == B.entities.end() || !SameEntity(kv.second, it->second)) return false;
    }
    return true;
}

int main() {
    // ---------- seed (fonte: goldens versionados) ----------
    std::vector<uint8_t> frame12, frame13;
    if (!ReadHexField("NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_12/viewport_12_vectors.json",
                      "resp_c2_hex", frame12) ||
        !ReadHexField("NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_13_buffs/viewport_13_buffs_vectors.json",
                      "resp_c2_hex", frame13))
        return 1;
    assert(frame12.size() == 79 && frame13.size() == 29);

    // ---------- frames damage 0x11 + túnel 0x000C ----------
    std::vector<uint8_t> atkOk  = { 0xC1, 0x0A, 0x11, 0x81, 0x00, 0x01, 0x23, 0xC5, 0x00, 0x44 };
    std::vector<uint8_t> atkMis = { 0xC1, 0x0A, 0x11, 0x33, 0x33, 0x00, 0x10, 0x00, 0x00, 0x00 };
    std::vector<uint8_t> atkTrc(atkOk.begin(), atkOk.begin() + 9);   // sem ShieldL
    std::vector<uint8_t> bothOk;                                     // 16 B
    {   const uint16_t id = 0x000C; const uint32_t sz = 10;
        const uint8_t h[6] = { 0x0C, 0x00, 0x0A, 0x00, 0x00, 0x00 };
        bothOk.assign(h, h + 6); bothOk.insert(bothOk.end(), atkOk.begin(), atkOk.end());
        (void)id; (void)sz; }
    std::vector<uint8_t> bothTrc;                                    // 15 B (body 9)
    {   const uint8_t h[6] = { 0x0C, 0x00, 0x0A, 0x00, 0x00, 0x00 };
        bothTrc.assign(h, h + 6); bothTrc.insert(bothTrc.end(), atkTrc.begin(), atkTrc.end()); }
    assert(atkOk.size() == 10 && atkMis.size() == 10 && atkTrc.size() == 9 &&
           bothOk.size() == 16 && bothTrc.size() == 15);

    // ---------- prova de DECODE (flags/bits do 0x11) ----------
    mvp::DamageEvent ev; std::string err;
    assert(mvp::ParseDamageRxPlain_C1(atkOk, ev, err));
    assert(ev.success && ev.key == 0x0100);           // raw 0x8100 -> b15 + &0x7FFF
    assert(ev.damage == 0x0123 && ev.shieldDamage == 0x0044);
    assert(ev.damageTypeRaw == 0xC5 && ev.type == 0x05 && ev.doubleEnable && ev.comboEnable);
    std::vector<uint8_t> classic;
    assert(mvp::ExtractClassicFromBothMessage_Asio(bothOk, classic, err));
    assert(classic == atkOk);                          // túnel devolve o C1 cru

    // ---------- ciclo: seed + damage clássico + túnel ----------
    mvp::WorldState ws; ws.clear();
    assert(mvp::ApplyFrame_C2_12_Characters(frame12, ws, err));
    assert(mvp::ApplyFrame_C2_13_Monsters(frame13, ws, err));
    assert(ws.entities.size() == 4);

    size_t missed = 0;
    assert(mvp::ApplyFrame_DamageRx_C1(atkOk, ws, err, &missed));
    assert(missed == 0);
    assert(ws.entities.at(0x0100).lastDamage == 0x0123);          // c->Hit :3188

    assert(mvp::ApplyFrame_DamageRx_C1(atkMis, ws, err, &missed));
    assert(missed == 1 && ws.entities.size() == 4);

    const mvp::WorldState snap = ws;
    assert(!mvp::ApplyFrame_DamageRx_C1(atkTrc, ws, err, &missed));
    assert(err.rfind("0x11:", 0) == 0);
    assert(missed == 1 && SameWorld(ws, snap));                   // trunc sem efeito

    // túnel: probe distingue apply real (lastDamage resetado antes)
    ws.entities.at(0x0100).lastDamage = -1;                       // PROBE de teste
    size_t missed2 = 0;
    assert(mvp::ApplyFrame_BOTH_MESSAGE_Tunnel_DamageOnly(bothOk, ws, err, &missed2));
    assert(missed2 == 0);
    assert(ws.entities.at(0x0100).lastDamage == 0x0123);          // túnel APLICOU

    const mvp::WorldState snap2 = ws;
    assert(!mvp::ApplyFrame_BOTH_MESSAGE_Tunnel_DamageOnly(bothTrc, ws, err, &missed2));
    assert(err.rfind("0x000C:", 0) == 0);
    assert(missed2 == 0 && SameWorld(ws, snap2));                 // trunc sem efeito

    // ---------- saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_damage_11/damage_11_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_damage_11/damage_11_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"damage_rx_0x11_tunnel_0x000c\",\n";
      j << "  \"frame12_c2_hex\": \"" << Hex(frame12) << "\",\n";
      j << "  \"frame13_c2_hex\": \"" << Hex(frame13) << "\",\n";
      j << "  \"attack_ok_c1_hex\": \"" << Hex(atkOk) << "\",\n";
      j << "  \"attack_miss_c1_hex\": \"" << Hex(atkMis) << "\",\n";
      j << "  \"attack_trunc_c1_hex\": \"" << Hex(atkTrc) << "\",\n";
      j << "  \"both_ok_hex\": \"" << Hex(bothOk) << "\",\n";
      j << "  \"both_trunc_hex\": \"" << Hex(bothTrc) << "\",\n";
      j << "  \"expected\": { \"key\": 256, \"key_hex\": \"0x0100\", \"lastDamage\": 291,"
        << " \"lastDamage_hex\": \"0x0123\", \"success\": true, \"damageType\": 5,"
        << " \"doubleEnable\": true, \"comboEnable\": true, \"shieldDamage\": 68,"
        << " \"missed_classic\": 1, \"missed_both\": 0, \"ws_size\": 4 },\n";
      j << "  \"trunc\": { \"applied\": false, \"error\": \"" << err << "\" } \n}\n"; }
    { std::ofstream m(mpath);
      m << "# DAMAGE RX 0x11 (PACKET_ATTACK) + túnel BOTH_MESSAGE 0x000C — golden vectors (1.3-M)\n\n";
      m << "- attack_ok **C1 10 B**: `" << Hex(atkOk) << "` (raw 0x8100 ⇒ Success=1, key 0x0100; Damage 0x0123; DT 0xC5 ⇒ type=5/double/combo; Shield 0x0044)\n";
      m << "- attack_miss **C1 10 B**: `" << Hex(atkMis) << "` (key 0x3333 inexistente → missed=1)\n";
      m << "- attack_trunc **9 B**: `" << Hex(atkTrc) << "` (sem ShieldL — DEVE falhar)\n";
      m << "- both_ok **olc 16 B**: `" << Hex(bothOk) << "` (0x000C + inner attack_ok — túnel)\n";
      m << "- both_trunc **15 B**: `" << Hex(bothTrc) << "` (size diz 10, body 9 — DEVE falhar)\n";
      m << "- Apply: lastDamage=0x0123 (c->Hit :3188); miss contabilizada; trunc não altera estado\n";
      m << "- Wire: PACKET_ATTACK=0x11 :26 · PRECEIVE_ATTACK :674-:685 · dispatch :13143-:13144 · túnel :99-:137\n"; }

    printf("GEN DAMAGE_11 OK: 0x0100 lastDamage=0x0123 (success/type=5/dbl/combo provados no decode); "
           "missed=%zu (classic) / %zu (tunnel); trunc rejeitado ('%s')\n",
           missed, missed2, err.c_str());
    return 0;
}
