# NEW_ERA_PROTOCOL_MVP_BOTH_ATTACK_TX_SPEC.md
Data: 2026-09-05
Status: NORMATIVO (Fase 1 — 1.3-R P2)

## Objetivo
Especificar o TX moderno (olc/ASIO) para:
- `BOTH_ATTACK1` (ProtocolHead `0x0008`) — ataque normal
- `BOTH_ATTACK2` (ProtocolHead `0x0009`) — skill attack (dialeto moderno, não igual ao C1 0xDB clássico)

Base de evidência: `EVIDENCE/1.3-R/NEW_ERA_1_3_R_BOTH_ATTACK1_2_BODIES_EVIDENCE.md` (commit governança: fd987c2+).

---

## 1) Framing olc/ASIO (message_header)
Header **6 bytes** (raw struct, endian LE):
- `id: u16`  (ProtocolHead)  → bytes `[id_lo][id_hi]`
- `size: u32` (tamanho do body) → bytes `[b0][b1][b2][b3]`

Sem `hton/ntoh` (dependente de LE, como no legado).

**Frame on-wire = header(6) + body(size).**

---

## 2) BOTH_ATTACK1 — id 0x0008
### 2.1) Body (7 bytes)
Body interpreta diretamente a struct `PMSG_ATTACK_RECV` (PBMSG head embutido, mas ignorado pelo handler).

Layout:
- `[hdr3]` = 3 bytes (PBMSG_HEAD, opaco)
- `indexH` (u8)
- `indexL` (u8)  → `index = MAKE_NUMBERW(indexH,indexL)` (**BE**)
- `action` (u8)  → esperado `0x78` (AT_ATTACK1)
- `dir` (u8)

Recomendação para `hdr3` (canônico):
- `C1 07 11` (equivale ao pacote clássico `C1 07 11 ...`)

### 2.2) Frame total
- header: `id=0x0008`, `size=7`
- total: 13 bytes

---

## 3) BOTH_ATTACK2 — id 0x0009
### 3.1) Body (9 bytes, branch >=701)
Body interpreta diretamente a struct `PMSG_MULTI_SKILL_ATTACK_RECV` (PBMSG head embutido, ignorado pelo handler).

Layout:
- `[hdr3]` = 3 bytes (PBMSG_HEAD, opaco)
- `skillH` (u8)
- `count` (u8)
- `skillL` (u8) → `skill = MAKE_NUMBERW(skillH,skillL)` (**BE**) com layout **intercalado**
- `x` (u8)
- `serial` (u8)
- `y` (u8)

**Importante:** este layout **não** é igual ao C1 clássico `0xDB` (que é `[TypeH][TypeL][x][y][Serial][Count]` + targets). Aqui a ordem é remapeada.

Recomendação para `hdr3` (canônico, por evidência/intent):
- `C3 09 DB` (PBMSG head “C3” embutido; handler ignora)

### 3.2) Frame total
- header: `id=0x0009`, `size=9`
- total: 15 bytes

---

## 4) Guards normativos (builder NEW-ERA)
- BOTH_ATTACK1: `dir` em `0..7` (se aplicável ao seu modelo); `index` ≠ 0.
- BOTH_ATTACK2: `count >= 1` (count==0 deve falhar localmente — não enviar frame).
- Serial: `1..50` (se usar `MakeSkillSerialNumber` compatível), wrap 50→1.

## 5) Compatibilidade
Manter suporte ao túnel `BOTH_MESSAGE (0x000C)` em paralelo, enquanto ambos coexistirem no MVP.
