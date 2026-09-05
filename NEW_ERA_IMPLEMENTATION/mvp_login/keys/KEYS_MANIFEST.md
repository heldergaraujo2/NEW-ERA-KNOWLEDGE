# KEYS MANIFEST — SimpleModulus (ENCDEC) — 1.0-D

| Nome | Origem (repo, READ-ONLY) | Destino (isolado) | Bytes | SHA256 |
|---|---|---|---|---|
| Enc1.dat | `muonline/Client/Data/Enc1.dat` | `NEW_ERA_IMPLEMENTATION/mvp_login/keys/Enc1.dat` | 54 | `ee73a59bf278d42735e537c96c25cc1ad0f69f1de089a7b20a1e6d2910a4372d` |
| Dec2.dat | `muonline/Client/Data/Dec2.dat` | `NEW_ERA_IMPLEMENTATION/mvp_login/keys/Dec2.dat` | 54 | `d19e950c44e5648113269cb35d74c25f1c7ca27399e62da95fa127d4d714325e` |

## Arquivos irmãos (NÃO copiados — permanecem só no repo)
| Nome | Path (repo) | Bytes | SHA256 |
|---|---|---|---|
| Enc2.dat (server TX) | `muonline/MuServer/Data/Hack/Enc2.dat` | 54 | `9cf86fdc5030fb986e62b1cf535fc90bc299ddee2a8d59beb0ad34202f37e89c` |
| Dec1.dat (server RX) | `muonline/MuServer/Data/Hack/Dec1.dat` | 54 | `5364ab7d639c584549601dee87fd73e27a509af51454ac33b542936aecbf8efd` |

## Formato (evidência: 0C10A__PacketManager…cpp)
- Header: `uint16 LE = 4370 (0x1122)` + `uint32 LE = 54` (LoadKey :166-:172; esperado fixo em :129/:142) — **validado nos 4 arquivos**.
- Corpo: 3 tabelas `uint32 LE [4]` — Modulus, Key, Xor (:187/:194/:201) — cada uma XOR `m_SaveLoadXor = {0x3F08A79B, 0xE25CC287, 0x93D27AB9, 0x20DEA7BF}` (:37-:40).

## Pareamento PROVADO (1.0-D, verificação offline)
- `Dec1.Key[i] = Enc1.Key[i]⁻¹ mod Enc1.Modulus[i]` (ex.: 0x5BC1⁻¹ mod 0x1F44F = 0x7B38 ✓) — cliente cifra (Enc1) ↔ GS decifra (Dec1).
- `Enc2.Key[i] = Dec2.Key[i]⁻¹ mod Dec2.Modulus[i]` (0x4673⁻¹ mod 0x11E6E = 0x3371 ✓) — GS cifra (Enc2) ↔ cliente decifra (Dec2).
- Modulus/Xor idênticos dentro de cada par — só Key é o inverso modular.
