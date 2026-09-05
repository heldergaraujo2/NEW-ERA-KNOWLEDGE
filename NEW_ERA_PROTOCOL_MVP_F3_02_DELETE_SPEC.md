# NEW-ERA — PROTOCOL SPEC — MVP F3:0x02 DELETE CHARACTER
> 1.1-D · 2026-09-05 · SEM REDE · offline. Status: NORMATIVA.

## 1. Fontes (NEW_ERA_EVIDENCE/, sha completos no Ledger §52)
| # | Fonte | Papel |
|---|---|---|
| G1 | `GS_Protocol.cpp…e8bb4dbb.cpp` | dispatch server (:964-:966) |
| C1 | `0E1L__13__WSclient.cpp` | RX do cliente (:672-:694) |
| H  | `0E1L__14__WSclient.h` | struct do request (:389-:395) |

## 2. Request C→S — `PREQUEST_DELETE_CHARACTER` (WSclient.h :389-:395)
```
[C1][0x19][0xF3][0x02][ID[10]][Resident[10]]   = 25 B
```
- Server: `case 0x02: CGCharacterDeleteRecv((PMSG_CHARACTER_DELETE_RECV*)lpMsg,aIndex)` (GS :964-:966; def server [NOT RECOVERED]).
- **ID**: ASCII zero-padded, MAX_ID_SIZE=10 (provado, §54). **Resident[10]**: número de registro (legado coreano); case 2/default do RX (=RESIDENTWRONG) confirma que o server valida. NEW-ERA: caller passa string ≤10, zero-pad aqui.
- Call-site do send no cliente: [NOT RECOVERED] nesta evidência (struct é do header do próprio cliente ⇒ layout autoritativo).
- XorData32 em [3..25) (padrão §3.2/§43) ⇒ C3 35 B (inner serial+23=24 B = 3 blocos cheios). BuxConvert: NÃO (exclusivo do login F1:01 — sem evidência aqui).

## 3. Response S→C — `PHEADER_DEFAULT_SUBCODE` 5 B (:129-:134; §38)
```
[C1][0x05][0xF3][0x02][Value]   = 5 B
```
`Value` (WSclient.cpp :675-:693):
| Valor | Significado (evidência) |
|---|---|
| 1 | SUCCESS — remove o char local e popup (:677-:682) |
| 0 | GUILD WARNING (:683-:685) |
| 3 | ITEM BLOCK (:686-:688) |
| 2 / default | RESIDENT WRONG (:689-:692) |
GS-style: Encrypt SEM Xor (DataSend :443-:470) ⇒ `streamXored=false`; C3 13 B (inner 4 B → bloco parcial).

## 4. Formato NEW-ERA decidido
- Builder recebe `(id, resident)` (strings ≤10, zero-pad); parser retorna `result` + significado pelo mapa acima. Bounds-check rigoroso; sem campos inventados.
