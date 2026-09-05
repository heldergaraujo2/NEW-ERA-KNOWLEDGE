# NEW-ERA — PHASE 0E.1-N: Definição e range de ConvertItemType (rede restrita)
> Microteste 0E.1-N · 2026-09-04 · offline + 1 Contents API + 10 raw@580472e (cap consumido SEM parar-cedo — def não encontrada).

## 1. Fontes
58 evidências pré-existentes + 10 novas `NEW_ERA_EVIDENCE/0E1N__01..10__*` (+.SHA256): CSItemOption.cpp 42.284 B · CSItemOption.h 6.569 · ItemAddOptioninfo.cpp/.h 2.361/1.377 · ItemInfo.cpp/.h 888/1.218 · ItemManager.cpp/.h 288/259 · NewUIInventoryCtrl.cpp/.h 44.178/7.417 (hashes nos sidecars; log integral no chat).

## 2. Busca offline (PASSO A)
`ConvertItemType(` nas 58 evidências: **5 ocorrências, TODAS calls** — WSclient.cpp :1951/:5647/:5722/:5887 e ZzzObject.cpp :6112 (`int Type = ConvertItemType(...)`) — **nenhuma definição/protótipo**.

## 3. Rede (PASSO B)
- Contents API: **HTTP 200, 613 itens** (603 code-files).
- Regex `(convert|item|equip|inventory|weapon|shop|trade|drop|pick)`: **39 candidatos**; ordem lexical → **10 baixados (10/10 HTTP 200, ~117 KB), truncagem 29**; parar-cedo NÃO disparou (0 ocorrências do token nos 10).
- **29 truncados** (registrados integralmente no chat): NewUIItem*/LuckyItem/Mix/MyInventory(74 KB)/MyShop/NPCShop/PurchaseShop/SetItem/StorageInventory/Trade · PersonalShopTitleImp.cpp/.h · **ZzzInventory.cpp (333.218 B) / .h** · zzzMixInventory.h.

## 4. Definição encontrada
**NÃO ENCONTRADA** em 68 arquivos de evidência (58+10). A def está (provavelmente) entre os 29 truncados — suspeitos #1: `ZzzInventory.cpp/.h` (lógica de inventário; tamanho e nome coerentes com utilitários de item) e `NewUIMyInventory.cpp` — ou em arquivo cujo NOME não casa a regex (ex.: headers utilitários genéricos).

## 5. Range do retorno
**INDETERMINADO** — sem o corpo, qualquer range seria invenção. Mantém-se a evidência estrutural do §31 (sentinela 0x1FFF em 3 receivers ⇒ domínio esperado 13-bit ⇒ P1/P4 ≤ 9277 < 10002), agora com **força reduzida**: nenhum dos 10 arquivos de item baixados contém sequer o token, sugerindo que o autor do utilitário não é a família CSItem/ItemInfo/ItemManager.

## 6. Impacto no 0E.1-M
P1/P4/P5 permanecem **CONDICIONAIS** (não pioraram nem fecharam); P2/P3 já provados in-bounds (§31). Vetor R→Models[] continua aberto por 1 elo.

## 7. Conclusão classificada: **(3) INCONCLUSIVO — definição não encontrada**
Faltou exatamente: os **29 candidatos truncados pelo cap 10** (ordem lexical colocou CS*/NewUIInventoryCtrl antes de ZzzInventory) + possíveis arquivos fora-da-regex. Custo para fechar: 1 comando com ≤10 raw priorizados (ZzzInventory.cpp/.h, NewUIMyInventory.cpp/.h, NewUIItemMng.cpp, PersonalShopTitleImp.cpp, NewUITrade.cpp, NewUINPCShop.cpp, NewUIMixInventory.cpp) OU regex ampliada em nova listagem.

## 8. Artefatos
Este relatório · 10 evidências + sidecars `0E1N__*` · Ledger §32 · CURRENT_STATE/MASTER_CHECKPOINT.
