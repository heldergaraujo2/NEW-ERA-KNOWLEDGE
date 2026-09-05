// NEW-ERA — 1.3-L — generate_both_position_vectors.cpp (BOTH_POSITION olc 0x0006)
// Golden determinístico do CANAL NOVO de position + EQUIVALÊNCIA com 0x15:
//   frame olc = [id:u16=0x0006 LE][size:u32 LE][body 7 B] (total 13 B)
//   body = [PBMSG_HEADER embutido 3 B — OPACO, NÃO lido pelo handler]
//          [KeyH][KeyL][X][Y]  (Key BE SEM máscara :1749 — idêntico ao 0x15)
// PROVA (spec 1.3-L): enum ProtocolSend.h :7-:26 (BOTH_POSITION=6) + ponte
// ProtocolSend.cpp :92-:94 (ReceiveMovePosition(msg.body.data())) +
// PRECEIVE_MOVE_POSITION header INCONDICIONAL (WSclient.h :892-:898) ⇒ os 3
// bytes de header viajam DENTRO do body olc (divergência de migração vs MOVE).
//   both_ok    13 B: 0600 07000000 C10715 0100 4D58 (key 0x0100 → (77,88))
//   pkt15_ok    7 B: C10715 0100 4D58               (clássico, mesmo efeito)
//   both_miss  13 B: 0600 07000000 C10715 3333 0909 (key 0x3333 inexistente)
//   both_trunc 12 B: 0600 07000000 C10715 0100 4D   (size=7, body 6 B — falha)
// EQUIVALÊNCIA: both_ok seguido de pkt15_ok produz o MESMO estado (x/y/target;
// dir/angle inalterados nos dois). Seed: frame12/13 dos JSONS VERSIONADOS.
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

