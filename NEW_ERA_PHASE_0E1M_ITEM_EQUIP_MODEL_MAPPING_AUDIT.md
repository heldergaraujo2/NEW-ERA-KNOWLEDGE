# NEW-ERA — PHASE 0E.1-M: Rede→Item/Equip→Model ID mapping (offline, evidências)
> Microteste 0E.1-M · 2026-09-04 · SEM REDE · somente evidências @580472e já no workspace · fecha o "último vetor plausível" R→Models[] iniciado no §30.

## 1. Fontes (sha lidos dos sidecars)
| Evidência | Bytes | SHA256 |
|---|---|---|
| `0E1L__13__WSclient…cpp` | 415.884 | `6538d944126181dfa77d4419bf992ac77151eec5411dc3ab243f2926fec868bf` |
| `0E1JN2__22__ZzzCharacter…cpp` | 462.313 | `02d3f539860e526e469c6f3c1c825741538a17516ba51bf78b1e47b9db581e86` |
| `0E1L__16__ZzzObject…cpp` | 378.840 | `2974f5bb48a983400cd9f1eee638ee4b32bc5f16da7a08a1203c26b2504ad8ed` |
| `0E1JN3__4___define.h` | 18.981 | `c2fa25d74def49ff12b76b5918e3108bb14e99b04a8bf47c77eefb84a3f7204f` |
| `0E1JN4__enum_h` | 56.491 | `7a3eba988f02b19178aee240ad2730615f851def3e83f6c8e0bfb85d3e35ad90` |

## 2. Handlers relevantes (Passo B — 40 localizados; 12 em detalhe)
Equipment/ChangePlayer: `ReceiveEquipment` :1937 (sub-switch interna, sub-case 0x13 — head pai não isolado, PENDING) · `ReceiveChangePlayer` :1944 (**opcode 0x25** "change character" :13141) · Ground/trade/shop: `ReceiveCreateItemViewport` :5631 (**0x20**, log :5657) · `ReceiveGetItem` :5680 (**0x22**) · `ReceiveDropItem` :5736 (**0x23**) · `ReceiveModifyItem` :5874 (sub-case 0x14) · `ReceiveBuy`/:6076 (**0x32**) · `ReceiveSell` :6105 (**0x33**) · `ReceiveTradeYourInventory` :6490 (**0x39**) · `ReceivePersonalShopItemList` :8617 / `ReceivePurchaseItem` :8724 / `ReceiveRefreshItemList` :8691 (sub-switch interna 0x05/0x06) · Viewports (já §30): 0x12/0x13/0x1F/0x45.

## 3. Caminhos rede→model id (C/D/E)
| # | Caminho | Fórmula do model id | Origem | Bound | <10002? | Sanitização |
|---|---|---|---|---|---|---|
| P1 | `ReceiveChangePlayer` 0x25 → Weapon[0/1]/BodyPart[HELM..GLOVES]/Helper.Type (WSclient :1959+/:2140) | `MODEL_ITEM + Type`, `Type = ConvertItemType(Data->Item)` :1951; **sentinela `Type==0x1FFF → -1` por case** | (C) | Type ≤ 0x1FFE ⇒ ≤ **9276** | **SIM se ConvertItemType ≤13 bit** (def AUSENTE) | ESTRUTURAL (sentinela) |
| P2 | `ReceiveEquipment` → `ChangeCharacterExt` (ZzzCharacter :12005) → Weapon[0].Type :12034 / Weapon[1].Type :12063 | `MODEL_SWORD + ExtType`; ExtType = `(Equipment[12]&240)<<4 \| Equipment[1]` (:12036-38) — **12 bit**; sentinela `0x0FFF→-1` :12046 | (C) | ≤1086+4094=**5180** | **SIM (provado)** | ESTRUTURAL (bit-width+sentinela) |
| P3 | `ChangeCharacterExt` → Wing.Type :12068+ / Helper.Type :12173+ | Wing: seletor `(Equipment[4]>>2)&3` + `Equipment[8]&0x07` → `MODEL_WING+{2..49}` ; Helper: `Equipment[4]&3`/`[9]&1`/`[15]&0xE0` → `MODEL_HELPER+{3,4,37,_type}` | (C) | ≤ `MODEL_HELPER(7742)+255`=**7997** | **SIM (provado)** | ESTRUTURAL (máscaras 1-3 bit) |
| P4 | `ReceiveCreateItemViewport` 0x20 → `CreateItem` (ZzzObject :6110) → **`o->Type = MODEL_SWORD+Type` :6145** | `Type = ConvertItemType(Item)` :6111 — **SEM sentinela neste caminho** | (C) | idem P1 ⇒ ≤9277 (13-bit) | **SIM se ConvertItemType ≤13 bit** (def AUSENTE) | ESTRUTURAL-CONDICIONAL — **alimenta os ~95 indexadores `o->Type` do §29** |
| P5 | `SetCharacterClass` (ZzzCharacter :11726) — local, pós-inventory | `p[EQUIPMENT_*].Type + MODEL_ITEM` :11741+ (p=CharacterMachine->Equipment) | (C) indireto | domínio de Type dos itens (idem ConvertItemType) | idem P1 | ESTRUTURAL herdados |
| P6 | `ReceiveGetItem` 0x22 | `Data->Result` indexa inventário: **check EXPLÍCITO** `if(Result>=MAX_EQUIPMENT_INDEX && Result<MAX_MY_INVENTORY_INDEX)` :5703-5705 | (R) | — | — | **EXPLÍCITA** |
| P7 | `ReceiveCreateItemViewport` 0x20 (Key) | `if(Key<0 \|\| Key>=MAX_ITEMS) Key=0` :5644-5645 — clamp EXPLÍCITO antes de `Items[Key]` | (R) | — | — | **EXPLÍCITA** (raro no cliente!) |
| P8 | Viewports 0x12/0x13/0x1F/0x45 (§30) | default-substitution em `CreateMonster` | (R) | — | — | ESTRUTURAL (default) |

