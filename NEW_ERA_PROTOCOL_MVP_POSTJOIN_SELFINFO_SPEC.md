# NEW-ERA — PROTOCOL SPEC — MVP POST-JOIN SELF INFO (F3:0xE0 GCNewCharacterInfoSend)
> 1.3-D P1/2 · 2026-09-05 · SEM REDE (somente NEW_ERA_EVIDENCE/). Status: NORMATIVA PARCIAL — classificação **(2)**: sender GS provado; layout exato e handler cliente **[NOT RECOVERED]** (consumidor provadamente AUSENTE neste client).

## 1. Fontes (somente leitura)
| # | Fonte | Bytes | SHA256 | Papel |
|---|---|---|---|---|
| G1 | `NEW_ERA_EVIDENCE/GS_Protocol.cpp__580472e__sha256_e8bb4dbb.cpp` | 135.014 | `e8bb4dbb…` | sender :3411-:3461 |
| C1 | `NEW_ERA_EVIDENCE/0E1L__13__WSclient.cpp` | 415.884 | `6538d944…` | dispatch F3 :12942-:13040 (censo) |
| H | `NEW_ERA_EVIDENCE/0E1L__14__WSclient.h` | 91.012 | `19d38a3b…` | busca de struct (ausente) |
| — | Árvore legada `muonline/` local | — | — | **INEXISTENTE no workspace** (não consultável) |

## 2. Identificador do pacote (PROVADO no GS)
- **HeadCode 0xF3 · SubCode 0xE0** — `pMsg.header.set(0xF3,0xE0,sizeof(pMsg))` (G1 :3417).
- Direção **S→C unicast**: `DataSend(lpObj->Index,(BYTE*)&pMsg,pMsg.header.size)` (G1 :3458) — DataSend serial (padrão C1 provado em F3:01/F3:03: `sizeof(struct)` = tamanho TOTAL do wire `[C1][size][F3][E0][campos]`; valor do size **[NOT RECOVERED]** sem a struct).
- Guard `#if(GAMESERVER_EXTRA==1)` (:3413) — família EXTRA: irmão `GCNewCharacterCalcSend` :3463 = **F3:0xE1**.
- Call-sites de `GCNewCharacterInfoSend` dentro de G1: **nenhum** (disparo fora da evidência — quando ocorre **[NOT RECOVERED]**).

## 3. Sender — inventário de campos (G1 :3419-:3456, ordem de atribuição)
| # | Campo | Linha | Evidência de tipo |
|---|---|---|---|
| 1-4 | Level · LevelUpPoint · Experience · NextExperience | :3419-:3422 | tipo [NOT RECOVERED] |
| 5-17 | Strength · Dexterity · Vitality · Energy · Leadership · Life · MaxLife · Mana · MaxMana · BP · MaxBP · Shield · MaxShield | :3423-:3435 | via `GET_MAX_WORD_VALUE` (clamp WORD — def da macro [NOT RECOVERED]; alvo **presumido** WORD) |
| 18-21 | FruitAddPoint · MaxFruitAddPoint · FruitSubPoint · MaxFruitSubPoint | :3436-:3439 | tipo [NOT RECOVERED] (gFruit.GetMaxFruitPoint) |
| 22-31 | ViewReset · ViewPoint · ViewCurHP · ViewMaxHP · ViewCurMP · ViewMaxMP · ViewCurBP · ViewMaxBP · ViewCurSD · ViewMaxSD | :3442-:3451 | cast explícito `(DWORD)` |
| 32-36 | ViewStrength · ViewDexterity · ViewVitality · ViewEnergy · ViewLeadership | :3452-:3456 | sem cast (tipo [NOT RECOVERED]) |
- **Offsets/tamanho total: [NOT RECOVERED]** — a definição de `PMSG_NEW_CHARACTER_INFO_SEND` não está na evidência nem no workspace. A ordem acima é a ordem de ATRIBUIÇÃO (correlação forte com a ordem da struct, não prova de layout).

## 4. Cliente — handler PROVADAMENTE AUSENTE neste build
- Sub-switch F3 do cliente (:12958-:13038) censo exaustivo — **24 subcodes**: `00 01 02 03 04 05 06 07 08 10 11 13 14 20 22 23 24 25 30 40 50 51 52` — **0xE0 não existe**.
- `grep 0xE0` em TODA a evidência: apenas o sender (:3417) e um bitmask irrelevante (`Equipment[15]&0xE0`, ZzzCharacter :12194).
- O switch **não tem default** (:13038) ⇒ subs desconhecidos são silenciosamente ignorados por este cliente.
- **Conclusão**: o consumidor de F3:E0 é um client modificado do ecossistema GAMESERVER_EXTRA, **não** o client upstream 580472e em evidência. Handler/Receive* correspondente: **[NOT RECOVERED — inexistente aqui]**.

## 5. Semântica (do que o sender transporta)
- Overlay COMPLETO de estado próprio: level/levelUpPoint/exp(+next), 13 stats/vitais clampados, contadores de frutas (add/sub/max) e 15 valores de janela de visualização (View* HP/MP/BP/SD/reset/point/stats).
- Para **este** client, o alimento pós-join do estado do personagem é o **F3:03 (1.3-C, já provado)** — não este pacote.

## 6. Observação (corrigida por evidência)
A premissa "primeiro pós-join que alimenta o estado do personagem" pertence ao **F3:03** (feito). O F3:E0 é um pacote EXTRA S→C **sem consumidor no client em evidência** — só o lado emissor é recuperável aqui. Viewport/spawn continua pendente (fica para depois, como definido).

## 7. Core desta parte (header-only, anti-invenção)
`ParseC1_F3_E0_SelfInfoResponsePlain` em `mvp_login_client.cpp` (bloco 1.3-D): valida frame C1 + head F3 + sub E0 + consistência de size e expõe o payload como **opaco** (nenhum campo decodificado — layout [NOT RECOVERED]). Sem TX (pacote é S→C). Golden/loopback ficam para quando a struct for recuperada (exigiria fetch permitido de header GS — fora desta parte).

## 8. Classificação: **(2) parcial**
Fechado: identidade (F3:E0 S→C C1), sender completo (campo-a-campo :3419-:3456), DataSend :3458, irmão F3:E1. Aberto: struct/offsets/size, handler cliente (ausente neste build), call-site do disparo.
