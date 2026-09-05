# NEW-ERA — PHASE 0E.1-JN2: MAX_MODELS via Contents API (ref=580472e) + raw@580472e (sem token)
> Microteste 0E.1-JN2 · 2026-09-04 · READ-ONLY · rede ultra-restrita (1 Contents API + 25 raw) · fechamento parcial da lacuna §24/§25.

## 1. Regras de rede e limites usados
- 1 chamada: `GET api.github.com/repos/wongddd/muonline/contents/Source%20Client/source?ref=580472e`
- 25 downloads raw@580472e (paths SOMENTE da seleção determinística abaixo). Sem /search/code, sem HTML, sem outros hosts, sem zip/clone/git, sem compilação/execução.

## 2. Resultado da Contents API (PASSO A)
| Campo | Valor |
|---|---|
| HTTP status | **200** |
| Itens retornados | **613** = 607 files + 6 dirs (6 dirs = subdirs já conhecidos localmente) |
| Arquivos com ext .h/.hpp/.cpp/.c | **603** |

## 3. Seleção determinística (PASSO B)
- Regex no NAME (case-insensitive): `(max|model|bmd|smd|open|data|zzz|define|global|std|textureindex)` → **63 candidatos**.
- Ordenação lexical por `path` → **primeiros 25 baixados; 38 TRUNCADOS** (limite do comando).
- Selecionados (n. path, size): 01 CMVP1stDirection.cpp 12.220 · 02 CMVP1stDirection.h 734 · 03 Define.h 91 · 04 Defined_Global.h 940 · 05 GlobalBitmap.cpp 27.886 · 06 GlobalBitmap.h 3.994 · 07 GlobalPortalSystem.cpp 1.796 · 08 GlobalPortalSystem.h 1.238 · 09 GlobalText.h 6.719 · 10 LoadData.cpp 3.049 · 11 LoadData.h 413 · 12 MoveCommandData.cpp 1.965 · 13 MoveCommandData.h 1.216 · 14 SMD.cpp 6.340 · 15 SMD.h 1.743 · 16 SMD2BMD.cpp 11.632 · 17 StdAfx.cpp 284 · 18 ZzzAI.cpp 22.391 · 19 ZzzAI.h 1.999 · 20 ZzzBMD.cpp 85.786 · 21 ZzzBMD.h 10.581 · 22 ZzzCharacter.cpp 462.313 · 23 ZzzCharacter.h 3.963 · 24 ZzzEffect.cpp 541.310 · 25 ZzzEffect.h 6.504.
- **Truncados de maior interesse** (38 total; lista completa no log): **ZzzOpenData.cpp/.h · stdafx.h · _define.h · _GlobalFunctions.h/.cpp · ZzzObject.cpp/.h · ZzzScene.cpp/.h · ZzzLodTerrain.cpp/.h · ZzzTexture.cpp/.h · ZzzOpenglUtil.cpp/.h · ZzzShader.cpp/.h · _TextureIndex.h** (GitHub) etc. — `Winmain.cpp` NÃO casa a regex do comando (ficou fora por critério, não por truncagem).

## 4. Tabela de downloads (PASSO C) — 25/25 HTTP 200, ~1,16 MB
Persistidos em `NEW_ERA_EVIDENCE/` como `0E1JN2__<NN>__<stem>__path_Source_Client_source__580472e.<ext>` (+ `.SHA256.txt` irmão). SHA256 (na ordem 01→25):
`59d352a8…4bfe` · `8323f432…841` · `5e582b97…dec` · `87b52786…dc7` · `c6ae4936…300` · `65da23d7…2b3e` · `00c4287b…5c5` · `8d2c4284…e8` · `d6241a02…d64` · `5a85cf47…e9` · `e8bf2abc…888` · `538b20dc…8ad` · `32a40758…ead` · `45e469dd…434` · `d0694809…b62` · `2862f9f4…991` · `1ac67a70…ea1` · `5a1b60f8…329` · `3d27d7c1…378` · **`0083f7fbb062747a589fb7bd47ee127665b384f90c3f0e07a08ff24f053dfa55`** (ZzzBMD.cpp) · **`44d0d6693ebb4a5fc652b1ca5b1f8d0c208d2785a457fc1ce47064dc4016a0d0`** (ZzzBMD.h) · `02d3f539…e86` · `b0c28944…234` · `58b84cde…282` · `5b9b1adf…c1f` (hashes completos nos .SHA256.txt).

