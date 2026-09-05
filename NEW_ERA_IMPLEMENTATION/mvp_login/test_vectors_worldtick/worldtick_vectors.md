# WorldTick — golden vectors (1.3-H2A: 0x12 + 0x13 + trunc)

- frame12 **C2 79 B** (golden 1.3-G; chars strides 36/38): `c2004f120201000a1403000102030405060708090a0b0c0d0e0f104865726f4100000000000b15130002001e2801aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa4865726f4200000000001f2920025566`
- frame13 **C2 29 B** (golden 1.3-F; monsters strides 11/13): `c2001d1302010100110a140b151001aa020200221e281f292003010203`
- frame13_trunc **28 B**: Apply DEVE falhar (`0x13: tamanho C2 inconsistente (declared 29 != total 28)`) e NÃO alterar ws
- expected pós-tick: 4 entidades (2 chars 0x0100/0x0200 + 2 monsters 0x0101/0x0202)
- Fonte dos frames: jsons versionados viewport_12/viewport_13_buffs (resp_c2_hex).
