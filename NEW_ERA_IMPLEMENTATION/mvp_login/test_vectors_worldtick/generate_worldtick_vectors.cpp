// NEW-ERA — 1.3-H2A — generate_worldtick_vectors.cpp (WORLD TICK 0x12+0x13+trunc)
// Golden determinístico do agregador WorldState (1.3-H):
//   frame12       = C2 79 B  (golden 1.3-G: 2 chars, strides 36/38, buffs 0/2)
//   frame13       = C2 29 B  (golden 1.3-F: 2 monsters, strides 11/13, buffs 1/3)
//   frame13_trunc = 28 B     (frame13 menos 1 byte — DEVE falhar e NÃO alterar ws)
// FONTE PRIMÁRIA dos frames: os jsons versionados (test_vectors_viewport_12 /
// test_vectors_viewport_13_buffs) — campo resp_c2_hex extraído em runtime.
// BUILD TWO-TU (por comando): este TU linka com ../mvp_login_client.cpp.
//   ==> O espelho abaixo (EntityKind/EntityRecord/WorldState) DEVE permanecer
//   token-idêntico ao bloco 1.3-H do core (ODR); Apply* são prototypes only.
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

// ---- ESPELHO ODR do bloco 1.3-H (não editar sem editar o core) ----
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

bool ApplyFrame_C2_12_Characters(const std::vector<uint8_t>& frame,
                                 WorldState& ws, std::string& err);
bool ApplyFrame_C2_13_Monsters(const std::vector<uint8_t>& frame,
                               WorldState& ws, std::string& err);

} } // namespace newera::mvp

using namespace newera;

// ---- extração de campo hex de um json versionado (sem digitação) ----
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

static bool SameState(const mvp::WorldState& a, const mvp::WorldState& b) {
    if (a.entities.size() != b.entities.size()) return false;
    for (const auto& kv : a.entities) {
        auto it = b.entities.find(kv.first);
        if (it == b.entities.end()) return false;
        const mvp::EntityRecord& r1 = kv.second, r2 = it->second;
        if (r1.kind != r2.kind || r1.x != r2.x || r1.y != r2.y ||
            r1.targetX != r2.targetX || r1.targetY != r2.targetY ||
            r1.dir != r2.dir || r1.angleDeg != r2.angleDeg || r1.pk != r2.pk ||
            r1.classByte != r2.classByte || r1.type != r2.type ||
            std::strcmp(r1.id, r2.id) != 0 || r1.buffs != r2.buffs) return false;
    }
    return true;
}

int main() {
    // ---------- frames (fonte: goldens versionados) ----------
    std::vector<uint8_t> frame12, frame13;
    if (!ReadHexField("NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_12/viewport_12_vectors.json",
                      "resp_c2_hex", frame12) ||
        !ReadHexField("NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_13_buffs/viewport_13_buffs_vectors.json",
                      "resp_c2_hex", frame13))
        return 1;
    assert(frame12.size() == 79 && frame12[0] == 0xC2 && frame12[3] == 0x12);
    assert(frame13.size() == 29 && frame13[0] == 0xC2 && frame13[3] == 0x13);
    std::vector<uint8_t> frame13_trunc(frame13.begin(), frame13.end() - 1);  // 28 B
    assert(frame13_trunc.size() == 28);

    // ---------- tick: 0x12 + 0x13 ----------
    mvp::WorldState ws; std::string err;
    if (!mvp::ApplyFrame_C2_12_Characters(frame12, ws, err)) { printf("FALHA 0x12: %s\n", err.c_str()); return 1; }
    if (!mvp::ApplyFrame_C2_13_Monsters(frame13, ws, err))   { printf("FALHA 0x13: %s\n", err.c_str()); return 1; }

    assert(ws.entities.size() == 4);
    assert(ws.countByKind(mvp::EntityKind::Character) == 2);
    assert(ws.countByKind(mvp::EntityKind::Monster) == 2);
    // chars (golden 1.3-G)
    assert(ws.entities.count(0x0100) && ws.entities.count(0x0200));
    assert(std::string(ws.entities[0x0100].id) == "HeroA" && ws.entities[0x0100].pk == 3);
    assert(ws.entities[0x0200].buffs.size() == 2);
    // monsters (golden 1.3-F)
    assert(ws.entities.count(0x0101) && ws.entities.count(0x0202));
    assert(ws.entities[0x0101].type == 0x0011 && ws.entities[0x0101].buffs.size() == 1);
    assert(ws.entities[0x0202].type == 0x0022 && ws.entities[0x0202].buffs.size() == 3);

    // ---------- trunc: DEVE falhar e NÃO alterar estado ----------
    const mvp::WorldState before = ws;                    // snapshot profundo
    std::string terr;
    const bool applied = mvp::ApplyFrame_C2_13_Monsters(frame13_trunc, ws, terr);
    assert(!applied);
    assert(terr.rfind("0x13:", 0) == 0);
    assert(SameState(before, ws));                        // estado intocado

    // ---------- saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_worldtick/worldtick_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_worldtick/worldtick_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"worldtick_c2_12_13_trunc\",\n";
      j << "  \"frame12_c2_hex\": \"" << Hex(frame12) << "\",\n";
      j << "  \"frame13_c2_hex\": \"" << Hex(frame13) << "\",\n";
      j << "  \"frame13_trunc_hex\": \"" << Hex(frame13_trunc) << "\",\n";
      j << "  \"expected\": { \"entities\": 4, \"characters\": 2, \"monsters\": 2,\n";
      j << "    \"keys\": [ 256, 512, 257, 514 ],\n";
      j << "    \"chars\": [ { \"key\": 256, \"id\": \"HeroA\", \"pk\": 3, \"dir\": 1 },"
        << " { \"key\": 512, \"id\": \"HeroB\", \"pk\": 0, \"dir\": 2, \"buffs_len\": 2 } ],\n";
      j << "    \"monsters\": [ { \"key\": 257, \"type\": 17, \"buffs_len\": 1 },"
        << " { \"key\": 514, \"type\": 34, \"buffs_len\": 3 } ],\n";
      j << "    \"trunc\": { \"applied\": false, \"error\": \"" << terr << "\","
        << " \"state_unchanged\": true } }\n}\n"; }
    { std::ofstream m(mpath);
      m << "# WorldTick — golden vectors (1.3-H2A: 0x12 + 0x13 + trunc)\n\n";
      m << "- frame12 **C2 79 B** (golden 1.3-G; chars strides 36/38): `" << Hex(frame12) << "`\n";
      m << "- frame13 **C2 29 B** (golden 1.3-F; monsters strides 11/13): `" << Hex(frame13) << "`\n";
      m << "- frame13_trunc **28 B**: Apply DEVE falhar (`" << terr << "`) e NÃO alterar ws\n";
      m << "- expected pós-tick: 4 entidades (2 chars 0x0100/0x0200 + 2 monsters 0x0101/0x0202)\n";
      m << "- Fonte dos frames: jsons versionados viewport_12/viewport_13_buffs (resp_c2_hex).\n"; }

    printf("GEN WORLDTICK OK: tick 0x12+0x13 -> %zu entidades (%zu chars, %zu monsters); "
           "trunc rejeitado ('%s'), estado intocado\n",
           ws.entities.size(),
           ws.countByKind(mvp::EntityKind::Character),
           ws.countByKind(mvp::EntityKind::Monster), terr.c_str());
    return 0;
}
