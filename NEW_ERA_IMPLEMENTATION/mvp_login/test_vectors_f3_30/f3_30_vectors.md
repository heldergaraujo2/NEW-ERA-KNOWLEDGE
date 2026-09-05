# F3:0x30 Option — golden vectors (1.2-A3, WIRE-REAL C1)

- REQ C->S **C1 plain** (34 B): `c122f34ae462028e5205044dfb599f3d487ce0928705d17117f7498fa7f4782ec9a4`
- RESP S->C **C1 plain** (34 B): `c122f3301234ffffffffffffffffffffffffffffffffffffa5515745010264000000`
- expected: hotKey0=0x1234 (swap :9398), 9x 0xFFFF, gameOption=0xA5, QWE, chatLog=1, keyR=2, qwer=100
- Wire real: SendRequestHotKey :1597-:1603 c/ Send()=FALSE (C1; C3 era só teste antigo).
