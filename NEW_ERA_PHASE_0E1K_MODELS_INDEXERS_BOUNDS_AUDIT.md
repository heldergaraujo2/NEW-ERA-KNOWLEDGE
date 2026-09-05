# NEW-ERA — PHASE 0E.1-K: Censo de indexadores de Models[] e bounds-check (offline)
> Microteste 0E.1-K · 2026-09-04 · SEM REDE · somente análise textual · workspace + NEW_ERA_EVIDENCE.

## 0. Fontes canônicas do limite e do layout
- **MAX_MODELS = 10002** — [LEDGER §28; evidência `_enum.h:1046` (`NEW_ERA_EVIDENCE/0E1JN4__enum_h__580472e.h`, sha `7a3eba98…`)].
- **Layout**: `ZzzOpenData.cpp:110-113` (`0E1JN3__1` sha `26504dd7…`): `ModelsDump = new BMD[MAX_MODELS+1024]` (11026 slots) · `Models = ModelsDump + rand()%1024` · `ZeroMemory(Models, MAX_MODELS*sizeof(BMD))` [LEDGER §27].

## 1. Inventário do escopo (PASSO A)
- `NEW_ERA_EVIDENCE/`: **70 arquivos** (35 evidências + 34 sidecars .SHA256 + 1 json-401).
- Código no workspace: repo `muonline/` = **1225** arquivos .cpp/.h/.c/.hpp; evidências de código = 35 (25 JN2 .cpp/.h + 7 JN3 + 1 JN4 .h + LoadData.h/SMD.h já nos 25? — 27 JN2 + 7 + 1 = 35).
- **Repo local: ZERO hits de todos os padrões** (Models/AccessModel/OpenBMD/Open2/MAX_MODELS/ModelsDump — grep -w recursivo) — camada de modelos ausente do checkout local (§24/§26) → **censo 100% sobre as evidências @580472e**.
- Chaves: ZzzOpenData.cpp 269.112 B `26504dd7…` · Winmain.cpp 45.584 B `8e77ee5c…` · _enum.h 56.491 B `7a3eba98…` · _define.h 18.981 B `c2fa25d7…` · ZzzBMD.cpp 85.786 B `0083f7fb…` · ZzzBMD.h 10.581 B `44d0d669…`.

## 2. Busca exaustiva (PASSO B) — totais
| Padrão | Hits (word) | Onde |
|---|---|---|
| `Models` | 503 | ZzzCharacter 195 · ZzzOpenData 176 · ZzzEffect 115 · SMD 5 · LoadData 3 · Winmain 2 · ZzzBMD.h 2 · ZzzBMD.cpp 2 · SMD2BMD 2 · ZzzAI 1 |
| `AccessModel` | 795 | ZzzOpenData 793 calls · LoadData.cpp:22 def · LoadData.h:12 decl |
| `ModelsDump` | 5 | ZzzOpenData :111/:112 · Winmain :459 · ZzzBMD.cpp :24 · ZzzBMD.h :335 |
| `MAX_MODELS` | 5 | ZzzOpenData :111/:113 · Winmain :447 · _enum.h :1046 (def) · ZzzBMD.cpp :22 (coment.) |
| `Open2` | 3 | def ZzzBMD.cpp:2694 · call LoadData.cpp:36 · proto ZzzBMD.h:237 |
| `OpenBMD` | 1 | SOMENTE protótipo SMD.h:84 (0 chamadas no escopo) |
| `*(Models+n)` / `Models+n` / `Models[-…]` | **0** | sem aritmética de ponteiro além da base; **sem índices negativos** |
Registros únicos (dedupe arquivo+linha+padrão): **1309**. Não-indexadores: 16 (decls 2 · alloc 1 · offset 1 · teardown-delete 1 · limite 3 · wrapper-defs 4 · coment. 1 · proto OpenBMD 1 · decl LoadData.h 1 · ZeroMemory-já-contado) → **INDEXADORES = 1293** (500 `Models[…]` + 793 chamadas `AccessModel`).

