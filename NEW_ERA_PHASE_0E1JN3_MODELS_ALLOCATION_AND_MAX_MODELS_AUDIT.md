# NEW-ERA — PHASE 0E.1-JN3: Models allocation site (ZzzOpenData) + MAX_MODELS (raw@580472e)
> Microteste 0E.1-JN3 · 2026-09-04 · READ-ONLY · rede = SOMENTE 7 URLs raw@580472e (lista fechada do comando) · fecha a lacuna de alocação do JN2.

## 1. Regras de rede usadas
- 7 downloads raw@580472e, lista fechada (ZzzOpenData.cpp/.h, stdafx.h, _define.h, _GlobalFunctions.h, ZzzObject.h, Winmain.cpp). Sem GitHub API, sem search, sem HTML, sem outros hosts, sem zip/clone/git, sem compilação/execução.

## 2. Tabela de downloads (7/7 HTTP 200, ~845 KB)
| # | Path @580472e | Status | Bytes | SHA256 | Evidência em `NEW_ERA_EVIDENCE/` |
|---|---|---|---|---|---|
| 1 | Source Client/source/ZzzOpenData.cpp | 200 | 269.112 | `26504dd7d9d5428217751b89553237651a0ad315db52703431556b58527ccc61` | `0E1JN3__1__ZzzOpenData.cpp__580472e` |
| 2 | Source Client/source/ZzzOpenData.h | 200 | 817 | `776212556dbe45c1ce792710892cc450bb547c73e2df7a467a52efbb0a55a25c` | `0E1JN3__2__ZzzOpenData.h__580472e` |
| 3 | Source Client/source/stdafx.h | 200 | 2.139 | `d004d4d1ce1e7ca0af511976ba7da9439072a0807e4bd98cfa70e17b25a2934f` | `0E1JN3__3__stdafx.h__580472e` |
| 4 | Source Client/source/_define.h | 200 | 18.981 | `c2fa25d74def49ff12b76b5918e3108bb14e99b04a8bf47c77eefb84a3f7204f` | `0E1JN3__4___define.h__580472e` |
| 5 | Source Client/source/_GlobalFunctions.h | 200 | 3.325 | `849936735953b00ea216ba0b28a4a61bd057c0166398c17580500c9a27832edb` | `0E1JN3__5___GlobalFunctions.h__580472e` |
| 6 | Source Client/source/ZzzObject.h | 200 | 4.108 | `dc7ce4e8079d0c4e08eea75e8240c3c704d0a2d9485086e7aab9d74c6f2c35da` | `0E1JN3__6__ZzzObject.h__580472e` |
| 7 | Source Client/source/Winmain.cpp | 200 | 45.584 | `8e77ee5c1f0b993946cfa25ebfd534b353aeee9c5e070bc1446f65354c428e5d` | `0E1JN3__7__Winmain.cpp__580472e` |
(+ 7 arquivos irmãos `.SHA256.txt`; nenhum `.error.txt` — 0 falhas.)

