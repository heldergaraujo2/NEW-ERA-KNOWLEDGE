// NEW-ERA — 1.3-K — generate_move_d4_asio_vectors.cpp (BOTH_MOVE olc 0x0007)
// Golden determinístico do movement "com path" no WIRE REAL deste build:
//   frame olc = [id:u16=0x0007 LE][size:u32 LE][body headerless >=5]
//   body = [KeyH][KeyL][DataX][DataY][Path0] (+ path tail opcional, opaco)
// PROVA (spec 1.3-K): ponte ProtocolSend.cpp :95-:96 (msg.body.data()) +
// Defined_Global.h :6 (NEW_PROTOCOL_SYSTEM) + PMOVE_CHARACTER headerless
// (WSclient.h :611-:620). Key BE SEM máscara :1691; dir=Path0>>4 :1699;
// angle=(dir-1)*45 (correlação :2271/:2637); apply não-Hero: target=Data
// :1710-:1711, x/y INALTERADOS (PathFinding2 :1730 interpola).
//   move_ok    11 B: 0700 05000000 0100 4D 58 30  (key 0x0100, (77,88), dir=3=>90°)
//   move_miss  11 B: 0700 05000000 3333 09 09 10  (key 0x3333 inexistente)
//   move_trunc 10 B: 0700 05000000 0100 4D 58     (body 4 B — DEVE falhar)
// Seed: frame12/13 lidos dos JSONS VERSIONADOS (fonte primária).
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

static std::vector<uint8_t> AsioFrame(uint16_t id, const std::vector<uint8_t>& body) {
    std::vector<uint8_t> f(6 + body.size());
    std::memcpy(&f[0], &id, 2);                     // u16 LE
    const uint32_t sz = (uint32_t)body.size();
    std::memcpy(&f[2], &sz, 4);                     // u32 LE
    std::memcpy(&f[6], body.data(), body.size());
    return f;
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

    // ---------- frames BOTH_MOVE ----------
    std::vector<uint8_t> moveOk  = AsioFrame(0x0007, { 0x01, 0x00, 77, 88, 0x30 });
    std::vector<uint8_t> moveMis = AsioFrame(0x0007, { 0x33, 0x33,  9,  9, 0x10 });
    // trunc: header declara size=5 mas body tem 4 B (sem Path0)
    std::vector<uint8_t> moveTrc(10, 0);
    {   const uint16_t id = 0x0007; const uint32_t sz = 5;
        std::memcpy(&moveTrc[0], &id, 2); std::memcpy(&moveTrc[2], &sz, 4);
        const uint8_t body4[4] = { 0x01, 0x00, 77, 88 };
        std::memcpy(&moveTrc[6], body4, 4); }
    assert(moveOk.size() == 11 && moveMis.size() == 11 && moveTrc.size() == 10);

    // ---------- ciclo: seed + move + miss + trunc ----------
    mvp::WorldState ws; ws.clear(); std::string err;
    assert(mvp::ApplyFrame_C2_12_Characters(frame12, ws, err));
    assert(mvp::ApplyFrame_C2_13_Monsters(frame13, ws, err));
    assert(ws.entities.size() == 4);
    const uint8_t xBefore = ws.entities.at(0x0100).x;    // 10 (x NÃO muda)
    const uint8_t yBefore = ws.entities.at(0x0100).y;    // 20

    size_t missed = 0;
    assert(mvp::ApplyFrame_PacketMoveD4_Asio(moveOk, ws, err, &missed));
    assert(missed == 0);
    assert(ws.entities.at(0x0100).targetX == 77 && ws.entities.at(0x0100).targetY == 88);
    assert(ws.entities.at(0x0100).dir == 3 && ws.entities.at(0x0100).angleDeg == 90);
    assert(ws.entities.at(0x0100).x == xBefore && ws.entities.at(0x0100).y == yBefore);

    assert(mvp::ApplyFrame_PacketMoveD4_Asio(moveMis, ws, err, &missed));
    assert(missed == 1);
    assert(ws.entities.size() == 4 && !ws.entities.count(0x3333));

    const mvp::WorldState snap = ws;
    assert(!mvp::ApplyFrame_PacketMoveD4_Asio(moveTrc, ws, err, &missed));
    assert(err.rfind("0xD4:", 0) == 0);
    assert(missed == 1);                                 // trunc não contabiliza
    assert(ws.entities.size() == snap.entities.size());
    assert(ws.entities.at(0x0100).targetX == 77 && ws.entities.at(0x0100).dir == 3);
    for (const auto& kv : snap.entities) assert(ws.entities.count(kv.first));

    // ---------- saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_move_d4_asio/move_d4_asio_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_move_d4_asio/move_d4_asio_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"both_move_asio_0x0007\",\n";
      j << "  \"frame12_c2_hex\": \"" << Hex(frame12) << "\",\n";
      j << "  \"frame13_c2_hex\": \"" << Hex(frame13) << "\",\n";
      j << "  \"move_ok_hex\": \"" << Hex(moveOk) << "\",\n";
      j << "  \"move_miss_hex\": \"" << Hex(moveMis) << "\",\n";
      j << "  \"move_trunc_hex\": \"" << Hex(moveTrc) << "\",\n";
      j << "  \"expected\": { \"moved_key\": 256, \"new_target\": [ 77, 88 ],"
        << " \"dir\": 3, \"angleDeg\": 90, \"xy_unchanged\": [ 10, 20 ],"
        << " \"missed_final\": 1, \"ws_size\": 4 },\n";
      j << "  \"trunc\": { \"applied\": false, \"error\": \"" << err << "\" }\n}\n"; }
    { std::ofstream m(mpath);
      m << "# BOTH_MOVE (ASIO 0x0007) — golden vectors (1.3-K)\n\n";
      m << "- move_ok **olc 11 B**: `" << Hex(moveOk) << "` (key 0x0100 → target 77/88; Path0=0x30 dir=3 ⇒ 90°)\n";
      m << "- move_miss **olc 11 B**: `" << Hex(moveMis) << "` (key 0x3333 inexistente → missed=1)\n";
      m << "- move_trunc **10 B**: `" << Hex(moveTrc) << "` (size diz 5, body 4 — DEVE falhar)\n";
      m << "- Apply: dir/angle/target atualizam; **x/y inalterados** (PathFinding2 :1730 interpola)\n";
      m << "- Wire: ponte :95-:96 + Defined_Global :6 + PMOVE_CHARACTER headerless :611-:620\n"; }

    printf("GEN MOVE_D4_ASIO OK: 0x0100 target=(77,88) dir=3 (90deg) xy=(%u,%u) intocados; "
           "missed=%zu; trunc rejeitado ('%s')\n",
           xBefore, yBefore, missed, err.c_str());
    return 0;
}
