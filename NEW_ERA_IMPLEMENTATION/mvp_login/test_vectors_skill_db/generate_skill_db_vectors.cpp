// NEW-ERA — 1.3-O — generate_skill_db_vectors.cpp (TX skill C1 0xDB)
// Golden determinístico do TX de skill (magic attack) C->S:
//   C1 0xDB size=9+3*Count: [TypeH][TypeL][x][y][Serial][Count]
//   + por alvo [KeyH][KeyL][SkillSerial]  (macro :600-:616; Send(TRUE))
// Golden: Type=0x0009, (x,y)=(50,60), serial=0x01 (teste — def [NOT
// RECOVERED]), Count=1, key 0x0101, skillSerial=0x02 (teste) => 12 B:
//   c1 0c db 00 09 32 3c 01 01 01 01 02
// RESP = damage 0x11 do golden 1.3-N (attack_tx_11_vectors.json — cadeia de
// fontes primárias) => lastDamage=0x0123 em dummy 0x0100.
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
bool BuildC1_SkillRequestWire(uint16_t skillType, uint8_t x, uint8_t y,
                              uint8_t serial, uint8_t count,
                              const uint16_t* targetKeys, uint8_t skillSerial,
                              std::vector<uint8_t>& out, std::string& err);
bool ApplyFrame_DamageRx_C1(const std::vector<uint8_t>& frame,
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

int main() {
    // ---------- REQ: builder real (Type 0x0009, (50,60), 1 alvo 0x0101) ----------
    const uint16_t skillType = 0x0009;
    const uint8_t  x = 50, y = 60, serial = 0x01, count = 1, skillSerial = 0x02;
    const uint16_t keys[1] = { 0x0101 };
    std::vector<uint8_t> req; std::string err;
    if (!mvp::BuildC1_SkillRequestWire(skillType, x, y, serial, count, keys,
                                       skillSerial, req, err)) {
        printf("FALHA builder: %s\n", err.c_str()); return 1; }
    const uint8_t expect[12] = { 0xC1, 0x0C, 0xDB, 0x00, 0x09, 0x32,
                                 0x3C, 0x01, 0x01, 0x01, 0x01, 0x02 };
    assert(req.size() == 12 && std::memcmp(req.data(), expect, 12) == 0);

    // multi-alvo sanidade (Count=2 => 15 B; NÃO vai ao golden — só prova do loop)
    {   const uint16_t k2[2] = { 0x0101, 0x0202 };
        std::vector<uint8_t> r2;
        assert(mvp::BuildC1_SkillRequestWire(skillType, x, y, serial, 2, k2,
                                             skillSerial, r2, err));
        assert(r2.size() == 15 && r2[1] == 15 && r2[12] == 0x02 && r2[13] == 0x02
               && r2[14] == skillSerial); }
    // guard: count 0 => erro
    {   std::vector<uint8_t> r0;
        assert(!mvp::BuildC1_SkillRequestWire(skillType, x, y, serial, 0, keys,
                                              skillSerial, r0, err)); }

    // ---------- RESP: damage 0x11 do golden 1.3-N (cadeia de fontes) ----------
    std::vector<uint8_t> resp;
    if (!ReadHexField("NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_attack_tx_11/attack_tx_11_vectors.json",
                      "resp_damage_c1_hex", resp))
        return 1;
    assert(resp.size() == 10);

    // ---------- apply do RESP em ws com dummy 0x0100 ----------
    mvp::WorldState ws; ws.clear();
    mvp::EntityRecord dummy{};
    dummy.key = 0x0100; dummy.kind = mvp::EntityKind::Character;
    ws.entities[0x0100] = dummy;
    size_t missed = 0;
    assert(mvp::ApplyFrame_DamageRx_C1(resp, ws, err, &missed));
    assert(missed == 0 && ws.entities.at(0x0100).lastDamage == 0x0123);

    // ---------- saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_skill_db/skill_db_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_skill_db/skill_db_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"skill_tx_0xdb_req_resp\",\n";
      j << "  \"req_c1_hex\": \"" << Hex(req) << "\",\n";
      j << "  \"resp_damage_c1_hex\": \"" << Hex(resp) << "\",\n";
      j << "  \"expected\": { \"skillType\": 9, \"skillType_hex\": \"0x0009\","
        << " \"x\": 50, \"y\": 60, \"serial\": 1, \"count\": 1,"
        << " \"targetKey\": 257, \"targetKey_hex\": \"0x0101\","
        << " \"skillSerial\": 2, \"req_size\": 12, \"resp_key\": 256,"
        << " \"resp_key_hex\": \"0x0100\", \"lastDamage\": 291,"
        << " \"lastDamage_hex\": \"0x0123\", \"missed\": 0 },\n";
      j << "  \"notas\": { \"serial_eh_valor_de_teste\": \"MakeSkillSerialNumber [NOT RECOVERED]\","
        << " \"send_encrypt\": \"spe.Send(TRUE) — wrap C3 [NOT RECOVERED]; builder = C1 plain pre-encrypt\" },\n";
      j << "  \"fonte_resp\": \"test_vectors_attack_tx_11/attack_tx_11_vectors.json#resp_damage_c1_hex\"\n}\n"; }
    { std::ofstream m(mpath);
      m << "# SKILL TX 0xDB (SendRequestMagicAttack) + RESP damage — golden vectors (1.3-O)\n\n";
      m << "- req **C1 12 B**: `" << Hex(req) << "` (Type 0x0009, (x,y)=(50,60), serial 0x01, Count=1, key 0x0101, skillSerial 0x02)\n";
      m << "- resp_damage **C1 10 B**: `" << Hex(resp) << "` (= golden 1.3-N/1.3-M; key 0x0100, Damage 0x0123)\n";
      m << "- Layout: [C1][size=9+3·Count][DB][Type BE][x][y][Serial][Count] + por alvo [Key BE][SkillSerial]\n";
      m << "- spe.Send(**TRUE**) pede encrypt — builder = C1 plain pré-encrypt (wrap C3 [NOT RECOVERED])\n";
      m << "- GS: case 0x19 → CGSkillAttackRecv (:121-:122); tradução 0xDB→0x19 [NOT RECOVERED]\n";
      m << "- Provas extra no gerador: Count=2 ⇒ 15 B (loop) e guard count=0 rejeitado\n"; }

    printf("GEN SKILL_DB OK: req=%s (12 B exatos); Count=2 => 15 B; guard count=0 OK; resp => lastDamage=0x0123\n",
           Hex(req).c_str());
    return 0;
}
