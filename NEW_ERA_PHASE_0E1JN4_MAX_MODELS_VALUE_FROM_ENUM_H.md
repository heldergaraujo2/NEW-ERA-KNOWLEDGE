# NEW-ERA — PHASE 0E.1-JN4: Valor/definição de MAX_MODELS em _enum.h (raw@580472e)
> Microteste 0E.1-JN4 · 2026-09-04 · READ-ONLY · rede = 1 URL raw@580472e (lista fechada) · **fecha a série 0E.1-J (J→JN→JN2→JN3→JN4)**.

## 1. Rede + download
- URL única: `raw.githubusercontent.com/wongddd/muonline/580472e/Source%20Client/source/_enum.h` → **HTTP 200, 56.491 B**, sha256 `7a3eba988f02b19178aee240ad2730615f851def3e83f6c8e0bfb85d3e35ad90`.
- Evidência: `NEW_ERA_EVIDENCE/0E1JN4__enum_h__580472e.h` + `.SHA256.txt`.

## 2. Ocorrências (PASSO B)
| Símbolo | _enum.h | Tipo | Trecho |
|---|---|---|---|
| **MAX_MODELS** | **:1046** | **DEFINIÇÃO (membro de enum anônimo, por expressão)** | `MAX_MODELS = (MODEL_MASK_HELM+MAX_ITEM_INDEX),` |
| MODEL_LOGO | :424 | definição (por expressão) | `MODEL_LOGO = MAX_WORLD_OBJECTS,` (= 160, âncora :422 `MAX_WORLD_OBJECTS = 160`) |
| MODEL_PLAYER | :991 | definição implícita (membro sequencial) | `MODEL_PLAYER ,` → **= 1078** (calculado) |