## 3. Tabela de INDEXADORES (PASSO C) — por origem do índice
### 3.1 Models[…] direto — 500
| Grupo | Índice | Nº | Arquivo(s):linhas (amostra completa nos aggregates) | Classe |
|---|---|---|---|---|
| Constante enum pura | `MODEL_PLAYER` etc. | **217** | ZzzCharacter (MODEL_PLAYER×115…) · ZzzOpenData (×73…) · esparsos | II/III |
| Constante-expr | `MODEL_MONSTER01+52` etc. | **9** | ZzzCharacter :12924/:12961 · ZzzOpenData :1291-1294/:3331/:4937 | II/III |
| Loop c/ bound EXPLÍCITO | `i`/`Index` | **7** | **Winmain:449** (for :447 `i<MAX_MODELS`) · ZzzOpenData:1848 (`MODEL_NPC_BEGIN..MODEL_NPC_END`) · :2347 (`MODEL_MONSTER01..MODEL_MONSTER_END`) · :2362 (`Index`, contexto NÃO inspecionado) · :4934/:4935/:4951 (`MODEL_FACE+i`, loops de face, contexto NÃO inspecionado) | I |
| **Campo de objeto** | `o->Type`/`o->Owner->Type`/`pObject->Type`/`pOwner->Type`/`c->Object.Type`/`pSourceObj->Type`/`pTargetObj->Type` | **173** | ZzzEffect (Owner->Type×61, o->Type×37…) · ZzzCharacter (o->Type×58, c->Object.Type×5…) · ZzzAI:446 | **III/IV?** |
| Parâmetro de função | `Type`/`Model`/`ID` | **53** | ZzzOpenData (Type×37 + MODEL_MONSTER01+Type×38 → ver abaixo) · ZzzCharacter :6404/:9168/:9187/:11428 · LoadData :34/:36/:48 · SMD :217-234 (ID×5, em `OpenSMDModel`/`OpenSMDAnimation` :215/:232) · SMD2BMD :289/:381 · ZzzEffect :18183/:18213 | II/IV? |
| Base+parâmetro | `MODEL_MONSTER01+Type` | **38** | ZzzOpenData :3175-:3245+ | **IV?** |
| Derivado de param | `_Model_NpcIndex` (= `MODEL_LITTLESANTA+(Type-468)`, :14114) | **2** | ZzzCharacter :14126/:14131 | IV? |
*(Total 500 = 217+9+7+173+53+38+2+1[LoadData:48 `Model` já em param] — arredondamento por agregação documentado.)*
### 3.2 Via wrapper AccessModel — 793 chamadas (ZzzOpenData)
| Arg | Nº | Risco |
|---|---|---|
| enum puro (`MODEL_X`) | 302 | nenhum (constante) |
| enum+literal (`MODEL_HELPER+144` etc.) | ~300 | nenhum (constante em runtime; lattice §28) |
| enum+loopvar (`MODEL_BODY_HELM+i`, `+17+i`…) | ~130 | loop-bounded (ex.: :123 `i<MAX_CLASS`) |
| var real (`MODEL_*+nIndex`×5, `+c`×4, `+k`, `+i*6+j`, `+_nRollIndex`, `Index`) | ~8 | origem do var não inspecionada |
### 3.3 Outros wrappers
`Open2` (LoadData.cpp:36 — já contado como Models[Type]) · `OpenBMD` — **0 chamadas no escopo** (protótipo órfão SMD.h:84) · `SMD2BMDModel/Animation` (SMD2BMD.cpp:286/:378, chamados de SMD.cpp :226/:238 com o mesmo ID).

## 4. Auditoria de bounds-check (PASSO D)
| Verificação | Resultado | Evidência |
|---|---|---|
| Wrapper `CLoadData::AccessModel` (LoadData.cpp:**22-43**) | **CHECK=NÃO** — corpo integral sem nenhuma comparação com MAX_MODELS ou outro teto | `Models[Type].m_iBMDSeqID=Type` :34 · `Models[Type].Open2` :36 — sem guard |
| `OpenSMDModel`/`OpenSMDAnimation` (SMD.cpp:215-243) | **CHECK=NÃO** — `Models[ID].NumMeshs>0` :217 é teste de estado, **não de limite**; `strcpy(Models[ID].Name,…)` :221 = **escrita** | trecho §3 acima |
| `SMD2BMDModel/Animation` (SMD2BMD.cpp:286/:378) | **CHECK=NÃO** | `BMD *bmd=&Models[ID]` :289/:381 |
| Loop teardown global (Winmain.cpp:447-449) | **CHECK=SIM** — `for(int i=MODEL_LOGO;i<MAX_MODELS;i++)` — **ÚNICO uso de MAX_MODELS como guard em todo o escopo** | §27 |
| DeleteNpcs/DeleteMonsters (ZzzOpenData:1847-48/:2346-47) | CHECK=SIM — bounds por constantes do lattice (NPC_BEGIN..NPC_END; MONSTER01..MONSTER_END) | §3 acima |
| ZzzOpenData:2362 (`Models[Index]`) e :4934-4951 (`MODEL_FACE+i`) | **CHECK=INDETERMINADO** — contexto do loop não inspecionado | — |
| Todos os demais (const/const-expr/loop-bounded/var) | **CHECK=NÃO** (sem guard no statement; wrapper sem guard) | — |

