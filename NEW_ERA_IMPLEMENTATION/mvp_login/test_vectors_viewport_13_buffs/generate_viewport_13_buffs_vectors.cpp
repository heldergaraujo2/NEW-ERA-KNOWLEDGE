// NEW-ERA — 1.3-F — generate_viewport_13_buffs_vectors.cpp (C2 0x13 + BUFFS)
// Golden determinístico do spawn c/ STRIDE VARIÁVEL (2 entidades, buffs 1 e 3):
//   RESP S->C = C2 29 B [C2][00][1D][0x13][count=2]
//     + Entidade A (PCREATE_MONSTER 10 B + 1 buff  = stride 11 :2710)
//     + Entidade B (PCREATE_MONSTER 10 B + 3 buffs = stride 13 :2710)
//   Layout por PCREATE_MONSTER (WSclient.h :588-:600):
//   [KeyH][KeyL][TypeH][TypeL][X][Y][TargetX][TargetY][Path][s_BuffCount][buffs…]
//   A: Key=0x0101 Type=0x0011 X=10 Y=20 T=11/21 Path=0x10(dir 1) buffs=[AA]
//   B: Key=0x0202 Type=0x0022 X=30 Y=40 T=31/41 Path=0x20(dir 2) buffs=[01 02 03]
//   SEM crypto. Harden 1.3-F: s_BuffCount>16 rejeitado (MAX_BUFF_SLOT_INDEX
//   :599/:613). TotalSize = 4+1+11+13 = 29.
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
    std::vector<uint8_t> f(29, 0);
    f[0] = 0xC2;
    f[1] = 0x00; f[2] = 0x1D;        // size=(SizeH<<8)|SizeL = 29
    f[3] = 0x13;
    f[4] = 2;                        // count (BYTE :202)
    // ---- Entidade A (off 5; stride 11) ----
    f[5] = 0x01; f[6] = 0x01;        // Key=0x0101 (flags 0)
    f[7] = 0x00; f[8] = 0x11;        // Type=0x0011 (10-bit, bits altos 0)
    f[9] = 10; f[10] = 20;           // X/Y
    f[11] = 11; f[12] = 21;          // Target
    f[13] = 0x10;                    // Path: dir=1 => ((1-1)*45)=0°
    f[14] = 1;                       // s_BuffCount=1
    f[15] = 0xAA;                    // buff[0]
    // ---- Entidade B (off 16; stride 13) ----
    f[16] = 0x02; f[17] = 0x02;      // Key=0x0202
    f[18] = 0x00; f[19] = 0x22;      // Type=0x0022
    f[20] = 30; f[21] = 40;
    f[22] = 31; f[23] = 41;
    f[24] = 0x20;                    // Path: dir=2 => 45°
    f[25] = 3;                       // s_BuffCount=3
    f[26] = 0x01; f[27] = 0x02; f[28] = 0x03;

    // ---------- asserts de frame ----------
    assert(f.size() == 29 && f[0] == 0xC2);
    assert(((f[1] << 8) | f[2]) == 29);
    assert(f[3] == 0x13 && f[4] == 2);

    // ---------- parse + asserts ----------
    std::vector<mvp::SpawnEntity> out; std::string err;
    assert(mvp::ParseC2_ViewportMonsterSpawnPlain(f, out, err));
    assert(out.size() == 2);
    assert(out[0].key == 0x0101 && out[0].type == 0x0011);
    assert(out[0].x == 10 && out[0].y == 20);
    assert(out[0].targetX == 11 && out[0].targetY == 21);
    assert(out[0].dir == 1 && out[0].angleDeg == 0);      // (1-1)*45
    assert(out[0].buffs.size() == 1 && out[0].buffs[0] == 0xAA);
    assert(out[1].key == 0x0202 && out[1].type == 0x0022);
    assert(out[1].x == 30 && out[1].y == 40);
    assert(out[1].dir == 2 && out[1].angleDeg == 45);     // (2-1)*45
    assert(out[1].buffs.size() == 3);
    assert(out[1].buffs[0] == 1 && out[1].buffs[1] == 2 && out[1].buffs[2] == 3);

    // ---------- negativos (harden 1.3-F) ----------
    std::vector<uint8_t> bad = f; bad[14] = 17;           // A: s_BuffCount>16
    assert(!mvp::ParseC2_ViewportMonsterSpawnPlain(bad, out, err) &&
           err.find("MAX_BUFF_SLOT_INDEX") != std::string::npos);
    bad = f; bad.resize(27); bad[2] = 27;                 // buffs de B truncados
    assert(!mvp::ParseC2_ViewportMonsterSpawnPlain(bad, out, err) &&
           err.find("truncados") != std::string::npos);
    bad = f; bad.push_back(0xFF); bad[2] = 30;            // byte residual
    assert(!mvp::ParseC2_ViewportMonsterSpawnPlain(bad, out, err) &&
           err.find("residuos") != std::string::npos);

    // ---------- saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_13_buffs/viewport_13_buffs_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_13_buffs/viewport_13_buffs_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"viewport_spawn_c2_0x13_buffs\",\n";
      j << "  \"resp_c2_hex\": \"" << Hex(f) << "\",\n";
      j << "  \"expected_parse\": [\n";
      j << "    { \"key\": " << out[0].key << ", \"type\": " << out[0].type
        << ", \"x\": " << (int)out[0].x << ", \"y\": " << (int)out[0].y
        << ", \"dir\": " << (int)out[0].dir << ", \"angleDeg\": " << out[0].angleDeg
        << ", \"buffs\": [ 170 ] },\n";
      j << "    { \"key\": " << out[1].key << ", \"type\": " << out[1].type
        << ", \"x\": " << (int)out[1].x << ", \"y\": " << (int)out[1].y
        << ", \"dir\": " << (int)out[1].dir << ", \"angleDeg\": " << out[1].angleDeg
        << ", \"buffs\": [ 1, 2, 3 ] }\n";
      j << "  ]\n}\n"; }
    { std::ofstream m(mpath);
      m << "# Viewport spawn 0x13 com BUFFS — golden vectors (1.3-F, C2)\n\n";
      m << "- RESP S->C **C2** (29 B; 2 entidades, strides **11 e 13** :2710): `"
        << Hex(f) << "`\n";
      m << "- A: key=0x0101 type=0x0011 x=10 y=20 dir=1 (0°) buffs=[0xAA]\n";
      m << "- B: key=0x0202 type=0x0022 x=30 y=40 dir=2 (45°) buffs=[01 02 03]\n";
      m << "- Negativos: s_BuffCount=17 (>16, harden) · buffs truncados · byte residual\n";
      m << "- Wire: PCREATE_MONSTER :588-:600 + s_BuffCount bytes; MAX_BUFF=16 :613.\n"; }

    printf("GEN VIEWPORT_13_BUFFS OK: resp C2 %zu B: 2 entidades (strides 11/13): "
           "A key=0x%04X buffs=%zuB, B key=0x%04X buffs=%zuB\n",
           f.size(), out[0].key, out[0].buffs.size(), out[1].key, out[1].buffs.size());
    return 0;
}
