# F3:0x02 Delete — golden vectors (1.2-A2, WIRE-REAL C1)

- REQ C->S **C1 plain** (34 B, Resident[20]): `c122f3788c5bb6aab810ee58114c442b926d3b805d180a659714ce796502ddb36d30`
- RESP S->C **C1 plain** (5 B): `c105f30201`
- expected: result=1 (SUCCESS); id=Hero1 resident=12345678901234567890
- Wire real: SendRequestDeleteCharacter :310-:320 c/ Send()=FALSE (C1, sem C3/crypto).
