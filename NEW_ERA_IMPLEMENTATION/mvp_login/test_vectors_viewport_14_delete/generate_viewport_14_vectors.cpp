// NEW-ERA — 1.3-I — generate_viewport_14_vectors.cpp (CICLO SPAWN->DELETE)
// Golden determinístico do ciclo: 0x12 (spawn chars) + 0x13 (spawn monsters)
// + 0x14 (DELETE C1) — fechando spawn/delete no WorldState.
//   frame12/13 = lidos dos JSONS VERSIONADOS (fonte primária, zero digitação):
//     viewport_12_vectors.json        (C2 79 B — 2 chars)
//     viewport_13_buffs_vectors.json  (C2 29 B — 2 monsters)
//   del14 (válido, C1 10 B): [C1][0A][14][count=3][keys BE]
//     0x0100 (char A) · 0x0101 (monster A) · raw 0x9999 (b15=1 -> máscara
//     &0x7FFF :2817 => key 0x1999 — INEXISTENTE, deve ser ignorada).
//   del14_trunc (9 B): header diz size=0x0A mas faltam bytes => DEVE falhar
//     e NÃO alterar o estado (snapshot).
// Wire 0x14: PHEADER_DEFAULT (WSclient.h :113-:118) + PDELETE_CHARACTER
// (:622-:626) 2 B BE; stride FIXO 2 (:2846); DeleteCharacter tolerante :2839.
#include "mvp_login_client.cpp"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

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
            r1.dir != r2.dir || r1.pk != r2.pk || r1.type != r2.type ||
            std::strcmp(r1.id, r2.id) != 0 || r1.buffs != r2.buffs) return false;
    }
    return true;
}

int main() {
    // ---------- frames spawn (fonte: goldens versionados) ----------
    std::vector<uint8_t> frame12, frame13;
    if (!ReadHexField("NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_12/viewport_12_vectors.json",
                      "resp_c2_hex", frame12) ||
        !ReadHexField("NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_13_buffs/viewport_13_buffs_vectors.json",
                      "resp_c2_hex", frame13))
        return 1;
    assert(frame12.size() == 79 && frame12[3] == 0x12);
    assert(frame13.size() == 29 && frame13[3] == 0x13);

    // ---------- delete 0x14 (válido): [C1][0A][14][03] + keys BE ----------
    std::vector<uint8_t> del14;
    del14.push_back(0xC1); del14.push_back(0x0A); del14.push_back(0x14);
    del14.push_back(3);                                   // count = 3
    del14.push_back(0x01); del14.push_back(0x00);         // 0x0100 (char A)
    del14.push_back(0x01); del14.push_back(0x01);         // 0x0101 (monster A)
    del14.push_back(0x99); del14.push_back(0x99);         // raw: b15=1 -> 0x1999 (inexistente)
    assert(del14.size() == 10 && del14[1] == 10);

    // ---------- delete 0x14 (truncado): 9 B, header diz 0x0A ----------
    std::vector<uint8_t> del14t(del14.begin(), del14.end() - 1);   // falta 1 byte
    assert(del14t.size() == 9 && del14t[1] == 0x0A);

    // ---------- ciclo spawn -> delete ----------
    mvp::WorldState ws; ws.clear(); std::string err;
    assert(mvp::ApplyFrame_C2_12_Characters(frame12, ws, err));
    assert(mvp::ApplyFrame_C2_13_Monsters(frame13, ws, err));
    assert(ws.entities.size() == 4);

    assert(mvp::ApplyFrame_DeleteEntities_C1(del14, ws, err));      // remove 2, ignora 0x1999
    assert(ws.entities.size() == 2);
    assert(!ws.entities.count(0x0100) && !ws.entities.count(0x0101));
    assert(ws.entities.count(0x0200) && ws.entities.count(0x0202));
    assert(ws.countByKind(mvp::EntityKind::Character) == 1);
    assert(ws.countByKind(mvp::EntityKind::Monster) == 1);

    // ---------- trunc: falha e NÃO altera ----------
    const mvp::WorldState before = ws;
    std::string terr;
    assert(!mvp::ApplyFrame_DeleteEntities_C1(del14t, ws, terr));
    assert(terr.rfind("0x14:", 0) == 0);
    assert(SameState(before, ws));

    // ---------- saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_14_delete/viewport_14_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_14_delete/viewport_14_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"viewport_cycle_spawn_delete_0x14\",\n";
      j << "  \"frame12_c2_hex\": \"" << Hex(frame12) << "\",\n";
      j << "  \"frame13_c2_hex\": \"" << Hex(frame13) << "\",\n";
      j << "  \"delete14_hex\": \"" << Hex(del14) << "\",\n";
      j << "  \"delete14_trunc_hex\": \"" << Hex(del14t) << "\",\n";
      j << "  \"expected_final_keys\": [ 512, 514 ],\n";
      j << "  \"expected_counts\": { \"characters\": 1, \"monsters\": 1 },\n";
      j << "  \"expected_removed\": [ 256, 257 ],\n";
      j << "  \"ignored_key_raw\": \"0x9999->0x1999 (b15 mask :2817; inexistente)\",\n";
      j << "  \"trunc\": { \"applied\": false, \"error\": \"" << terr << "\", \"state_unchanged\": true }\n}\n"; }
    { std::ofstream m(mpath);
      m << "# Viewport DELETE 0x14 — golden vectors (1.3-I: ciclo spawn→delete)\n\n";
      m << "- delete14 **C1 10 B**: `" << Hex(del14) << "` (count=3; keys BE 0x0100/0x0101/0x9999)\n";
      m << "- delete14_trunc **C1 9 B**: `" << Hex(del14t) << "` (size diz 0x0A — DEVE falhar)\n";
      m << "- Ciclo: 0x12+0x13 → ws=4; delete → ws=2 (ficam 0x0200/0x0202; counts 1/1)\n";
      m << "- raw 0x9999: b15=1 → máscara &0x7FFF (:2817) ⇒ key 0x1999 — inexistente, ignorada\n";
      m << "- Wire: PHEADER_DEFAULT :113-:118 + PDELETE_CHARACTER :622-:626 (BE, stride 2 :2846)\n"; }

    printf("GEN VIEWPORT_14 OK: ciclo 4->2 (removidas 0x0100/0x0101; 0x9999->0x1999 ignorada); "
           "trunc rejeitado ('%s'), estado intocado\n", terr.c_str());
    return 0;
}
