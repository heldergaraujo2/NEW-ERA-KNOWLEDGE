// NEW-ERA — 1.3-G — generate_viewport_12_vectors.cpp (C2 0x12 CHARACTER SPAWN)
// Golden determinístico do spawn de players/NPCs (2 entidades, buffs 0 e 2):
//   RESP S->C = C2 79 B [C2][00][4F][0x12][count=2]
//     + Entidade A (PCREATE_CHARACTER 36 B fixos, 0 buffs  = stride 36 :2376)
//     + Entidade B (36 B + 2 buffs [55 66]                 = stride 38 :2376)
//   Layout por PCREATE_CHARACTER (WSclient.h :537-:550):
//   [KeyH][KeyL][X][Y][Class][Equip17][ID10][TX][TY][Path][n][buffs…]
//   A: Key=0x0100 X=10 Y=20 Class=0x03(pose3) Equip=00..10 "HeroA" T=11/21
//      Path=0x13 (dir=1 => 0°; PK=3) n=0
//   B: Key=0x0200 X=30 Y=40 Class=0x01(pose1=teleport) Equip=AA×17 "HeroB"
//      T=31/41 Path=0x20 (dir=2 => 45°; PK=0) n=2 buffs=[55 66]
//   Equipment preservado OPACO (ChangeCharacterExt :2320 é render-layer).
//   SEM crypto. TotalSize = 4+1+36+38 = **79 B (0x004F)** — o valor 115 do
//   comando DIVERGE da soma dos próprios componentes; evidência prevalece
//   (precedente anti-invenção F3:06/0x13 "ViewPortNumber").
#include "mvp_login_client.cpp"

