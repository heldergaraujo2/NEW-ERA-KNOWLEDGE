# F3:0x00 CharList — golden vectors (1.1-C)

- REQ C->S (C3, 13 B): `c30dcc9123495ae2c26f944e70`
- RESP S->C stub GS-style SEM Xor (C3, 57 B): `c3399ac91ad84713f70266754020e6145956d402635984b1481004c4863de5642a95a0451b0d0f125202d4d8cdf8451b0d0f125202d4d8c2f8`
- expected: count=1, id=`Hero1`, level=345, maxClass=3
- Chaves: Enc1 (req) / Enc2-derivado=inverso(Dec2) (resp, §48). Serials 0x01.
