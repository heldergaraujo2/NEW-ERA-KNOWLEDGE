# NEW-ERA — PROTOCOL SPEC — MVP WORLDSTATE LOOP (0x12 + 0x13)
> 1.3-H P1/2 · 2026-09-05 · SEM REDE. Status: NORMATIVA (agregador sobre parsers já provados §69-§71; nenhum byte novo decodificado).

## 1. Escopo
- Manter **WorldState só de spawn**: upsert de entidades vindas de **C2 0x12** (characters/players/NPCs) e **C2 0x13** (monstros). **Sem move/delete ainda** (0x14 delete, PACKET_MOVE/POSITION virão depois).

## 2. Entradas / Saídas
- **Entradas**: frames C2 *plain* `[C2][szH][szL][head][count][entidades…]` — head 0x12 ou 0x13 (validação completa delegada aos parsers provados: `ParseViewportCharacterSpawnPlain_C2` / `ParseC2_ViewportMonsterSpawnPlain`).
- **Saída**: `WorldState.entities` — `unordered_map<key:uint16, EntityRecord>` com `kind` (Character/Monster), `x/y`, `targetX/Y`, `dir`, `angleDeg`, `pk` e `id[11]` (só Character), `classByte` (Character) / `type` (Monster), `buffs` (cópia). Counters derivados (`countByKind`) — sem cache (consistencia).

## 3. Regras
- **Last-write-wins**: key repetida no mesmo frame ou em frames sucessivos **sobrescreve** o registro (upsert `entities[key] = r`). Mesmo key com kind diferente também sobrescreve (nenhuma regra de imutabilidade de kind evidenciada).
- **Frame inválido NÃO altera estado**: parse acontece em vetor local; só após `parse OK` o mapa é atualizado. Erro ⇒ `return false` + `err` do parser (prefixos `0x12:`/`0x13:`), estado intocado.
- **key==0 permitida**: range provado é 0..0x7FFF (máscara :2177/:2605); nenhuma proibição de key 0 evidenciada nos handlers — não se assume bloqueio.
- Buffs armazenados por CÓPIA (fonte: vetor do parser); equipment[17] NÃO entra no WorldState (opaco, render-layer :2320).

## 4. Core desta parte (bloco 1.3-H)
`enum class EntityKind` · `struct EntityRecord` · `struct WorldState { unordered_map; countByKind; clear; }` · `ApplyFrame_C2_12_Characters(frame, ws, err)` · `ApplyFrame_C2_13_Monsters(frame, ws, err)` — ambos wrapper-apply (parse local → upsert LWW). Sem TX; sem rede; sem render.

## 5. Próximo (fora do escopo)
- 0x14 delete viewport (:13116-:13119), PACKET_MOVE/PACKET_POSITION (:13094-:13099) para um tick completo de mundo.