## 5. Achados (PASSO D) — EVIDÊNCIA EM ARQUIVO @580472e
### 5.1 Ocorrência ÚNICA de MAX_MODELS no conjunto = COMENTÁRIO histórico
`0E1JN2__20__ZzzBMD...cpp` (= `Source Client/source/ZzzBMD.cpp`) **:22**:
```cpp
//BMD Models[MAX_MODELS];      // :22  ← array estático ORIGINAL, comentado
BMD *Models;                   // :23  ← definição ATUAL: PONTEIRO dinâmico
BMD *ModelsDump;               // :24
```
`0E1JN2__21__ZzzBMD...h` **:333-335**:
```cpp
//extern BMD   Models[];       // :333 ← comentado
extern BMD   *Models;          // :334 ← declaração ATIVA (ponteiro)
extern BMD *ModelsDump;        // :335
```
### 5.2 Nenhuma definição de MAX_MODELS no conjunto
`#define/enum/const/constexpr MAX_MODELS` = **0 hits** nos 25. `Define.h` (91 B) só define `MAX_ID_SIZE 10`; `Defined_Global.h` (940 B) só feature-flags (NEW_PROTOCOL_SYSTEM, ASG_ADD_GENS_SYSTEM, KJH_PBG_ADD_INGAMESHOP_SYSTEM, PJH_ADD_PANDA_PET…). ZzzBMD.h define os limites-IRMÃOS mas não MAX_MODELS: **`#define MAX_BONES 200` (:6) · `MAX_MESH 50` (:7) · `MAX_VERTICES 15000` (:8) · `MAX_MONSTER_SOUND 10` (:35)**.
### 5.3 O limite real não está no conjunto
`new BMD` / `Models =` / `delete[] Models` = **0 hits** nos 25 → o site de alocação do ponteiro (onde o tamanho/limite é imposto) está FORA do conjunto. Usos ativos de `Models[...]` (ponteiro indexado) no conjunto: LoadData.cpp **:34** `Models[Type].m_iBMDSeqID = Type;` e **:36** `Models[Type].Open2(Dir,Name);` (hub `CLoadData::AccessModel` :22-43, com `FatalError` p/ Player/Monster/Angel ausentes); LoadData.cpp:48; SMD.cpp:217-234 (`Models[ID].Version = 10` :222); SMD2BMD.cpp:289/:381; ZzzAI.cpp:446; ZzzCharacter.cpp:379/:386/:448+ (`Models[MODEL_PLAYER]` — enum `MODEL_PLAYER` também não definido no conjunto). Conhecimento prévio [CHAT-ONLY §21.1] aponta a alocação para `ZzzOpenData.cpp:110-121` (`new BMD[MAX_MODELS+1024]` + `rand()%1024` anti-tamper) — **arquivo truncado neste microteste, NÃO verificado em arquivo**.

## 6. Conclusão classificada
**(4) AINDA INCONCLUSIVO para a definição/valor de MAX_MODELS — com progresso substantivo e próximo passo exato:**
1. **[FILE-FACT]** `MAX_MODELS` sobrevive no hub de modelos apenas como **comentário histórico** (ZzzBMD.cpp:22); o storage de modelos @580472e é **ponteiro dinâmico `BMD *Models`** (ZzzBMD.cpp:23, extern ZzzBMD.h:334), não array estático;
2. **[FILE-FACT]** logo, o limite real de modelos é imposto no **site de alocação** (`Models = new BMD[...]`) — fora do conjunto baixado (0 hits de alocação nos 25);
3. se `new BMD[MAX_MODELS+1024]` [CHAT-ONLY] se confirmar em ZzzOpenData.cpp, então MAX_MODELS **existe** como símbolo definido em header fora do conjunto (candidatos naturais: `stdafx.h`, `_define.h`, `ZzzOpenData.h`, `_GlobalFunctions.h` — TODOS casaram a regex e caíram na truncagem 26º+).
- **Faltam exatamente 5-6 arquivos** para fechar: `ZzzOpenData.cpp` · `ZzzOpenData.h` · `stdafx.h` · `_define.h` (+`Winmain.cpp`, fora-regex, teardown :443-458 [CHAT-ONLY]) → microteste sugerido **0E.1-JN3** (mesma mecânica, lista explícita, ≤6 downloads).

## 7. Limitações
- Amostra = 25/603 arquivos de `/source` (cap do comando; seleção por nome, não por conteúdo); raiz de `Source Client/` e `sdk-directx9/` seguem não-varridos.
- Contents API ref=580472e (exatidão de commit garantida; sem risco branch≠commit).
- Não se pode afirmar inexistência global de MAX_MODELS a partir de amostra (daí classe 4, não 3).

## 8. Artefatos
- Este relatório · 25 evidências + 25 .SHA256.txt em `NEW_ERA_EVIDENCE/` · Ledger §26 · CURRENT_STATE/MASTER_CHECKPOINT atualizados.