## 3. MAX_MODELS: ocorrências (todas) e definição
**3 ocorrências ATIVAS (nenhuma comentada) — e NENHUMA definição nos 7:**
| Arquivo evidência | Linha | Trecho |
|---|---|---|
| `0E1JN3__1__ZzzOpenData.cpp__580472e` | **:111** | `ModelsDump = new BMD [MAX_MODELS+1024];` |
| `0E1JN3__1__ZzzOpenData.cpp__580472e` | **:113** | `ZeroMemory( Models, MAX_MODELS * sizeof ( BMD));` |
| `0E1JN3__7__Winmain.cpp__580472e` | **:447** | `for(int i=MODEL_LOGO;i<MAX_MODELS;i++)` → `Models[i].Release();` (:449) |
Definição (`#define/enum/const/constexpr MAX_MODELS`): **0 hits nos 7** (incl. `_define.h` com 415 #defines e `stdafx.h` completo). Build-define já refutado (Main.vcxproj, Ledger §24). **Conclusão: a definição está em header FORA da lista fechada — candidato #1: `_enum.h`** (presente no repo conforme listing JN2; incluído por `stdafx.h:85`; lar natural de `MODEL_PLAYER`/`MODEL_LOGO`, ausentes de `_define.h`).

## 4. Models: site de alocação e limite real [FILE-FACT]
`0E1JN3__1__ZzzOpenData.cpp__580472e` — **`void OpenPlayers()` :110-113**:
```cpp
110: void OpenPlayers()
111:     ModelsDump = new BMD [MAX_MODELS+1024];
112:     Models = ModelsDump + ( rand() % 1024);      // anti-tamper: base deslocada aleatoriamente (0..1023)
113:     ZeroMemory( Models, MAX_MODELS * sizeof ( BMD));
```
- **Limite real imposto por `MAX_MODELS`** (o símbolo É o limite — classe (3) fica refutada): aloca `MAX_MODELS+1024` BMDs e usa janela de `MAX_MODELS` a partir do offset aleatório (sobram `1024-rand()` slots de folga no fim; consistente com :113).
- Boot chain: `OpenBasicData` **:5394-5396** (`g_ErrorReport.Write("[OpenBasicData] OpenPlayers enter")` → `OpenPlayers();` → `"... done"`) — confirma [CHAT-ONLY §21.1] EM ARQUIVO.
- Teardown: `Winmain.cpp :447-449` (loop `MODEL_LOGO..MAX_MODELS-1` → `Models[i].Release()`) + **:459** `SAFE_DELETE_ARRAY( ModelsDump );` (único delete do bloco — confirma [CHAT-ONLY] ":443-458" com fim exato :459).
- Alocações-irmãs no mesmo teardown (:456-458): `CharacterMemoryDump`, `ItemAttRibuteMemoryDump`, `RendomMemoryDump` — mesmo padrão *Dump de anti-tamper.
- Família de constantes de modelo (evidência de expressões encadeadas/condicionais), `_define.h`: `MAX_MODEL_MONSTER 400` (:110) · `MODEL_MONSTER01 = MODEL_SKILL_END+1` (:112) · `MODEL_MONSTER_END = MODEL_MONSTER01+MAX_MODEL_MONSTER` (:113) · `MODEL_BODY_NUM 24/20` (:116/:118, condicional `PBG_ADD_NEWCHAR_MONK`) · `MAX_CLASS 7/6` (:379/:381, idem condicional).

## 5. Conclusão classificada
**(2) MAX_MODELS EXISTE (símbolo ativo: 3 usos em 2 TUs, §3) mas o valor não é um literal fechado nos 7 — depende de definição em header fora da lista** (candidato #1 `_enum.h` via `stdafx.h:85`; família usa expressões encadeadas e condicionais de feature-flags, §4; build-define refutado §24).
→ **Falta 1 (um) arquivo para fechar o valor: `_enum.h`** (sugerido 0E.1-JN4: 1 download raw@580472e).
Nota de enquadramento: a classe (4) do comando exigiria "nem alocação nem definição aparecerem" — alocação APARECEU (logo (4) excluída pela letra); a (1) exige valor literal (não achado); a (3) está refutada (MAX_MODELS é usado exatamente no ponto que impõe o limite).

## 6. Limitações
- Lista fechada de 7 não incluía `_enum.h`/`_types.h`/`_struct.h`/`Winmain.h` (todos includes de stdafx.h) — definição pode estar em qualquer um deles (candidato #1 `_enum.h` por semântica).
- [CHAT-ONLY §21.1] mencionava "4 arquivos-usuários" de MAX_MODELS; em arquivo (JN2+JN3, 32 arquivos) vemos **3**: ZzzOpenData.cpp (:111/:113), Winmain.cpp (:447) + comentário ZzzBMD.cpp:22 — 4º usuário não localizado no conjunto baixado (hipótese: arquivo não-baixado, ex. ZzzScene.cpp).
- **Nota metodológica (quase-falso-negativo)**: o 1º grep de token usou glob `*__580472e.*` que só casou os sidecars `.SHA256.txt` (arquivos de evidência não têm extensão final) → teria reportado "0 ocorrências"; detectado no grep de alocação e REFEITO com paths explícitos (resultado §3 é o correto). Lição registrada: sempre validar cobertura de glob contra o inventário.
- Bounds-check explícito de índice de model (`if(idx >= MAX_MODELS)`): não existe nos 7 — o limite é imposto apenas pelo tamanho da alocação (+ teardown). Achado de risco herdado do §12 (sem validação de índice).

## 7. Artefatos
- Este relatório · 7 evidências + 7 `.SHA256.txt` em `NEW_ERA_EVIDENCE/` · Ledger §27 · CURRENT_STATE/MASTER_CHECKPOINT atualizados.
