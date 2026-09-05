// NEW-ERA — 1.3-E — generate_viewport_13_vectors.cpp (C2 head 0x13 SPAWN)
// Golden determinístico do spawn de monstro no viewport no formato REAL:
//   RESP S->C = C2 15 B (mínimo: 1 entidade, 0 buffs):
//     [C2][SizeH][SizeL][0x13][count=1] + PCREATE_MONSTER 10 B
//     (WSclient.h :588-:600; handler :2585-:2712; stride :2710).
//   ENTIDADE (ordem PROVADA da struct — SEM campo "ViewPortNumber": ele NAO
//   EXISTE em PCREATE_MONSTER; nota anti-invenção igual ao F3:06 §65):
//     [KeyH][KeyL][TypeH][TypeL][X][Y][TargetX][TargetY][Path][s_BuffCount]
//   Key=0x0123 (CreateFlag=b15=0; TeleportFlag=KeyH.0x40=0) · Type=0x02AA
//   (10-bit, bits altos 0) · X=100/Y=200 · Target 101/201 · Path=0x20
//   (dir=2 => 45°) · s_BuffCount=0 => stride fixa 10. SEM crypto.
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
    const uint16_t kKey = 0x0123;    // b15=0 (CreateFlag), KeyH.0x40=0 (Teleport)
    const uint16_t kType = 0x02AA;   // 682 (<=1023, 10-bit)
    const uint8_t kX = 100, kY = 200, kTX = 101, kTY = 201;
    const uint8_t kDir = 2;          // Path high nibble => Path=0x20

    std::vector<uint8_t> f(15, 0);
    f[0] = 0xC2;
    f[1] = 0x00; f[2] = 0x0F;        // size=(SizeH<<8)|SizeL = 15
    f[3] = 0x13;
    f[4] = 1;                        // count (BYTE :202)
    // PCREATE_MONSTER (ordem da struct :589-:599)
    f[5] = (uint8_t)(kKey >> 8);     // KeyH = 0x01
    f[6] = (uint8_t)(kKey & 0xFF);   // KeyL = 0x23
    f[7] = (uint8_t)(((kType >> 8) & 0x03));  // TypeH = 0x02 (myMob/build=0)
    f[8] = (uint8_t)(kType & 0xFF);  // TypeL = 0xAA
    f[9] = kX; f[10] = kY;
    f[11] = kTX; f[12] = kTY;
    f[13] = (uint8_t)(kDir << 4);    // Path = 0x20
    f[14] = 0;                       // s_BuffCount = 0

    // ---------- asserts de frame ----------
    assert(f.size() == 15 && f[0] == 0xC2);
    assert(((f[1] << 8) | f[2]) == 15);
    assert(f[3] == 0x13);
    assert(f[5] == 0x01 && f[6] == 0x23 && f[7] == 0x02 && f[8] == 0xAA);
    assert(f[13] == 0x20 && f[14] == 0);

    // ---------- parse + asserts ----------
    std::vector<mvp::SpawnEntity> out; std::string err;
    assert(mvp::ParseC2_ViewportMonsterSpawnPlain(f, out, err));
    assert(out.size() == 1);
    assert(out[0].key == 0x0123);
    assert(out[0].type == 0x02AA);
    assert(out[0].x == 100 && out[0].y == 200);
    assert(out[0].targetX == 101 && out[0].targetY == 201);
    assert(out[0].dir == 2 && out[0].angleDeg == 45);   // (2-1)*45 :2637
    assert(!out[0].createFlag && !out[0].teleportFlag && !out[0].myMob);
    assert(out[0].buildTime == 0 && out[0].buffs.empty());

    // ---------- negativos ----------
    std::vector<uint8_t> bad = f; bad.resize(14);       // entidade truncada
    assert(!mvp::ParseC2_ViewportMonsterSpawnPlain(bad, out, err) &&
           err.rfind("0x13:", 0) == 0);
    bad = f; bad[4] = 2;                                // count=2, só 1 entidade
    assert(!mvp::ParseC2_ViewportMonsterSpawnPlain(bad, out, err));
    bad = f; bad[1] = 0x00; bad[2] = 0x0E;              // size inconsistente
    assert(!mvp::ParseC2_ViewportMonsterSpawnPlain(bad, out, err));

    // ---------- saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_13/viewport_13_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_13/viewport_13_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"viewport_spawn_c2_0x13\",\n";
      j << "  \"resp_c2_hex\": \"" << Hex(f) << "\",\n";
      j << "  \"expected_parse\": { \"count\": 1, \"key\": " << out[0].key
        << ", \"type\": " << out[0].type << ", \"x\": " << (int)out[0].x
        << ", \"y\": " << (int)out[0].y << ", \"dir\": " << (int)out[0].dir
        << ", \"angleDeg\": " << out[0].angleDeg << " }\n}\n"; }
    { std::ofstream m(mpath);
      m << "# Viewport spawn 0x13 — golden vectors (1.3-E, C2)\n\n";
      m << "- RESP S->C **C2** (15 B; 1 entidade, 0 buffs): `" << Hex(f) << "`\n";
      m << "- expected: count=1, key=0x0123 (291), type=0x02AA (682), x=100, y=200,\n";
      m << "  target=101/201, dir=2 (Path=0x20 => 45°), flags=0, buffs=0\n";
      m << "- Wire: PWMSG_HEADER :83-:89 + count :202 + PCREATE_MONSTER :588-:600;\n";
      m << "  handler :2585-:2712 (stride :2710). NOTA: \"ViewPortNumber\" do exemplo\n";
      m << "  NÃO EXISTE na struct — não inventado (anti-invenção, igual F3:06).\n"; }

    printf("GEN VIEWPORT_13 OK: resp C2 %zu B: count=1 key=0x%04X type=0x%04X "
           "x=%u y=%u dir=%u (%d°)\n",
           f.size(), out[0].key, out[0].type, out[0].x, out[0].y, out[0].dir,
           out[0].angleDeg);
    return 0;
}
