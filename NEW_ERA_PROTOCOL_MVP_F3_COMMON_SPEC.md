# NEW-ERA — PROTOCOL SPEC — MVP PÓS-LOGIN: 0xF3 COMUNS (00/01/02/03/06/30/52)
> 1.1-A · 2026-09-05 · SEM REDE · base: §35 (reconciliação F3) + extração fresca desta spec. Status: NORMATIVA para o próximo MVP.

## 1. Fontes
| # | Fonte (NEW_ERA_EVIDENCE/) | Bytes | SHA256 |
|---|---|---|---|
| G1 | `GS_Protocol.cpp__580472e…cpp` | 135.014 | `e8bb4dbb28d1baf23222dbc72762e2ebd55b116dbb8d36e5f017dd744e510f13` |
| C1 | `0E1L__13__WSclient…cpp` | 415.884 | `6538d944126181dfa77d4419bf992ac77151eec5411dc3ab243f2926fec868bf` |
| H | `0E1L__14__WSclient.h` (structs) | 91.012 | `19d38a3b6506a745662c14dd7cdd6e41ca3ad29b28b17c76f860cd9e67799c10` |

## 2. Servidor (C→S requests) — dispatch :955 → switch(lpMsg[3]) :956-:1034
| Sub | case | Handler | Cast/struct | Notas |
|---|---|---|---|---|
| 0x00 | :958 | `CGCharacterListRecv(aIndex)` | **nenhum** (só aIndex) | request SEM payload |
| 0x01 | :961 | `CGCharacterCreateRecv` | `(PMSG_CHARACTER_CREATE_RECV*)` | def [NOT RECOVERED] |
| 0x02 | :964 | `CGCharacterDeleteRecv` | `(PMSG_CHARACTER_DELETE_RECV*)` | def [NOT RECOVERED] |
| 0x03 | :967 | `CGCharacterInfoRecv` | `(PMSG_CHARACTER_INFO_RECV*)` | def [NOT RECOVERED] (join/map-server) |
| 0x06 | :970 | `CGLevelUpPointRecv` | `(PMSG_LEVEL_UP_POINT_RECV*)` | def [NOT RECOVERED] |
| 0x30 | :991 | `CGOptionDataRecv` | `(PMSG_OPTION_DATA_RECV*)` | def [NOT RECOVERED] |
| 0x52 | :999 | `gMasterSkillTree.CGMasterSkillRecv` (:1001) | `(PMSG_MASTER_SKILL_RECV*)` | def [NOT RECOVERED] |

## 3. Cliente (S→C responses) — case 0xF3 :12942 → switch(SubCode) :12958-:13038
| Sub | case | Handler (def) | Cast/struct | Notas |
|---|---|---|---|---|
| 0x00 | :12960 | `ReceiveCharacterList` (:523) | `LPPHEADER_DEFAULT_CHARACTER_LIST` | lista variável além da struct (ver §5) |
| 0x01 | :12963 | `ReceiveCreateCharacter` (:622) | `LPPRECEIVE_CREATE_CHARACTER` | |
| 0x02 | :12966 | `ReceiveDeleteCharacter` (:672) | `LPPHEADER_DEFAULT_SUBCODE` | só header+sub+value (5 B) |
| 0x03 | :12970 | `ReceiveJoinMapServer` (:871) | inline (sem cast único) | retorna FALSE → falha de decode |
| 0x06 | :12988 | `ReceiveAddPoint` (:6200) | `LPPRECEIVE_ADD_POINT` | |
| 0x30 | :13023 | `ReceiveOption` (:9388) | `LPPRECEIVE_OPTION` | |
| 0x52 | :13035 | `Receive_Master_LevelGetSkill` (:7387) | `LPPMSG_ANS_MASTERLEVEL_SKILL` | |

## 4. Direcionalidade
**COMUNS ≠ mesmo pacote**: 0xF3 é namespace duplex (§35). Cada subcode comum aparece **duas vezes, em direções opostas**: request C→S no GS (`CG*Recv`) e response S→C no cliente (`Receive*`), com structs e layouts **independentes** (ex.: 0x00 = request vazio ↔ response com lista de personagens; 0x03 = info do char ↔ join-map-server). O pareamento semântico request→response é por fluxo, não por bytes.

## 5. Structs encontradas (WSclient.h; todas C1 ⇒ PBMSG_HEADER 3 B + SubCode)
| Struct | Linhas | Layout (após 3B+Sub) | Tam ≈ |
|---|---|---|---|
| `PHEADER_DEFAULT_CHARACTER_LIST` | :137-:146 | `MaxClass; MoveCount; Value` (Header+Sub condicionais a NEW_PROTOCOL_SYSTEM) | **7 B** |
| `PRECEIVE_CREATE_CHARACTER` | :376-:386 | `Result; ID[MAX_ID_SIZE=10 inf.]; Index; Level(WORD); Class` (Equipment[24] comentado) | **19 B** |
| `PHEADER_DEFAULT_SUBCODE` | :129-:134 | `Value` (canônica §38) | **5 B** |
| `PRECEIVE_ADD_POINT` | :883-:890 | `Result; Max(W); ShieldMax(W); SkillManaMax(W)` | **11 B** |
| `PRECEIVE_OPTION` | :1205-:1215 (pack(1)) | `HotKey[20]; GameOption; KeyQWE[3]; ChatLogBox; KeyR; QWERLevel(int)` | **34 B** |
| `PMSG_ANS_MASTERLEVEL_SKILL` | :2860-:2868 | `btResult; nMLPoint(short); nSkillNum(int); nSkillLevel(int)` | **15 B** |
**Faltantes**: todas as `PMSG_*_RECV` server-side [NOT RECOVERED — headers GS fora do escopo]; payload VARIÁVEL da character-list (slots por personagem, lido no corpo de `ReceiveCharacterList` :523+) — **a extrair na 1.1-B**.

## 6. Escolha do próximo subcode: **0x00 — CHARACTER LIST**
- **Central no fluxo**: é o 1º pacote pós-login-OK (GS dispara `CGCharacterListRecv` na conexão; response habilita a tela de seleção de personagem — gateway de todo o resto).
- **Request C→S sem payload** (handler usa só `aIndex`, :958-:959): zero structs faltantes no lado client→server — implementável imediatamente sobre o stack já provado (C1/C3, serial, chaves §48).
- **Response com struct presente e pequena** (7 B de header de lista); única lacuna é o layout variável dos slots — delimitado e extraível na própria evidência (`ReceiveCharacterList` :523), sem rede.
Runner-up: 0x02 delete (usa `PHEADER_DEFAULT_SUBCODE` 5 B, já canônica) — porém menos central.