#include <cassert>
#include <cstdio>
#include <cstring>
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
    std::vector<uint8_t> f(79, 0);
    f[0] = 0xC2;
    f[1] = 0x00; f[2] = 0x4F;        // size=(SizeH<<8)|SizeL = 79
    f[3] = 0x12;
    f[4] = 2;                        // count (BYTE :202)
    // ---- Entidade A (off 5; 36 B fixos; stride 36) ----
    f[5] = 0x01; f[6] = 0x00;        // Key=0x0100 (CreateFlag=0)
    f[7] = 10; f[8] = 20;            // X/Y
    f[9] = 0x03;                     // Class (pose=&0x07=3)
    for (int i = 0; i < 17; ++i) f[10 + i] = (uint8_t)i;   // Equip 0x00..0x10
    std::memcpy(&f[27], "HeroA", 5);                        // ID[10] zero-pad
    f[37] = 11; f[38] = 21;          // Target
    f[39] = 0x13;                    // Path: dir=1 (hi) | PK=3 (lo)
    f[40] = 0;                       // s_BuffCount=0
    // ---- Entidade B (off 41; 36 B + 2 buffs; stride 38) ----
    f[41] = 0x02; f[42] = 0x00;      // Key=0x0200
    f[43] = 30; f[44] = 40;
    f[45] = 0x01;                    // Class (pose=1 = teleport :2242)
    for (int i = 0; i < 17; ++i) f[46 + i] = 0xAA;          // Equip AA×17
    std::memcpy(&f[63], "HeroB", 5);
    f[73] = 31; f[74] = 41;
    f[75] = 0x20;                    // Path: dir=2 (hi) | PK=0 (lo)
    f[76] = 2;                       // s_BuffCount=2
    f[77] = 0x55; f[78] = 0x66;      // buffs

    // ---------- asserts de frame ----------
    assert(f.size() == 79 && f[0] == 0xC2);
    assert(((f[1] << 8) | f[2]) == 79);
    assert(f[3] == 0x12 && f[4] == 2);

    // ---------- parse + asserts ----------
    std::vector<mvp::SpawnCharacter> out; std::string err;
    assert(mvp::ParseViewportCharacterSpawnPlain_C2(f, out, err));
    assert(out.size() == 2);
    assert(out[0].key == 0x0100 && std::string(out[0].id) == "HeroA");
    assert(out[0].x == 10 && out[0].y == 20);
    assert(out[0].classByte == 0x03 && out[0].poseAction == 3);
    assert(out[0].dir == 1 && out[0].pk == 3 && out[0].angleDeg == 0);  // (1-1)*45
    assert(!out[0].createFlag && out[0].buffs.empty());
    assert(out[0].equipment[0] == 0x00 && out[0].equipment[16] == 0x10);  // opaco OK
    assert(out[1].key == 0x0200 && std::string(out[1].id) == "HeroB");
    assert(out[1].x == 30 && out[1].y == 40);
    assert(out[1].classByte == 0x01 && out[1].poseAction == 1);        // teleport
    assert(out[1].dir == 2 && out[1].pk == 0 && out[1].angleDeg == 45); // (2-1)*45
    assert(out[1].buffs.size() == 2 && out[1].buffs[0] == 0x55 &&
           out[1].buffs[1] == 0x66);
    assert(out[1].equipment[0] == 0xAA && out[1].equipment[16] == 0xAA);

    // ---------- negativos ----------
    std::vector<uint8_t> bad = f; bad[76] = 17;          // B: s_BuffCount>16
    assert(!mvp::ParseViewportCharacterSpawnPlain_C2(bad, out, err) &&
           err.find("MAX_BUFF_SLOT_INDEX") != std::string::npos);
    bad = f; bad.resize(78); bad[2] = 78;                // buffs de B truncados
    assert(!mvp::ParseViewportCharacterSpawnPlain_C2(bad, out, err) &&
           err.find("truncados") != std::string::npos);
    bad = f; bad.push_back(0xFF); bad[2] = 80;           // byte residual
    assert(!mvp::ParseViewportCharacterSpawnPlain_C2(bad, out, err) &&
           err.find("residuos") != std::string::npos);

    // ---------- saídas golden ----------
    const char* jpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_12/viewport_12_vectors.json";
    const char* mpath = "NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_12/viewport_12_vectors.md";
    { std::ofstream j(jpath);
      j << "{\n  \"caso\": \"viewport_character_c2_0x12\",\n";
      j << "  \"resp_c2_hex\": \"" << Hex(f) << "\",\n";
      j << "  \"expected_parse\": [\n";
      j << "    { \"key\": " << out[0].key << ", \"id\": \"" << out[0].id
        << "\", \"x\": " << (int)out[0].x << ", \"y\": " << (int)out[0].y
        << ", \"class\": " << (int)out[0].classByte
        << ", \"pose\": " << (int)out[0].poseAction
        << ", \"dir\": " << (int)out[0].dir << ", \"pk\": " << (int)out[0].pk
        << ", \"angleDeg\": " << out[0].angleDeg << ", \"buffs_len\": 0"
        << ", \"buffs_bytes\": [], \"equip_first_last\": [ 0, 16 ] },\n";
      j << "    { \"key\": " << out[1].key << ", \"id\": \"" << out[1].id
        << "\", \"x\": " << (int)out[1].x << ", \"y\": " << (int)out[1].y
        << ", \"class\": " << (int)out[1].classByte
        << ", \"pose\": " << (int)out[1].poseAction
        << ", \"dir\": " << (int)out[1].dir << ", \"pk\": " << (int)out[1].pk
        << ", \"angleDeg\": " << out[1].angleDeg << ", \"buffs_len\": 2"
        << ", \"buffs_bytes\": [ 85, 102 ], \"equip_first_last\": [ 170, 170 ] }\n";
      j << "  ]\n}\n"; }
    { std::ofstream m(mpath);
      m << "# Viewport 0x12 character — golden vectors (1.3-G, C2)\n\n";
      m << "- RESP S->C **C2** (79 B; 2 entidades, strides **36/38** :2376): `"
        << Hex(f) << "`\n";
      m << "- A: key=0x0100 \"HeroA\" x=10 y=20 class=0x03(pose3) Path=0x13 "
        << "(dir=1→0°, PK=3) equip=0x00..0x10 buffs=0\n";
      m << "- B: key=0x0200 \"HeroB\" x=30 y=40 class=0x01(teleport) Path=0x20 "
        << "(dir=2→45°, PK=0) equip=0xAA×17 buffs=[55 66]\n";
      m << "- Negativos: s_BuffCount=17 (>16) · buffs truncados · byte residual\n";
      m << "- Wire: PCREATE_CHARACTER :537-:550 (EQUIPMENT_LENGTH=17 :71; Path\n";
      m << "  dual dir/PK :2237/:2271). Equipment viaja OPACO (render-layer).\n"; }

    printf("GEN VIEWPORT_12 OK: resp C2 %zu B: 2 entidades (strides 36/38): "
           "A %s dir=%u pk=%u, B %s dir=%u pk=%u buffs=%zuB\n",
           f.size(), out[0].id, out[0].dir, out[0].pk, out[1].id, out[1].dir,
           out[1].pk, out[1].buffs.size());
    return 0;
}
