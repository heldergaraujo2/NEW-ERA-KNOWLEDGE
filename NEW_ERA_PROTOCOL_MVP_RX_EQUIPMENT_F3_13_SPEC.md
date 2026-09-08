# NEW_ERA_PROTOCOL_MVP_RX_EQUIPMENT_F3_13_SPEC.md
Data: 2026-09-07  
Status: NORMATIVO (Fase 1 — 1.3-Z P2)

## Objetivo
Congelar o contrato on-wire (RX) para **F3:13** (Equipment/CharSet), usado para aparência de personagens no viewport.

Base (evidência): Ledger §93 + report `EVIDENCE/1.3-Z/...`.

---

## 1) Framing (C1 fixo)
Formato fixo (24 bytes):
`[C1][size=0x18][head=0xF3][sub=0x13] + body(20B)`

Guards normativos:
- frame[0]==0xC1
- frame[1]==len(frame)==0x18
- frame[2]==0xF3
- frame[3]==0x13

---

## 2) Layout do body (20 bytes)
Offsets a partir do início do frame:

- +4..+5: `index` (u16 big-endian)
- +6..+23: `CharSet[18]` (18 bytes)

### Nuance (client pinado)
O client pinado modela o payload como:
- `Class = CharSet[0]` (byte em +6)
- `Equipment[17] = CharSet[1..17]` (bytes +7..+23)

O handler `ReceiveEquipment` aplica somente `Equipment[17]` via `ChangeCharacterExt(...)`.
No MVP, é válido:
- armazenar `classByte = CharSet[0]` (opcional, para render/skin),
- e armazenar `equipment17 = CharSet[1..17]` para a aparência.

---

## 3) Guards normativos (parser NEW-ERA)
- Rejeitar se `Size != 0x18`
- Rejeitar se `index == 0` (opcional; se o legado aceitar 0, marcar como PROBABLE e ajustar)
- Não alterar estado se frame inválido

---

## 4) Transporte moderno
No pin atual (`NEW_PROTOCOL_SYSTEM=1`), este C1 viaja dentro do `BOTH_MESSAGE` (envelope), mas o parser aqui recebe o frame clássico já extraído.
