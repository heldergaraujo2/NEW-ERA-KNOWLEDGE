# NEW-ERA — PHASE 0C.8-A: Reconciliação F3 (sub-opcodes) cliente↔servidor (offline)
> Microteste 0C.8-A · 2026-09-04 · SEM REDE · fecha a pendência derivada da §23 ("F3 22×23 não reconciliado").

## 1. Fontes
| Lado | Evidência | SHA256 |
|---|---|---|
| Servidor | `NEW_ERA_EVIDENCE/GS_Protocol.cpp__580472e__sha256_e8bb4dbb.cpp` (135.014 B) | `e8bb4dbb28d1baf23222dbc72762e2ebd55b116dbb8d36e5f017dd744e510f13` |
| Cliente | `NEW_ERA_EVIDENCE/0E1L__13__WSclient…cpp` (415.884 B) | `6538d944126181dfa77d4419bf992ac77151eec5411dc3ab243f2926fec868bf` |
| Auxiliar | `0E1L__14__WSclient.h` (structs P*_HEADER_DEFAULT_SUBCODE) | sidecar `0E1L__14…` |

## 2. Servidor — head 0xF3 (GameServer ProtocolCore)
- `case 0xF3:` **:955** → `switch(lpMsg[3])` **:956-:1034** — **22 subcases, SEM default** (consistente com §23: F3=22; default único do dispatch é 0xBF@:656).
- Lista (linha→case): :958 0x00 · :961 0x01 · :964 0x02 · :967 0x03 · :970 0x06 · :973 0x12 · :976 0x15 · :981 0x16 · :986 0x21 · :991 0x30 · :994 0x31 · :999 0x52 · :1004 0xE6 · :1007 0xE7 · :1010 0xE8 · :1013 0xE9 · :1016 0xEA · :1019 0xEB · :1022 0xEC · :1025 0xED · :1028 0xF0 · :1031 0xF1.
- Handlers: todos **CG\*** (direção cliente→servidor): CGCharacterListRecv/CreateRecv/DeleteRecv/InfoRecv (0x00-0x03), CGLevelUpPointRecv (0x06), CGCharacterMoveViewportEnableRecv (0x12), CGCharacterNameCheckRecv/NameChangeRecv (0x15/0x16), CGOptionChangeSkinRecv (0x21), CGOptionDataRecv (0x30), CGClientSecurityBreachRecv (0x31), CGMasterSkillRecv (0x52), CGOffTradeRecv (0xEB), CGItemBuyConfirmRecv (0xED); E6-EA/EC/F0/F1 corpos sem símbolo CG capturado pelo extrator (inline/subsistemas).

## 3. Cliente — head 0xF3 (WSclient.cpp)
- `case 0xF3:` **:12942** → subcode lido por **struct**: C1 → `PHEADER_DEFAULT_SUBCODE.SubCode`; C2/C3 → `PHEADER_DEFAULT_SUBCODE_WORD.SubCode` (:12944-:12953) → `switch(subcode)` **:12958-:13038** — **23 subcases, SEM default** (consistente com §10.3 [MEM]).
- **Mapeamento de bytes (provado, WSclient.h)**: `PBMSG_HEADER={Code;Size;HeadCode}` (3 B) ⇒ SubCode = **byte[3]** para C1 (= exatamente o `lpMsg[3]` do servidor); `PWMSG_HEADER={Code;SizeH;SizeL;HeadCode}` (4 B) ⇒ SubCode = **byte[4]** para C2/C3. *Nota PENDING menor: o servidor lê lpMsg[3] fixo; correspondência exata para pacotes C2 no servidor não verificada (não afeta a reconciliação).*
- Lista (linha→case): :12960 0x00 · :12963 0x01 · :12966 0x02 · :12969 0x03 · :12975 0x04 · :12978 0x10 · :12985 0x05 · :12988 0x06 · :12991 0x07 · :12994 0x08 · :12997 0x11 · :13000 0x13 · :13003 0x14 · :13006 0x20 · :13009 0x22 · :13013 0x24 · :13017 0x25 · :13020 0x23 · :13023 0x30 · :13026 0x40 · :13029 0x50 · :13032 0x51 · :13035 0x52.
- Handlers: todos **Receive\*** (direção servidor→cliente): ReceiveCharacterList/CreateCharacter/DeleteCharacter/JoinMapServer, Revival, Inventory, LevelUp, AddPoint, Damage, PK, MagicList, Equipment, ModifyItem, SummonLife, WT/Soccer, Option, ServerCommand, MasterLevel Exp/Up/GetSkill.

## 4. Reconciliação (D)
| Conjunto | Subcodes | N |
|---|---|---|
| **COMUNS** | 00 01 02 03 06 30 52 | **7** |
| **SOMENTE SERVIDOR** | 12 15 16 21 31 E6 E7 E8 E9 EA EB EC ED F0 F1 | **15** |
| **SOMENTE CLIENTE** | 04 05 07 08 10 11 13 14 20 22 23 24 25 40 50 51 | **16** |
(7+15=22 ✓; 7+16=23 ✓; INCERTOS: 0 — todos literais.)

## 5. Causa do mismatch (com evidência)
**O head 0xF3 é um namespace DUPLEX: cada lado despacha a DIREÇÃO OPOSTA.** O sub-switch do servidor trata REQUESTS do cliente (handlers CG\*Recv); o do cliente trata RESPONSES/PUSHES do servidor (handlers Receive\*). Não se espera espelhamento 1:1:
- **Comuns (7) = pares request↔response que compartilham o subcode** (nomes casam: CGCharacterListRecv↔ReceiveCharacterList; CGCharacterCreateRecv↔ReceiveCreateCharacter; CGCharacterDeleteRecv↔ReceiveDeleteCharacter; CGCharacterInfoRecv↔ReceiveJoinMapServer; CGLevelUpPointRecv↔ReceiveAddPoint; CGOptionDataRecv↔ReceiveOption; CGMasterSkillRecv↔Receive_Master_LevelGetSkill);
- **Somente-servidor (15) = requests sem resposta de mesmo subcode** no cliente deste commit: 0x12 toggle de viewport (sem resposta dedicada), 0x15/0x16 name check/change, 0x21 skin, 0x31 security breach (reporte unidirecional), 0xE6-0xF1 (sistemas server-side: OffTrade 0xEB, ItemBuyConfirm 0xED, etc.);
- **Somente-cliente (16) = pushes/notificações S→C** sem request de mesmo subcode: dano 0x07, PK 0x08, inventário 0x10, magic list 0x11, equipment 0x13, summon 0x20, WT/Soccer 0x22-0x25, ServerCommand 0x40, master-level 0x50/0x51, etc. (requests correspondentes, quando existem, viajam sob outros subcodes/heads).
A divergência "22×23" era **coincidência numérica entre dois conjuntos direcionais distintos** — não anomalia.

## 6. Conclusão classificada: **(1) MISMATCH RESOLVIDO e RECONCILIADO** (causa direcional duplex; 7 pares espelhados; 15 requests sem resposta de mesmo subcode; 16 pushes sem request de mesmo subcode). Pendência da §23 ENCERRADA.

## 7. Artefatos
Este relatório · Ledger §35 · CURRENT_STATE/MASTER_CHECKPOINT. (Nenhuma nova evidência.)
