// NEW-ERA — 1.3-J — generate_move_15_vectors.cpp (CICLO SPAWN->MOVE->DELETE)
// Golden determinístico do 1º update de movimento no WorldState:
//   MOVE_OK    C1 7 B [C1][07][15][0100][4D][58]  — key existente 0x0100 -> (77,88)
//   MOVE_MISS  C1 7 B [C1][07][15][3333][09][09]  — key inexistente -> ignored+missed
//   MOVE_TRUNC 6 B (MOVE_OK sem Y)                — DEVE falhar, estado intocado
// Sequência provada: 0x12+0x13 (ws=4) -> move_ok (x/y/target=77/88, missed=0)
//   -> move_miss (missed=1, ws segue 4) -> move_trunc (false, snapshot intacto)
//   -> delete14 (C1 10 B do golden 1.3-I) -> ws=2.
// Frames 0x12/0x13/delete14 lidos dos JSONS VERSIONADOS (fonte primária).
// Wire 0x15: PRECEIVE_MOVE_POSITION (WSclient.h :892-:898) — Key BE SEM
// máscara :1749; PositionX/Y :1762-:1763 E TargetX/Y :1764-:1765.
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
    // ---------- frames (fonte: goldens versionados) ----------
    std::vector<uint8_t> frame12, frame13, delete14;
    if (!ReadHexField("NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_12/viewport_12_vectors.json",
                      "resp_c2_hex", frame12) ||
        !ReadHexField("NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_13_buffs/viewport_13_buffs_vectors.json",
                      "resp_c2_hex", frame13) ||
        !ReadHexField("NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_14_delete/viewport_14_vectors.json",
                      "delete14_hex", delete14))
        return 1;
    assert(frame12.size() == 79 && frame13.size() == 29 && delete14.size() == 10);

    // ---------- frames de movimento 0x15 ----------
    std::vector<uint8_t> moveOk  = { 0xC1, 0x07, 0x15, 0x01, 0x00, 77, 88 };  // key 0x0100
    std::vector<uint8_t> moveMis = { 0xC1, 0x07, 0x15, 0x33, 0x33,  9,  9 };  // key 0x3333 (inexistente)
    std::vector<uint8_t> moveTrc(moveOk.begin(), moveOk.end() - 1);           // 6 B (sem Y)
    assert(moveOk.size() == 7 && moveMis.size() == 7 && moveTrc.size() == 6);

    // ---------- ciclo spawn -> move -> delete ----------
    mvp::WorldState ws; ws.clear(); std::string err;
    assert(mvp::ApplyFrame_C2_12_Characters(frame12, ws, err));
    assert(mvp::ApplyFrame_C2_13_Monsters(frame13, ws, err));
    assert(ws.entities.size() == 4);
    const mvp::WorldState wsBeforeMove = ws;

    size_t missed = 0;
    assert(mvp::ApplyFrame_PositionUpdate_C1(moveOk, ws, err, &missed));
    assert(missed == 0);
    assert(ws.entities.at(0x0100).x == 77 && ws.entities.at(0x0100).y == 88);
    assert(ws.entities.at(0x0100).targetX == 77 && ws.entities.at(0x0100).targetY == 88);
    assert(ws.entities.size() == 4);                       // move não cria entries

    assert(mvp::ApplyFrame_PositionUpdate_C1(moveMis, ws, err, &missed));
    assert(missed == 1);
    assert(ws.entities.size() == 4 && !ws.entities.count(0x3333));

    const mvp::WorldState wsAfterMoves = ws;               // snapshot
    assert(!mvp::ApplyFrame_PositionUpdate_C1(moveTrc, ws, err, &missed));
    assert(err.rfind("0x15:", 0) == 0);
    assert(missed == 1);                                   // trunc não contabiliza miss
    assert(ws.entities.size() == wsAfterMoves.entities.size());
    assert(ws.entities.at(0x0100).x == 77 && ws.entities.at(0x0100).y == 88);
    for (const auto& kv : wsAfterMoves.entities)
        assert(ws.entities.count(kv.first));

    assert(mvp::ApplyFrame_DeleteEntities_C1(delete14, ws, err));
    assert(ws.entities.size() == 2);

    // ---------- saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_move_15/move_15_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_move_15/move_15_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"position_update_c1_0x15_cycle\",\n";
      j << "  \"frame12_c2_hex\": \"" << Hex(frame12) << "\",\n";
      j << "  \"frame13_c2_hex\": \"" << Hex(frame13) << "\",\n";
      j << "  \"move_ok_hex\": \"" << Hex(moveOk) << "\",\n";
      j << "  \"move_miss_hex\": \"" << Hex(moveMis) << "\",\n";
      j << "  \"move_trunc_hex\": \"" << Hex(moveTrc) << "\",\n";
      j << "  \"delete14_hex\": \"" << Hex(delete14) << "\",\n";
      j << "  \"expected\": { \"key_moved\": 256, \"new_x\": 77, \"new_y\": 88,"
        << " \"missed_final\": 1, \"ws_final_size\": 2 },\n";
      j << "  \"trunc\": { \"applied\": false, \"error\": \"" << err << "\","
        << " \"note\": \"estado intocado (snapshot)\" }\n}\n"; }
    { std::ofstream m(mpath);
      m << "# Position update 0x15 — golden vectors (1.3-J: spawn→move→delete)\n\n";
      m << "- move_ok **C1 7 B**: `" << Hex(moveOk) << "` (key 0x0100 → 77,88; target=idem :1764)\n";
      m << "- move_miss **C1 7 B**: `" << Hex(moveMis) << "` (key 0x3333 inexistente → ignored, missed=1)\n";
      m << "- move_trunc **6 B**: `" << Hex(moveTrc) << "` (sem Y — DEVE falhar, ws intocado)\n";
      m << "- Ciclo: ws=4 → move(77/88) → miss → trunc falho → delete → **ws=2**\n";
      m << "- Wire: PRECEIVE_MOVE_POSITION :892-:898 (Key BE sem máscara :1749)\n"; }

    printf("GEN MOVE_15 OK: 0x0100->(77,88) target=idem; missed=%zu; trunc rejeitado; "
           "ws final=%zu\n", missed, ws.entities.size());
    return 0;
}
