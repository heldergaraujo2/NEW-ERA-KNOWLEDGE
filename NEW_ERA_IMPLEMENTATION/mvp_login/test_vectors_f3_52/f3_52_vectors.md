# F3:0x52 MasterSkill — golden vectors (1.1-F)

- REQ C->S **WIRE REAL C1 plain** (8 B): `c108f328d4841a69`
- REQ C3 opcional de pipeline (13 B): `c30dc8de0a9681b27237783b01`
- RESP S->C stub GS-style SEM Xor (C3, 24 B): `c318b4140e36d9ffa14ceccbfe0c753eb55297963efdc1fa`
- expected: result=1, mlPoint=5, skillNum=123456, skillLevel=3
- Serial 0x01 (resp). Loopback usa o REQ C1 real (primeiro golden wire-format).
