# Viewport spawn 0x13 com BUFFS — golden vectors (1.3-F, C2)

- RESP S->C **C2** (29 B; 2 entidades, strides **11 e 13** :2710): `c2001d1302010100110a140b151001aa020200221e281f292003010203`
- A: key=0x0101 type=0x0011 x=10 y=20 dir=1 (0°) buffs=[0xAA]
- B: key=0x0202 type=0x0022 x=30 y=40 dir=2 (45°) buffs=[01 02 03]
- Negativos: s_BuffCount=17 (>16, harden) · buffs truncados · byte residual
- Wire: PCREATE_MONSTER :588-:600 + s_BuffCount bytes; MAX_BUFF=16 :613.
