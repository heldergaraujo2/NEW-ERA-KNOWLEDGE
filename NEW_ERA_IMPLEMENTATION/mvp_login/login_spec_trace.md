# MVP LOGIN — trace de conformidade com a spec (checklist de implementação)
> Fonte: `NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md` (sha e583db6b…) · seções §2/§3/§4/§5/§6.
> Marcar [x] quando o item estiver implementado e verificado.

## Pacote 1 — S→C `C1 0xF1:0x00` JoinServer handshake (12 B) — spec §4.1
| Off | Campo | Valor | [ ] |
|---|---|---|---|
| 0 | Code | 0xC1 | [ ] |
| 1 | Size | 0x0C | [ ] |
| 2 | Head | 0xF1 | [ ] |
| 3 | Sub | 0x00 | [ ] |
| 4 | Result | 0x01=SUCCESS; outro→SERVER_LOST | [ ] |
| 5-6 | NumberH/L | HeroKey=(H<<8)+L | [ ] |
| 7-11 | Version[5] | cliente exige `Version[i]-(i+1)` | [ ] |
Handler: `build_F1_00_receive_join_parser()`.

## Pacote 2 — C→S `C3 0xF1:0x01` login request (49 B lógicos [INFERIDO 10/10]) — spec §4.2
| Off | Campo | Transform | [ ] |
|---|---|---|---|
| 0-3 | C1,size,F1,01 | header interno (vira payload do C3) | [ ] |
| 4-13 | ID[10] | `^BuxConvert` (XOR3 {FC,CF,AB}) | [ ] |
| 14-23 | Password[10] | `^BuxConvert` | [ ] |
| 24-27 | TickCount | DWORD LE, cru | [ ] |
| 28-32 | VersionEnc[5] | `Version[i]-(i+1)` | [ ] |
| 33-48 | Serial[16] | cru (SIZE_PROTOCOLSERIAL=16) | [ ] |
Handler: `build_F1_01_login_request_packet()`.

## Pacote 3 — S→C `C1 0xF1:0x01` login result (≥5 B) — spec §4.3/§6
| Off | Campo | Valor | [ ] |
|---|---|---|---|
| 0-3 | C1,size,F1,01 | header | [ ] |
| 4 | Value | tabela canônica §6 (núcleo {0,1,2,3,4,6}+0x20; default=0x07) | [ ] |
Handler: `parse_F1_01_login_result()`.

## Camadas TX do pacote 2 (ordem exata — spec §3)
1. [ ] BuxConvert em ID/Password (XOR cíclico 3 B, chave {0xFC,0xCF,0xAB}) — §41 PROVADO
2. [ ] Stream builder C1 (`Init(0xC1,0xF1)`; header 4 B sem XOR)
3. [x] XorData32 encadeado por campo (`buf[i]^=buf[i-1]^Filter[i%32]`; tabela 32 B ≡ GS) — §43 PROVADO (**tabela preenchida em 1.0-B**, extraída da evidência, cross-check GS 32/32)
4. [ ] Serial byte (`g_byPacketSerialSend++` na posição size do envelope)
5. [ ] SimpleModulus/CPacketManager por bloco — STUB (1.0-C+)
6. [ ] Envelope: ciphertext <256 B ⇒ **C3** `[0xC3][len][ct…]` (senão C4)

## Camadas RX (pacotes 1 e 3) — spec §3
- [x] aceitar C1 plain (ENCRYPT_STATE=0; result S→C sai como C1) — 1.0-E
- [x] RX C3: Decrypt (DecryptBlock 11→8) + serial check (resync não-fatal, evidência :11703-:11708) + unXor reverso (ExtractPacket :565) — 1.0-E

## Validações espelhadas (spec §5) — cliente-side aplicável
- [ ] Version[5] do handshake: check `Version[i]-(i+1)`
- [ ] Value 6/0/2/3/4/1 → mapear resultado; default → falha genérica
