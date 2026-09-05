# F3:0x02 Delete — golden vectors (1.1-D)

- REQ C->S (C3, 35 B): `c3231cb6385e17ce83d56d340144021e53cab3741410cbfe0431638586303015d097a2`
- RESP S->C stub GS-style SEM Xor (C3, 13 B): `c30d0a572a7cd7dac278183009`
- expected: result=1 (SUCCESS); req id=Hero1 resident=1234567890
- Chaves: Enc1 (req) / Enc2-derivado=inverso(Dec2) (resp, §48). Serials 0x01.
