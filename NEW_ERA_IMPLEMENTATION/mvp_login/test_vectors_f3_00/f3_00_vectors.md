# F3:0x00 CharList — golden vectors (1.2-A1, WIRE-REAL C1)

- REQ C->S **C1 plain** (5 B, byLanguage=1): `c105f37ac7`
- RESP S->C **C1 plain** (40 B, 1 slot): `c128f300030001004865726f31000000000059010000000000000000000000000000000000000000`
- expected: count=1, id=`Hero1`, level=345, maxClass=3
- Wire real: SendRequestCharactersList :288-:296 c/ Send()=FALSE (C1, sem C3/crypto).
