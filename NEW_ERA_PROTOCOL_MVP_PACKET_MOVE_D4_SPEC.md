# NEW-ERA — PROTOCOL SPEC — MVP PACKET_MOVE 0xD4 (ReceiveMoveCharacter, wire olc BOTH_MOVE)
> 1.3-K P1/2 · 2026-09-05 · SEM REDE. Status: NORMATIVA — classificação **(1) layout provado** para o wire ATIVO (olc); path clássico C1 0xD4 documentado como legado incoerente neste build.

## 1. Fontes (somente leitura)
| # | Fonte | SHA256 | Papel |
|---|---|---|---|
| C1 | `0E1L__13__WSclient.cpp` | `6538d944…` | handler :1688-:1744; dispatch clássico :13094-:13095 |
| H | `0E1L__14__WSclient.h` | `19d38a3b…` | PMOVE_CHARACTER :611-:620 (**header CONDICIONAL**) · PMOVE_MY_CHARACTER :603-:608 |
| I | `0E1L__21__wsclientinline.h` | `67a6d535…` | `#define PACKET_MOVE 0xD4` :23 |
| P6 | `0E1L__06__ProtocolSend.cpp` | `609e7e935fc88a02…` | **ponte olc :95-:96** `case BOTH_MOVE: ReceiveMoveCharacter((BYTE*)msg.body.data())`; TX SendCharacterMoveNew :223-:284 (`SendPacket(BOTH_MOVE)` :284) · SendPositionNew :212 |
| P7 | `0E1L__07__ProtocolSend.h` | — | `enum class ProtocolHead:uint16_t` :7-:26 ⇒ **BOTH_POSITION=6 (0x0006) · BOTH_MOVE=7 (0x0007)** |
| DG | `0E1JN2__04__Defined_Global.h` | `87b5278655bb53f9…` | **:6 `#define NEW_PROTOCOL_SYSTEM`** (cliente) |

## 2. Descoberta central (por que o wire é olc, não C1/C2)
- Este cliente **define NEW_PROTOCOL_SYSTEM** (DG :6) ⇒ `PMOVE_CHARACTER` compila **SEM PBMSG_HEADER** (H :612-:614 `#ifndef`): body = `[KeyH][KeyL][PositionX][PositionY][Path[1…]]`.
- A ponte olc (P6 :95-:96) chama o handler com **`msg.body.data()`** — combina EXATAMENTE com o struct headerless. ⇒ **Wire RX ativo: frame olc `[id:u16=0x0007][size:u32 LE][body]`** (mesmo framing da família 1.3-C).
- O dispatch clássico `case PACKET_MOVE: ReceiveMoveCharacter(ReceiveBuffer)` (:13094-:13095) passaria o buffer C1 **com header** — com struct headerless seria **misparse** (KeyH=0xC1) ⇒ **legado incoerente neste build; NÃO implementado** (anti-invenção).
- Nuance lateral: `BOTH_POSITION` :93 → `ReceiveMovePosition(body)` — mas `PRECEIVE_MOVE_POSITION` (:892-:898) tem header **incondicional** ⇒ o upstream migrou MOVE (struct headerless) e NÃO migrou POSITION. O parser 0x15 clássico da 1.3-J permanece correto p/ o path clássico.

## 3. Layout (provado) — frame olc, mín. 11 B
| off | Campo | Decode (evidência) |
|---|---|---|
| 0..1 | id = 0x0007 BOTH_MOVE | u16 LE (enum ordinal 7; P7 :7-:26) |
| 2..5 | size = body len (≥5) | u32 LE (ProtocolAsio.h :33-:37) |
| 6/7 | KeyH,KeyL | Key=(KeyH<<8)+KeyL **BE** :1691 — **SEM máscara 0x7FFF** (grep :1688-:1745 vazio) |
| 8/9 | PositionX,Y | :1704-:1705/:1710-:1711 |
| 10 | **Path[0]** | **dir = Path[0]>>4** :1699 (`TargetAngle` nibble) |
| 11.. | Path steps (variável) | **NÃO lidos pelo handler** (só Path[0]); tamanho = size−5; preservados opacos |
- **1 entidade por frame** (sem count); tamanho variável pelo path (size do olc header).
- **angleDeg = ((dir−1)×45)** — CORRELAÇÃO da convenção provada em :2271/:2637 (0x12/0x13); o 0xD4 só seta o nibble TargetAngle :1699 (não converte aqui). Marcada como correlação.

## 4. Aplicação no objeto (evidência :1696-:1741)
- Entidade **não-Hero** (caso do WorldState/viewport): **`TargetX/Y = Data`** :1710-:1711 e `PathFinding2(...)` :1730 interpola o movimento (**x/y NÃO setados** no pacote-apply; dir seta TargetAngle :1699).
- Exceções documentadas (fora do MVP): Hero `!Movement` ⇒ Position=Data :1700-:1707; monstro `MODEL_MONSTER01+52` ⇒ Position=Data + CreateAngle :1712-:1719; dead ⇒ ignora :1696.
- Apply MVP (`ApplyFrame_PacketMoveD4_Asio`): key existente ⇒ `dir/angleDeg` (:1699, correlação *45) + `targetX/Y = X/Y` (:1710-:1711), **x/y inalterados**; key inexistente ⇒ ignora + `missed++`; frame inválido não altera estado.

## 5. Conclusão
**(1) layout provado** p/ o wire ativo (olc BOTH_MOVE 0x0007 + body headerless) — prova tripla: ponte P6 :95-:96 + DG :6 + struct condicional H :611-:620. Restam como correlação documentada: conversão *45 (convenção :2271/:2637). Legado C1 0xD4: incoerente neste build (documentado, não implementado).
