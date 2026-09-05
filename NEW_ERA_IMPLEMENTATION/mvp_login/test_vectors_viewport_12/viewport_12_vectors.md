# Viewport 0x12 character — golden vectors (1.3-G, C2)

- RESP S->C **C2** (79 B; 2 entidades, strides **36/38** :2376): `c2004f120201000a1403000102030405060708090a0b0c0d0e0f104865726f4100000000000b15130002001e2801aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa4865726f4200000000001f2920025566`
- A: key=0x0100 "HeroA" x=10 y=20 class=0x03(pose3) Path=0x13 (dir=1→0°, PK=3) equip=0x00..0x10 buffs=0
- B: key=0x0200 "HeroB" x=30 y=40 class=0x01(teleport) Path=0x20 (dir=2→45°, PK=0) equip=0xAA×17 buffs=[55 66]
- Negativos: s_BuffCount=17 (>16) · buffs truncados · byte residual
- Wire: PCREATE_CHARACTER :537-:550 (EQUIPMENT_LENGTH=17 :71; Path
  dual dir/PK :2237/:2271). Equipment viaja OPACO (render-layer).