// comparação CAMPO-A-CAMPO (EntityRecord não tem operator==)
static bool SameEntity(const mvp::EntityRecord& a, const mvp::EntityRecord& b) {
    if (a.key != b.key || a.kind != b.kind) return false;
    if (a.x != b.x || a.y != b.y) return false;
    if (a.targetX != b.targetX || a.targetY != b.targetY) return false;
    if (a.dir != b.dir || a.angleDeg != b.angleDeg) return false;
    if (a.pk != b.pk || a.classByte != b.classByte || a.type != b.type) return false;
    if (std::memcmp(a.id, b.id, sizeof(a.id)) != 0) return false;
    return a.buffs == b.buffs;
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

    // ---------- frames BOTH_POSITION + 0x15 ----------
    // body olc = header PBMSG embutido (C1 07 15 por convenção clássica; OPACO
    // — o handler :1746-:1767 NÃO le esses bytes; parser NÃO valida) + payload
    const uint8_t hdr3[3] = { 0xC1, 0x07, 0x15 };
    std::vector<uint8_t> bothOk(13, 0);
    {   const uint16_t id = 0x0006; const uint32_t sz = 7;
        std::memcpy(&bothOk[0], &id, 2); std::memcpy(&bothOk[2], &sz, 4);
        const uint8_t body[7] = { hdr3[0], hdr3[1], hdr3[2], 0x01, 0x00, 77, 88 };
        std::memcpy(&bothOk[6], body, 7); }
    std::vector<uint8_t> pkt15 = { 0xC1, 0x07, 0x15, 0x01, 0x00, 77, 88 };
    std::vector<uint8_t> bothMis(13, 0);
    {   const uint16_t id = 0x0006; const uint32_t sz = 7;
        std::memcpy(&bothMis[0], &id, 2); std::memcpy(&bothMis[2], &sz, 4);
        const uint8_t body[7] = { hdr3[0], hdr3[1], hdr3[2], 0x33, 0x33, 9, 9 };
        std::memcpy(&bothMis[6], body, 7); }
    // trunc: header declara size=7 mas body tem 6 B (falta Y) => frame 12 B
    std::vector<uint8_t> bothTrc(12, 0);
    {   const uint16_t id = 0x0006; const uint32_t sz = 7;
        std::memcpy(&bothTrc[0], &id, 2); std::memcpy(&bothTrc[2], &sz, 4);
        const uint8_t body6[6] = { hdr3[0], hdr3[1], hdr3[2], 0x01, 0x00, 77 };
        std::memcpy(&bothTrc[6], body6, 6); }
    assert(bothOk.size() == 13 && pkt15.size() == 7 && bothMis.size() == 13 && bothTrc.size() == 12);

    // ---------- ciclo: seed + both_ok + pkt15 (equiv) + miss + trunc ----------
    mvp::WorldState ws; ws.clear(); std::string err;
    assert(mvp::ApplyFrame_C2_12_Characters(frame12, ws, err));
    assert(mvp::ApplyFrame_C2_13_Monsters(frame13, ws, err));
    assert(ws.entities.size() == 4);
    const uint8_t dirSpawn = ws.entities.at(0x0100).dir;  // dir NÃO muda em nenhum

    size_t missed = 0;
    assert(mvp::ApplyFrame_BOTH_POSITION_Asio(bothOk, ws, err, &missed));
    assert(missed == 0);
    assert(ws.entities.at(0x0100).x == 77 && ws.entities.at(0x0100).y == 88);
    assert(ws.entities.at(0x0100).targetX == 77 && ws.entities.at(0x0100).targetY == 88);
    assert(ws.entities.at(0x0100).dir == dirSpawn);       // sem dir no pacote

    // EQUIVALÊNCIA: 0x15 clássico sobre o MESMO estado => MESMO estado final
    const mvp::WorldState afterBoth = ws;
    size_t missed15 = 0;
    assert(mvp::ApplyFrame_PositionUpdate_C1(pkt15, ws, err, &missed15));
    assert(missed15 == 0);
    assert(SameWorld(ws, afterBoth));

    assert(mvp::ApplyFrame_BOTH_POSITION_Asio(bothMis, ws, err, &missed));
    assert(missed == 1);
    assert(ws.entities.size() == 4 && !ws.entities.count(0x3333));

    const mvp::WorldState snap2 = ws;
    assert(!mvp::ApplyFrame_BOTH_POSITION_Asio(bothTrc, ws, err, &missed));
    assert(err.rfind("0x0006:", 0) == 0);
    assert(missed == 1);                                  // trunc não contabiliza
    assert(SameWorld(ws, snap2));

    // ---------- saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_both_position_0006/both_position_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_both_position_0006/both_position_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"both_position_asio_0x0006_equiv_0x15\",\n";
      j << "  \"frame12_c2_hex\": \"" << Hex(frame12) << "\",\n";
      j << "  \"frame13_c2_hex\": \"" << Hex(frame13) << "\",\n";
      j << "  \"both_ok_hex\": \"" << Hex(bothOk) << "\",\n";
      j << "  \"pkt15_ok_hex\": \"" << Hex(pkt15) << "\",\n";
      j << "  \"both_miss_hex\": \"" << Hex(bothMis) << "\",\n";
      j << "  \"both_trunc_hex\": \"" << Hex(bothTrc) << "\",\n";
      j << "  \"expected\": { \"moved_key\": 256, \"moved_key_hex\": \"0x0100\","
        << " \"x\": 77, \"y\": 88, \"target\": [ 77, 88 ], \"missed_final\": 1,"
        << " \"ws_size\": 4, \"equiv_0x15\": true, \"dir_unchanged\": true },\n";
      j << "  \"trunc\": { \"applied\": false, \"error\": \"" << err << "\" } \n}\n"; }
    { std::ofstream m(mpath);
      m << "# BOTH_POSITION (ASIO 0x0006) + equivalência 0x15 — golden vectors (1.3-L)\n\n";
      m << "- both_ok **olc 13 B**: `" << Hex(bothOk) << "` (key 0x0100 → x/y/target 77/88; hdr3 embutido C1 07 15 OPACO)\n";
      m << "- pkt15_ok **C1 7 B**: `" << Hex(pkt15) << "` (clássico — MESMO efeito sobre o estado)\n";
      m << "- both_miss **olc 13 B**: `" << Hex(bothMis) << "` (key 0x3333 inexistente → missed=1)\n";
      m << "- both_trunc **12 B**: `" << Hex(bothTrc) << "` (size diz 7, body 6 — DEVE falhar)\n";
      m << "- Equivalência provada: both_ok ⇒ pkt15_ok mantém estado idêntico (campo a campo)\n";
      m << "- Wire: enum :7-:26 + ponte :92-:94 + PRECEIVE_MOVE_POSITION header incondicional :892-:898\n"; }

    printf("GEN BOTH_POSITION OK: 0x0100 x/y/target=(77,88) dir=%u intocado; equiv 0x15 OK; "
           "missed=%zu; trunc rejeitado ('%s')\n",
           (unsigned)dirSpawn, missed, err.c_str());
    return 0;
}
