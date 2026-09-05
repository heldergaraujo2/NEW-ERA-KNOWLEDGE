# NEW-ERA — PROTOCOL SPEC — MVP F3:0x01 CREATE CHARACTER (WIRE-REAL)
> 1.3-B · 2026-09-05 · SEM REDE · offline. Status: NORMATIVA (wire-real C1 desde o nascimento).

## 1. Fontes (NEW_ERA_EVIDENCE/)
| # | Fonte | Bytes | SHA256 | Papel |
|---|---|---|---|---|
| I | `0E1L__21__wsclientinline.h` | 62.684 | `67a6d535…` | TX :298-:308 |
| C1 | `0E1L__13__WSclient.cpp` | 415.884 | `6538d944…` | RX :622-:670 |
| H | `0E1L__14__WSclient.h` | 91.012 | `19d38a3b…` | structs :367-:373/:376-:386 |
| G1 | `GS_Protocol.cpp…e8bb4dbb.cpp` | 135.014 | `e8bb4dbb…` | dispatch :960-:963 |

## 2. Request C→S — WIRE REAL (wsclientinline.h :298-:308)
```
#define SendRequestCreateCharacter( p_ID, p_Class, p_Skin )
  spe.Init(0xC1,0xF3) << 0x01;
  spe.AddData(p_ID, strlen(p_ID)); spe.AddNullData(MAX_ID_SIZE - strlen(p_ID));
  spe << (BYTE)(((p_Class)<<4) + (p_Skin)); spe.Send();
```
- **[C1][0x0F][F3][01][ID[10] zero-padded][classSkin] = 15 B** — bate com `PREQUEST_CREATE_CHARACTER` (H :367-:373: Header+Sub+ID[10]+Class).
- **classSkin**: 1 byte **nibble-packado** — high = Class, low = Skin (:306).
- `Send()` default FALSE (:120) ⇒ **C1 plain**; XOR32 encadeado em [3..15).
- Validações de nome/classes nos callers: [NOT RECOVERED nesta evidência]. Builder NEW-ERA: bounds-check (nome ≤ 10; class/skin ≤ 0xF cada).
- Server: `CGCharacterCreateRecv((PMSG_CHARACTER_CREATE_RECV*)lpMsg)` (G1 :960-:963; def [NOT RECOVERED]).

## 3. Response S→C — `PRECEIVE_CREATE_CHARACTER` (WSclient.h :376-:386)
```
[C1][0x13][F3][01][Result][ID[10]][Index][Level:W][Class] = 19 B
```
(Equipment[24] comentado no header — não está no wire.)
Semântica (`ReceiveCreateCharacter` C1 :622-:670):
- **Result**: 1 = sucesso (:625) · 0 = FAIL (:664-:665) · 2 = FAIL2 (:666-:667).
- Em sucesso: `Index` = slot 0..4 (switch de posições :629-:644); `Level` WORD (:651); `Class` em **tipo server** — cliente converte via `ChangeServerClassTypeToClientClassType` (:653); `ID[10]` + NUL (:656-:657).

## 4. Nota
C1 plain em ambas as direções (Send default FALSE — §60); sem C3/crypto neste fluxo.
