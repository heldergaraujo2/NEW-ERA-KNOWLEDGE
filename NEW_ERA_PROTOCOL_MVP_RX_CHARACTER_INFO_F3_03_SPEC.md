# NEW_ERA_PROTOCOL_MVP_RX_CHARACTER_INFO_F3_03_SPEC.md

Data: 2026-09-07  

Status: NORMATIVO (Fase 1 — 1.3-Y P2)



## Objetivo

Congelar o contrato on-wire (RX) para o pacote **F3:03** que o client trata em `ReceiveJoinMapServer`,

e que o GS envia como **CharacterInfo** (stats/posição/exp/dinheiro).



Base (evidência): Ledger §91 (Y/W4/X2).



---



## 1) Framing (C1 com subcode)

Formato fixo (66 bytes):

`[C1][size=0x42][head=0xF3][sub=0x03] + body(62B)`



Guards normativos:

- frame[0]==0xC1

- frame[1]==len(frame)==0x42

- frame[2]==0xF3

- frame[3]==0x03



---



## 2) Layout do body (62 bytes) — offsets a partir do início do frame

(endianness)

- Campos `WORD` e `DWORD`: **little-endian** (wire = bytes crus do struct no GS e lidos como WORD/DWORD no client).

- `Exp[8]` e `NextExp[8]`: **big-endian por bytes** (client monta com shifts).



### 2.1) Posição e direção

- +4:  X (u8)

- +5:  Y (u8)

- +6:  MapNumber (u8)

- +7:  Dir/Angle (u8)



### 2.2) Experiência (8B + 8B)

- +8..+15:  Exp[8]  (u8[8], BE-bytes)

- +16..+23: NextExp[8] (u8[8], BE-bytes)



### 2.3) Pontos e stats (WORD LE)

- +24..25: LevelUpPoint (u16 LE)

- +26..27: Strength (u16 LE)

- +28..29: Dexterity (u16 LE)

- +30..31: Vitality (u16 LE)

- +32..33: Energy (u16 LE)



### 2.4) Vitals (WORD LE)

- +34..35: Life (u16 LE)

- +36..37: MaxLife (u16 LE)

- +38..39: Mana (u16 LE)

- +40..41: MaxMana (u16 LE)

- +42..43: Shield (u16 LE)

- +44..45: MaxShield (u16 LE)

- +46..47: BP (SkillMana) (u16 LE)

- +48..49: MaxBP (u16 LE)



### 2.5) Money e flags

- +50..53: Money (u32 LE)

- +54: PKLevel (u8)

- +55: CtlCode (u8)



### 2.6) Fruit/Leadership/Minus (WORD LE)

- +56..57: AddPoint (u16 LE)

- +58..59: MaxAddPoint (u16 LE)

- +60..61: Leadership/Charisma (u16 LE)

- +62..63: MinusPoint (u16 LE)

- +64..65: MaxMinusPoint (u16 LE)



---



## 3) Observações

- No pin atual (`NEW_PROTOCOL_SYSTEM=1`), este C1 viaja **dentro do BOTH_MESSAGE** (envelope moderno), sem conversão C1->C3.

- O GS tem campos condicionais (>=602/EXTRA) na struct server-side; o client pinado não lê esses campos.

