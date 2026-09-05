# F3:0x01 CreateCharacter — golden vectors (1.3-B, WIRE-REAL C1)

- REQ C->S **C1 plain** (15 B, id/classSkin=0x32): `c10ff37b8f58b5a9c40980367f2229`
- RESP S->C **C1 plain** (19 B): `c113f301014865726f4e657700000000010003`
- expected: result=1, id=HeroNew, index=0, level=1, serverClass=3
- Wire: SendRequestCreateCharacter :298-:308 c/ Send()=FALSE (C1; sem C3).
