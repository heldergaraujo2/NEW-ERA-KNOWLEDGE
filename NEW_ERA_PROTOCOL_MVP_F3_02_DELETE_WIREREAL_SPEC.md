# NEW-ERA — PROTOCOL SPEC — MVP F3:0x02 DELETE CHARACTER (WIRE-REAL)
> 1.2-A2 · 2026-09-05 · SEM REDE · offline. Status: NORMATIVA (supersede o formato 1.1-D: Resident[10]+C3 de teste).

## 1. Fontes (NEW_ERA_EVIDENCE/)
| # | Fonte | Bytes | SHA256 | Papel |
|---|---|---|---|---|
| I | `0E1L__21__wsclientinline.h` | 62.684 | `67a6d535…` | TX :310-:320 |
| C1 | `0E1L__13__WSclient.cpp` | 415.884 | `6538d944…` | RX :672-:694 |
| H | `0E1L__14__WSclient.h` | 91.012 | `19d38a3b…` | struct :389-:395 |
| G1 | `GS_Protocol.cpp…e8bb4dbb.cpp` | 135.014 | `e8bb4dbb…` | dispatch :964-:966 |

## 2. Request C→S — WIRE REAL (wsclientinline.h :310-:320)
```
#define SendRequestDeleteCharacter( p_ID, p_Resident )
  spe.Init(0xC1,0xF3) << 0x02;
  spe.AddData(p_ID, strlen(p_ID)); spe.AddNullData(MAX_ID_SIZE - strlen(p_ID));
  spe.AddData(p_Resident, 20); spe.Send();
```
- **[C1][0x22][F3][02][ID[10]][Resident[20]] = 34 B** (size 0x22 montado pelo stream builder; ID zero-padded até MAX_ID_SIZE=10).
- `Send()` default `bEncrypt=FALSE` (:120) ⇒ **C1 plain no wire**; XOR32 encadeado em [3..34) (AddData bXor=TRUE §43).
- ⚠️ Divergência struct×wire: `PREQUEST_DELETE_CHARACTER` (H :389-:395) declara `Resident[10]` (25 B), mas o **macro real envia 20** — wire é autoritativo (estrutura não usada no cast do send).
- Server: `CGCharacterDeleteRecv((PMSG_CHARACTER_DELETE_RECV*)lpMsg)` (G1 :964-:966; def [NOT RECOVERED]).

## 3. Response S→C — inalterado (PHEADER_DEFAULT_SUBCODE 5 B; §38)
`[C1][05][F3][02][Value]` — Value (:675-:693): **1=SUCCESS · 0=GUILDWARNING · 3=ITEM_BLOCK · 2/default=RESIDENTWRONG**.

## 4. Nota de migração
Versão antiga do MVP usava Resident[10] (struct) e golden C3; **agora wire-real C1 + Resident[20]**. Builders C3 antigos marcados DEPRECATED no core.

## 5. Formato NEW-ERA
- `BuildC1_F3_02_DeleteRequestWire(const std::string& id10, const std::array<uint8_t,20>& resident20)` → 34 B + XOR32 [3..34).
- Parser `ParseC1_F3_02_DeleteResponsePlain` inalterado (5 B, result).
