# NEW-ERA — PROTOCOL SPEC — MVP POSITION UPDATE (0x15 ReceiveMovePosition)
> 1.3-J P1/2 · 2026-09-05 · SEM REDE. Status: NORMATIVA — classificação **(1) layout provado** (struct + handler completos).

## 1. Fontes (somente leitura)
| # | Fonte | SHA256 | Papel |
|---|---|---|---|
| C1 | `0E1L__13__WSclient.cpp` | `6538d944…` | handler :1746-:1767; dispatch :13097-:13099 |
| H | `0E1L__14__WSclient.h` | `19d38a3b…` | PRECEIVE_MOVE_POSITION :892-:898 · PBMSG_HEADER :76-:81 · PMOVE_CHARACTER :611-:620 |
| I | `0E1L__21__wsclientinline.h` | `67a6d535…` | `#define PACKET_POSITION 0x15` :24 (PACKET_MOVE 0xD4 :23) |

## 2. Pacote escolhido
- **HeadCode 0x15 (PACKET_POSITION), S→C** — `case PACKET_POSITION: ReceiveMovePosition(ReceiveBuffer)` (:13097-:13099). Escolhido como o MAIS SIMPLES update de posição (1 entidade, sem lista/path).
- **Framing C1** — cast `LPPRECEIVE_MOVE_POSITION` (:1748) sobre `{PBMSG_HEADER Header{Code;Size;HeadCode}; KeyH; KeyL; PositionX; PositionY}` (H :892-:898) ⇒ frame **7 B** `[C1][07][0x15][KeyH][KeyL][X][Y]` (tamanho FIXO).

## 3. Layout (provado)
| off | Campo | Decode (evidência) |
|---|---|---|
| 0 | Code=0xC1 | BYTE :78 |
| 1 | size=7 (= total) | BYTE :79 |
| 2 | HeadCode=0x15 | BYTE :80 |
| 3/4 | KeyH,KeyL | Key=(KeyH<<8)+KeyL **BE** :1749 — **SEM máscara 0x7FFF neste handler** (nuance: 0x12/0x13/0x14 mascaram; aqui a lookup `FindCharacterIndex(Key)` usa o valor cru) |
| 5 | PositionX | :1762 |
| 6 | PositionY | :1763 |
- **Sem count/lista** (update único); **sem dir/angle no pacote** (ângulo permanece o do spawn; client seta `JumpTime=1` :1766 e caso especial MODEL_BALL :1753-:1760 — fora do escopo).
- Semântica: `PositionX/Y = Data` :1762-:1763 **e** `TargetX/Y = Data` :1764-:1765 (target = posição recebida).

## 4. Irmão NÃO escolhido (documentado)
- **0xD4 PACKET_MOVE** `ReceiveMoveCharacter` :1688-:1745 — `PMOVE_CHARACTER` (H :611-:620) com **header CONDICIONAL** (`#ifndef NEW_PROTOCOL_SYSTEM PBMSG_HEADER` — no build NOVO o header some do cast!), `Path[1]` variável (caminho), `TargetAngle = Path[0]>>4` :1699, lógica Hero/monstro/Appear. Maior complexidade — fica para parte futura.

## 5. Core desta parte (bloco 1.3-J)
`MoveUpdate{key,x,y}` · `ParsePositionUpdatePlain_C1(frame, vector<MoveUpdate>&, err)` (exige C1 7 B exato/head 0x15; out.size()==1) · `ApplyFrame_PositionUpdate_C1(frame, ws, err, size_t* missed=nullptr)` — parse local; para key **existente**: `x/y` e `targetX/Y` atualizados (=posição; dir/angle **inalterados** — não vêm no pacote); key inexistente: **ignorada sem falhar** (contabilizada em `missed` se ponteiro dado). Frame inválido não altera estado.

## 6. Conclusão
**(1) layout provado** — struct (H :892-:898), handler inteiro (:1746-:1767), define (:24). Ciclo spawn/delete/**move** fechado no papel (move simplificado: teleporte de posição; path-move 0xD4 documentado como pendência).
