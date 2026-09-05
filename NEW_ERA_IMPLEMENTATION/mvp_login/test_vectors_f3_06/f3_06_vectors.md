# F3:0x06 AddPoint — golden vectors (1.3-A, WIRE-REAL C1)

- REQ C->S **C1 plain** (5 B, pointType=2 VIT): `c105f37cc2`
- RESP S->C **C1 plain** (11 B): `c10bf30612e803b80ba00f`
- expected: result=0x12 (ok|VIT), stat=2, max=1000 (LifeMax p/ VIT :6216), shield=3000, skillMana=4000
- Wire real: SendRequestAddPoint :1189-:1195 c/ Send()=FALSE (C1; sem C3/crypto).
