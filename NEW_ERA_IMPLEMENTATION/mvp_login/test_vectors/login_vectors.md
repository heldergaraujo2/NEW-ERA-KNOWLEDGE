# Golden test vectors — MVP login 0xF1 (1.0-F)

Determinístico (sem rede/sockets/RNG). Chaves reais: keys/Enc1.dat (TX) e keys/Dec2.dat (RX);
Dec1/Enc2-derivados por INVERSO MODULAR (relação §48).

## T1 — login request (C→S)
- plain49 (pós XorData32): `c131f179084441ce2221235ab819ed4e5e69c5e3f547c764e361aa381140a8fe19744fc47bcd572206a651edaffeca991c`
- inner48 (serial+plain[2..49)): `01f179084441ce2221235ab819ed4e5e69c5e3f547c764e361aa381140a8fe19744fc47bcd572206a651edaffeca991c`
- envelope C3 (68 B): `c34497132826c5c6078560a590bc357b7d1dc06218b8c9fc510320971ce2e13435704506156c4258e03160c42015b0c059394505e34a15f7c2ddaa378f8e56e2a130c9fc`
- assert roundtrip: OK

## R1 — F1:00 simulado (stream-Xor)
- C3 (24 B): `c318dae70ace5a8ae385cc083d2f9b7d21c7bba1d4f84678`
- parse: head=F1 sub=00 result=01 heroKey=0x1234 versionOk=true serial=01 — OK

## R2 — F1:01 estilo GS real (Encrypt sem Xor — DataSend :443-:470)
- C3 (13 B): `c30de1dc358a007e647c89320b`
- parse: value=01 serial=02 — OK