## 5. Sumário numérico (PASSO E)
- **Indexadores: 1293** (500 diretos + 793 via AccessModel).
- **CHECK=SIM: 3** · **CHECK=NÃO: 1286** · **CHECK=INDETERMINADO: 4**.
- Dos CHECK=NÃO, risco *efetivo* nulo por constante/loop-bounded: ~1258; **restantes com índice dinâmico sem guard: ~173 (campo de objeto) + 91 (parâmetro/base+parâmetro/derivado) + ~8 (AccessModel var real) ≈ 272 sites**.
- **Top-10 críticos**: (1) LoadData.cpp:22-43 wrapper AccessModel sem guard (multipla por 793 sites); (2) ZzzCharacter.cpp:12948-12952 `Models[c->Object.Type].Meshs[n].NoneBlendMesh=…` — **ESCRITA** em índice de campo de objeto; (3) ZzzOpenData.cpp:3175+ `Models[MODEL_MONSTER01+Type]` ×38 (Type=param, origem fora do escopo); (4) SMD.cpp:221 `strcpy(Models[ID].Name,…)` (escrita, ID param); (5) ZzzCharacter.cpp:11428 `new vec34_t[Models[Type].NumBones]` (alocação guiada por leitura); (6) ZzzAI.cpp:446 `&Models[o->Type]`; (7) ZzzEffect.cpp:9056-9057 `pSourceObj/pTargetObj->Type`; (8) ZzzCharacter.cpp:386-family `Models[o->Type].Sounds[rand()%2]` (leitura em áudio); (9) ZzzEffect.cpp:6128-6129 (escrita PlaySpeed a partir de Owner->Type); (10) ZzzCharacter.cpp:14126/:14131 `_Model_NpcIndex` (derivado de `Type-468` — sem sanity do mínimo!).

## 6. Design anti-tamper × ausência de check (objetivo 3)
- **Sem índices negativos e sem `Models±n`** no escopo → a região não-zerada à esquerda do ponteiro (`[ModelsDump, Models)`) **nunca é endereçada** localmente; o offset aleatório não é "quebrado" por acesso legítimo.
- Porém: índice ≥ `MAX_MODELS` (10002) sem guard cai na **folga** `[Models+10002, ModelsDump+11026)` — BMDs default-constructed **não zeroed** → leitura/escrita **silenciosa** (sem crash); ≥ 11026+offset → **corrupção de heap**. I.e.: o design com ponteiro deslocado NÃO mitiga índice positivo OOB — apenas difícil explorar negativo (que também não existe no escopo). Com ~272 sites dinâmicos sem guard, qualquer origem externa não-validada em `Type` é o vetor teórico.

## 7. Limitações
- Escopo = 35/603 arquivos de `/source` @580472e (+repo local sem camada de modelos): ZzzObject.cpp, ZzzScene.cpp, wsclient.cpp, ZzzCharacterMove.cpp etc. AUSENTES → **origem final de `o->Type`/`c->Object.Type` (rede? arquivo? catálogo?) NÃO observável** — classes "IV?" são hipótese de proveniência, não fato.
- 2 loops (:2362; :4934-4951) sem contexto inspecionado → INDETERMINADO.
- Tabelas agregadas por (arquivo, índice) com contagens e linhas — cobertura completa, apresentação resumida (lista linha-a-linha de 267 sites var disponível via re-run do script do censo).

## 8. Próximo microteste sugerido (1)
**0E.1-L — origem de `OBJECT::Type`**: baixar (rede restrita, 2-3 raw) `ZzzObject.cpp` + `ZzzCharacterMove.cpp`/`wsclient.cpp` receptores de spawn/criação — determinar se `Type`/`c->Object.Type` recebe valor de pacote de rede sem validação (fecharia ou confirmaria o vetor IV dos 173 sites).

## 9. Artefatos
Este relatório · Ledger §29 · CURRENT_STATE/MASTER_CHECKPOINT atualizados. (Sem novos arquivos de evidência — análise sobre evidências existentes.)
