# NEW-ERA — PROTOCOL SPEC — MVP F3:0x06 ADD POINT (WIRE-REAL)
> 1.3-A · 2026-09-05 · SEM REDE · offline. Status: NORMATIVA (wire-real desde o nascimento — C1 plain; C3 não é usado aqui).

## 1. Fontes (NEW_ERA_EVIDENCE/)
| # | Fonte | Bytes | SHA256 | Papel |
|---|---|---|---|---|
| I | `0E1L__21__wsclientinline.h` | 62.684 | `67a6d535…` | TX :1189-:1195 |
| C1 | `0E1L__13__WSclient.cpp` | 415.884 | `6538d944…` | RX :6200-:6230 |
| H | `0E1L__14__WSclient.h` | 91.012 | `19d38a3b…` | struct :882-:890 |
| G1 | `GS_Protocol.cpp…e8bb4dbb.cpp` | 135.014 | `e8bb4dbb…` | dispatch :969-:971 |

## 2. Request C→S — WIRE REAL (wsclientinline.h :1189-:1195)
```
#define SendRequestAddPoint( p_Type )
  spe.Init(0xC1,0xF3) << (BYTE)0x06 << (BYTE)(p_Type); spe.Send();
```
- **[C1][0x05][F3][06][pointType] = 5 B** (size 5 = header 2 + sub 1 + type 1; evidência :1193).
- `Send()` default `bEncrypt=FALSE` (:120) ⇒ **C1 plain**; XOR32 encadeado em [3..5) (§43).
- **pointType**: id do atributo a subir. Call-site do macro [NOT RECOVERED nesta evidência]; correlação 1:1 com o switch do RX (§3): **0=Strength · 1=Dexterity · 2=Vitality · 3=Energy · 4=Charisma** (documentada como correlação, não invenção).
- Server: `CGLevelUpPointRecv((PMSG_LEVEL_UP_POINT_RECV*)lpMsg)` (G1 :969-:971; def [NOT RECOVERED]).

## 3. Response S→C — `PRECEIVE_ADD_POINT` (WSclient.h :882-:890)
```
[C1][0x0B][F3][06][Result][Max:W][ShieldMax:W][SkillManaMax:W] = 11 B
```
Consumo (`ReceiveAddPoint` C1 :6200-:6230):
- `Result` **nibble-packado**: `Result>>4` ≠ 0 ⇒ sucesso (gasta 1 LevelUpPoint :6205); `Result&0xF` = stat: 0=STR · 1=DEX · 2=VIT (+`LifeMax=Max` :6216) · 3=ENE (+`ManaMax=Max` :6220) · 4=CHA (:6222).
- Em sucesso: `SkillManaMax` :6226 e `ShieldMax` :6227 sempre atualizados; `CalculateAll()` :6229.
- Campos WORD LE; bounds-check 11 B no parser.

## 4. Nota
Wire-real C1 plain em ambas as direções (padrão Send()=FALSE, §60). C3/crypto não participam deste fluxo.
