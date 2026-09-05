# NEW-ERA — PROTOCOL SPEC — MVP F3:0x52 MASTER SKILL
> 1.1-F · 2026-09-05 · SEM REDE · offline. Status: NORMATIVA.

## 1. Fontes (NEW_ERA_EVIDENCE/)
| # | Fonte | Bytes | SHA256 | Papel |
|---|---|---|---|---|
| G1 | `GS_Protocol.cpp…e8bb4dbb.cpp` | 135.014 | `e8bb4dbb…` | dispatch :999-:1002 |
| C1 | `0E1L__13__WSclient.cpp` | 415.884 | `6538d944…` | RX :7387-:7412+ |
| H | `0E1L__14__WSclient.h` | 91.012 | `19d38a3b…` | struct :2860-:2868 |
| I | `0E1L__21__wsclientinline.h` | 62.684 | `67a6d535…` | TX :2286-:2292 |

## 2. Request C→S — REAL WIRE **C1 PLAIN** (wsclientinline.h :2286-:2292)
```
#define SendRequestMasterLevelSkill(SkillNum)
  spe.Init(0xC1,0xF3) << (BYTE)0x52 << (int)SkillNum; spe.Send();
```
- Layout: **[C1][0x08][F3][52][SkillNum int32 LE] = 8 B** + XOR32 encadeado em [3..8) (operator<</AddData com bXor=TRUE — §43).
- `Send()` default `bEncrypt=FALSE` (StreamPacketEngine.h :120) ⇒ **C1 plain no wire** (§60). O builder C3 NEW-ERA (`BuildC3_…Encrypted`) existe só para exercitar o pipeline crypto em teste (C3 13 B; inner serial+6=7 B → bloco parcial).
- Server: `case 0x52: #if(GAMESERVER_UPDATE>=401) gMasterSkillTree.CGMasterSkillRecv((PMSG_MASTER_SKILL_RECV*)lpMsg,aIndex)` :999-:1002 (def server [NOT RECOVERED]).

## 3. Response S→C — `PMSG_ANS_MASTERLEVEL_SKILL` (WSclient.h :2860-:2868)
```
[C1][0x0F][F3][52][btResult][nMLPoint i16][nSkillNum i32][nSkillLevel i32] = 15 B
```
- `h` (PBMSG 3 B) + `subcode` = prefixo 4 B; depois 1+2+4+4 = 11 ⇒ **15 B**.
- Consumo (WSclient.cpp :7389-:7397+): `btResult==1` habilita (:7391); `nSkillNum > -1` (:7393); `switch(nSkillNum)` por AT_SKILL_*_UP (:7397+); `nMLPoint`/`nSkillLevel` da struct (:2865-:2867).
- GS-style: Encrypt SEM Xor ⇒ `streamXored=false`; golden C3 24 B (inner serial+13=14 B → 2 blocos: 8+6 parcial → ct 22).

## 4. Direcionalidade (duplex §35)
Mesmo subcode 0x52 em direções opostas: request minimal (int32) ↔ response rico (15 B). Não é o mesmo pacote.

## 5. Formato NEW-ERA
- `BuildC1_F3_52_MasterSkillRequestPlain(int32_t)` → 8 B (wire REAL; XOR32 [3..8)) — **usado no golden/loopback (primeiro golden em wire-format real)**.
- `BuildC3_F3_52_MasterSkillRequestEncrypted(int32_t, err)` → C3 13 B (opcional, teste de pipeline).
- `ParseC1_F3_52_MasterSkillResponsePlain` → `ParsedMasterSkill{result, mlPoint, skillNum, skillLevel}`; bounds-check 15 B.
