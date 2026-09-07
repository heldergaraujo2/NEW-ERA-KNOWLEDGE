# NEW-ERA_PROTOCOL_MVP_RX_ACTION_MAGIC_SPEC.md
Data: 2026-09-06  
Status: NORMATIVO (Fase 1 — 1.3-S P2)

## Objetivo
Congelar o contrato on-wire (RX) para:
- `0x18` Action (animação/ack de ação/attack)
- `0x19` Magic/Skill event (efeito/skill, com bit de sucesso no target)

Base (evidência): Ledger §83 + reports em `EVIDENCE/1.3-S/` (client structs + GS emitters).

---

## 1) Framing (clássico MU)
Ambos são pacotes **C1** com tamanho fixo **0x09**:

- `C1 09 18 ...6B...`
- `C1 09 19 ...6B...`

Sem subheader.

---

## 2) RX 0x18 — ACTION (C1 len 0x09)
### 2.1) Layout (6 bytes de body)
`C1 09 18 | KeyH KeyL | Angle | Action | TargetKeyH TargetKeyL`

- `Key = (KeyH<<8)|KeyL` (BE)
- `TargetKey = (TargetKeyH<<8)|TargetKeyL` (BE)
- `Angle`: byte (interpretação no client legado: `angleDeg = (Angle-1)*45`)
- `Action`: byte (enum AT_*; ex.: 0x78 aparece como ATTACK1 no fluxo de combate)

### 2.2) Semântica (WorldState NEW-ERA)
- Atualiza estado/anim do ator (`Key`) e do alvo (`TargetKey`) conforme Action.
- O pacote é broadcast (viewport) e ocorre no melee **antes** do damage 0x11 (comprovado no GS).

---

## 3) RX 0x19 — MAGIC/SKILL EVENT (C1 len 0x09)
### 3.1) Layout (6 bytes de body) — client pinado (<701)
`C1 09 19 | MagicH MagicL | SourceKeyH SourceKeyL | TargetKeyH TargetKeyL`

- `MagicNumber = (MagicH<<8)|MagicL` (BE)
- `SourceKey = (SourceKeyH<<8)|SourceKeyL` (BE)
- `TargetWord = (TargetKeyH<<8)|TargetKeyL` (BE)
  - `Success = (TargetWord >> 15) & 1`
  - `TargetKey = TargetWord & 0x7FFF`

### 3.2) Observação crítica (>=701)
No GS existe bifurcação `GAMESERVER_UPDATE>=701` na struct de envio (index-first vs skill-first).
O **client pinado** usa o layout skill-first acima; NEW-ERA pode:
- manter apenas esse layout no MVP, ou
- aceitar ambos se houver detecção por versão/feature-flag.

---

## 4) Guards normativos (parser NEW-ERA)
- Validar framing: C1 + size==0x09 + head==0x18/0x19.
- Rejeitar se `Size != 9` (ao contrário do legado que não valida dentro do handler).