## 3. Definição de MAX_MODELS (PASSO C)
`_enum.h:1046` — último membro do 2º enum anônimo da região `//object start…//object end` (:320-:1048; enum#1 :321-:883 termina em `MODEL_SKILL_END` :882; enum#2 :885-:1047 ancorado em `MODEL_NPC_BEGIN = MODEL_MONSTER_END` :889).
**Cadeia completa de cálculo** (resolvedor textual próprio: expansão de macros de _define.h + contagem sequencial de membros + #ifdef pela clausura baixada; todos os passos com arquivo:linha):
```
MAX_MODELS       (:1046) = MODEL_MASK_HELM + MAX_ITEM_INDEX(512, _define.h:342)   = 9490+512  = 10002
MODEL_MASK_HELM  (:1045) = MODEL_MULTI_SHOT3 + MAX_QUEST_ITEM(64, _define.h:352)   = 9426+64   = 9490
MODEL_MULTI_SHOT3(:1044) = implícito (MODEL_QUEST+3; :1042-1044)                   = 9423+3    = 9426
MODEL_QUEST      (:1041) = MODEL_EVENT + MAX_EVENT_ITEM(35, _define.h:353)         = 9388+35   = 9423
MODEL_EVENT      (:1040) = MODEL_BODY_BOOTS + MODEL_BODY_NUM(20, _define.h:118)    = 9368+20   = 9388
MODEL_BODY_*     (:1035-1038) = cadeia +MODEL_BODY_NUM(20)×5                       = …+20×5
MODEL_BODY_HELM  (:1033, ramo #else p/ PBG_ADD_NEWCHAR_MONK_ITEM OFF) = MODEL_BOOTS2+MODEL_ITEM_COMMON_NUM(2, _define.h:126) = 9288
MODEL_HELM2..BOOTS2 (:1020-1024) = MODEL_ITEM + MAX_ITEM + 2×4                     = 9278..9286
MODEL_HELM2      (:1020) = MODEL_ITEM + MAX_ITEM(16×512=8192, _define.h:339/:344)  = 1086+8192 = 9278
MODEL_ITEM       (:1001) = implícito (MODEL_SHADOW_STAFF+1)                        = 1085+1    = 1086
MODEL_PLAYER     (:991)  = implícito                                              = 1078
MODEL_NPC_BEGIN  (:889)  = MODEL_MONSTER_END (macros _define.h:112-113:
                           MODEL_SKILL_END+1+MAX_MODEL_MONSTER(400, :110))         = 564+1+400 = 965
MODEL_SKILL_END  (:882)  = último membro implícito do enum#1                       = 564
âncoras duras validadas: MODEL_WORLD_OBJECT=0 · MODEL_LIGHT01=130(:403) · MODEL_FURNITURE01=140(:408) · MODEL_CANDLE=150(:415) · MAX_WORLD_OBJECTS=160(:422) · MODEL_LOGO=160(:424) — TODAS OK
```
Configuração de flags evidenciada (todos os 5 gates OFF): `ASG_ADD_KARUTAN_MONSTERS`/`PBG_ADD_NEWCHAR_MONK_ITEM`/`ASG_ADD_KARUTAN_NPC`/`LEM_ADD_LUCKYITEM`/`PBG_ADD_NEWCHAR_MONK` — **0 `#define` em _define.h/Defined_Global.h/stdafx.h baixados** e ausentes do Main.vcxproj (§24).

## 4. Validações e ressalvas (anti-invenção)
1. **Âncoras duras**: 6/6 OK (0/130/140/150/160/160).
2. **Lattice estrutural 16×512** íntegro: `MODEL_SWORD=MODEL_ITEM+0` → `MODEL_ETC=+15×512`; `MODEL_HELM2=MODEL_ITEM+8192` = logo após o bloco de itens ✓ (`MODEL_AXE=1598`, `MODEL_HELM=4670`…).
3. **Spot-check linha-a-linha** (:424-:448 e :985-:1012): mapeamento fiel (ex.: `MODEL_CARD=164 → MODEL_FACE=168`, `MODEL_FISH01=179 → MODEL_CLOUD=188`).
4. **Geometria da alocação (fecha com §27!)**: `new BMD[MAX_MODELS+1024]` = **11026** slots; janela útil = `Models = ModelsDump + rand()%1024` com `MAX_MODELS=10002` → índice máximo usado 1023+10002−1 = **11024 ≤ 11025** — **encaixe exato** (folga de exatamente 1 slot): os 1024 extras são EXATAMENTE o orçamento do offset anti-tamper. Confirmação estrutural independente do valor 10002.
5. ⚠️ **Ressalva principal — comentário autoral stale**: `:1001 MODEL_ITEM , // (515)`. O valor 515 é **inalcançável em QUALQUER configuração** dos flags in-file: com todos OFF `MODEL_ITEM=1086`; ligar os 4 blocos gated (:+36+25+2+1=+64) dá no máximo 1150 — **515 < mínimo possível** → o comentário é uma anotação histórica de uma versão menor do enum (classificado STALE — inferência forte, não fato).
6. **Sensibilidade**: flags indefinidos na clausura (podem viver em Winmain.h, não baixado) só poderiam **elevar** o valor: variante all-ON parcial = 10082 (com `MAX_CLASS=7`/`MODEL_BODY_NUM=24` reais seria ~10103). Faixa: **MAX_MODELS ∈ [10002, ~10103]**; valor na configuração evidenciada = **10002**.
7. Método: script textual (contagem+avaliação) — sem compilação; assume ramos `#else` de `MAX_CLASS`/`MODEL_BODY_NUM` (correto p/ config evidenciada; documentado); 612 membros computados na região object; zero símbolos não-resolvidos.

## 5. Conclusão classificada: **(1) MAX_MODELS definido em _enum.h com valor resolvido = 10002** (ressalvas 5/6 acima)
- Definição: `_enum.h:1046` (membro de enum anônimo, expressão `(MODEL_MASK_HELM+MAX_ITEM_INDEX)`).
- Impacto: alocação real do cliente = `new BMD[11026]` (~11026 objetos BMD; janela útil 10002, offset aleatório ≤1023); zero-init da janela; teardown itera `MODEL_LOGO(160)..10001`.
- Série 0E.1-J **ENCERRADA**: (a) macro/build-define refutados (§24); (b) storage = ponteiro (§26); (c) alocação (§27); (d) **valor 10002 (§28)**.

## 6. Limitações e próximo passo
- Winmain.h não baixado (flags só podem elevar o valor — bornear [10002, ~10103]).
- Comentário (515) tratado como STALE por impossibilidade aritmética (§4.5).
- Próximo sugerido (derivado, sem rede): **0E.1-K — censo de indexadores de `Models[...]`** (quem indexa, com quais constantes, e se existe bounds-check — §27 mostrou que não existe nos 7; risco §12).

## 7. Artefatos
- Este relatório · `NEW_ERA_EVIDENCE/0E1JN4__enum_h__580472e.h`(+.SHA256) · Ledger §28 · CURRENT_STATE/MASTER_CHECKPOINT atualizados.
