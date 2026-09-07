# rx_character_info_f3_03_vectors.md



Fonte normativa: `NEW_ERA_PROTOCOL_MVP_RX_CHARACTER_INFO_F3_03_SPEC.md`



## charinfo_basic_nontrivial

- Frame hex (66B): `c1 42 f3 03 ...`

- Destaques:

  - X=50, Y=60, Map=0, Dir=3

  - Exp bytes BE: `01 02 03 04 05 06 07 08`

  - NextExp bytes BE: `11 12 13 14 15 16 17 18`

  - WORDs/DWORD little-endian (ex.: Money 0x11223344 = `44 33 22 11`)



## charinfo_size_mismatch_invalid

- Frame inválido: size byte 0x41 com payload de 0x42 bytes (deve rejeitar).