## 4. Constantes resolvidas (D)
`MODEL_ITEM=1086` · `MODEL_SWORD=1086` · `MODEL_WING=1086+12×512=7230` · `MODEL_HELPER=1086+13×512=7742` (:1003-1016 _enum.h; MAX_ITEM_INDEX=512, MAX_ITEM_TYPE=16, MAX_ITEM=8192 _define.h:339-344) · **Coerência estrutural do lattice**: `MODEL_HELM2 = MODEL_ITEM+8192` (:1020) = exatamente o fim do bloco de itens ⇒ **o enum foi desenhado para acomodar TODOS os ids de item (16 grupos×512) após MODEL_ITEM** — evidência de design forte de que `MODEL_ITEM+Type` (Type<8192) sempre cai no bloco de itens.

## 5. Top riscos
1. **P4/CreateItem :6145** — `o->Type = MODEL_SWORD+Type` **sem sentinela** e sem check; se `ConvertItemType` produzir >13 bit (lê até 5 bytes), OOB direto nos indexadores `o->Type` (§29: ~95 sites de render) → heap-corruption ≥11026+offset;
2. **P1/0x25** — `MODEL_ITEM+Type` idem (sentinela mitiga o caso "vazio", não largura);
3. **ConvertItemType def AUSENTE do escopo** (só 6 call-sites em WSclient/ZzzObject) — o bound de P1/P4/P5 inteiro depende dela;
4. P5 (`SetCharacterClass`) sem check, domínio herdado;
5. (menor) `Items[ItemKey]` em ReceiveGetItem :5712 usa `ItemKey` global cujo set não auditado aqui.

## 6. Conclusão classificada: **(3) INCONCLUSIVO por falta de 1 arquivo — a definição de `ConvertItemType`**
- **Provado in-bounds (P2, P3)**: máscaras 12-bit/1-3-bit com sentinelas — impossível OOB.
- **P1/P4/P5 estruturalmente in-bounds SE E SOMENTE SE `ConvertItemType` retornar ≤ 0x1FFE** — evidência indireta FORTÍSSIMA de 13-bit (sentinela 0x1FFF em 3 receivers; lattice `MODEL_HELM2=MODEL_ITEM+MAX_ITEM` projetado para o domínio 16×512), mas a **def não está em nenhuma das 58 evidências** (6 call-sites; declaração em header não baixado — candidatos no repo: headers de item/CSItemOption.h etc.).
- Exatamente o que falta: **1 raw download futuro** do header que define `ConvertItemType` (fecharia P1/P4/P5 ⇒ classificação (1) esperada).

## 7. Próximo sugerido (1)
**0E.1-N (rede mínima, 1-2 raw)**: localizar e baixar a def de `ConvertItemType` (via Contents API listing já conhecido — arquivos com "Item" no nome; fallback: csclientinline/WSclient.h incluídos) e fechar o bound ⇒ promover a classificação para (1) ou (2).

## 8. Artefatos
Este relatório · Ledger §31 · CURRENT_STATE/MASTER_CHECKPOINT. (Nenhuma nova evidência — offline.)
