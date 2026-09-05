# NEW-ERA — PHASE 0E.1-J: MAX_MODELS — DEFINITION HUNT (workspace local, SEM REDE)
> Microteste 0E.1-J · 2026-09-04 · READ-ONLY · Fonte: exclusivamente o workspace local (rede proibida).

## Fontes examinadas
| Fonte | Cobertura |
|---|---|
| `muonline/` INTEIRO (grep recursivo, todos os tipos de arquivo) | token `MAX_MODELS` + padrões de definição (#define/enum/const/constexpr/static const/template) + `-DMAX_MODELS` |
| `Source Client/Main.vcxproj` (648 L) | todas as `PreprocessorDefinitions` (ClCompile Debug :93 / Release :157 / ResourceCompile :180-181 vazia) |
| `Source Client/Main.vcxproj.filters/.user`, `Main.sln` | inclusos no grep global |
| `Source Client/CUsersAdministratortmpcl.rsp` (132 B; 1º char do nome = U+F03A private-use) | response file `cl` lido integralmente |
| `Source Client/_TextureIndex.h` (raiz; 823 L / 23.257 B) | grep Models/MAX_* |
| `Source Client/source/` local (345 arquivos, 6 subdirs) + `dependencies/` + `sdk-directx9/` | grep global |

## Seção 1 — Ocorrências de MAX_MODELS (workspace local)
**ZERO ocorrências** (uso OU definição) em todo o repo local — `grep -rnw "MAX_MODELS"` exit 1 (sem match), incluindo build files e .rsp.
Contexto [CHAT-ONLY, §21.1 — NÃO-CANÔNICO, fora deste workspace]: no GitHub @580472e o token é usado em 4 arquivos de `Source Client/source/` (Winmain.cpp :447 loop teardown; ZzzBMD.cpp :22 `//BMD Models[MAX_MODELS];` comentado; ZzzOpenData.cpp :111 `new BMD[MAX_MODELS+1024]` e :113 `ZeroMemory`; 4º arquivo não-identificado) — **todos ausentes do workspace local** (verificado: `source/` local contém APENAS os subdirs ExternalObject/GameShop/Math/OpenGL3/Time/Utilities; raiz vazia).

## Seção 2 — Definição encontrada (ou ausência)
- `#define MAX_MODELS`: **0** · `enum…MAX_MODELS`: **0** (padrão coberto — lacuna do scan chat-only 0E.1-I, aqui coberta) · `const/constexpr/static const…MAX_MODELS`: **0** · `template<int MAX_MODELS>`: **0**.
- **Build**: `Main.vcxproj` ClCompile — Debug (:93): `WIN32;_DEBUG;_WINDOWS;_FOREIGN_DEBUG;_LANGUAGE_FOREIGN;_LANGUAGE_ENG;_ALLOW_RTCc_IN_STL` · Release (:157): `WIN32;NDEBUG;_WINDOWS;_FOREIGN_NDEBUG;_LANGUAGE_FOREIGN;_LANGUAGE_ENG;LDS_PATCH_GLOBAL_100520` · ResourceCompile (:180-181): vazio. **NENHUM `MAX_MODELS=…`** → a definição NÃO provém do build do cliente. `.rsp`: apenas `/nologo /EHsc /MT /Fe…debugger.exe …` (132 B).
- Conclusão parcial: **símbolo não definido em NENHUM arquivo presente no workspace local.**

## Seção 3 — Limite real imposto (evidência indireta)
- `Models[` local: **0 declarações/usos** (arquivos-portadores ausentes). Únicos matches `MAX*MODEL*`: `GL_MAX_MODELVIEW_STACK_DEPTH` (dependencies/include/gl/GL.h :527, glew.h :580, glad.h :552) — **falso-positivo OpenGL, sem relação**.
- Logo, **o ponto onde o limite é imposto não é observável no workspace local** — vive nos arquivos da raiz de `source/` (ZzzBMD.h `class BMD`/membros, ZzzOpenData.cpp `OpenPlayers`, Winmain.cpp teardown) ou num header ainda não varrido do GitHub fora de `/source/`.

## Seção 4 — Classificação canônica
**(4) INCONCLUSIVO NO WORKSPACE (faltam arquivos).**
- Faltam exatamente: (i) os arquivos da **raiz de `Source Client/source/`** (local: raiz vazia; no GitHub @580472e: ~603 além dos 345 em subdirs [CHAT-ONLY]) — incluindo Winmain.cpp, ZzzBMD.cpp/.h, ZzzOpenData.cpp, ZzzScene.cpp/.h, SMD.cpp/.h (ausência verificada um a um); (ii) no lado GitHub, as partes nunca varridas: raiz de `Source Client/` (exceto _TextureIndex.h/Main.*, já cobertas localmente) e `sdk-directx9/` — acesso exige rede (proibida neste microteste).
- Nuance registrada: o scan 0E.1-I [CHAT-ONLY] cobriu os 948 de `/source` com rx **sem** padrão enum — a hipótese "enum anônimo" permanece ABERTA no lado GitHub.

## Seção 5 — Limitações + achados laterais (fato, com evidência)
1. Workspace DEGRADADO confirmado e re-quantificado: `Source Client/source` = 345 arquivos (6 subdirs, raiz vazia).
2. **Divergência local↔commit 580472e (nova evidência)**: `_TextureIndex.h` existe na RAIZ local com 23.257 B; o GitHub @580472e tem `Source Client/source/_TextureIndex.h` com 18.779 B [0E.1-A, CHAT-ONLY] — tamanhos distintos (mesmo arquivo em versões diferentes, ou arquivos distintos na raiz vs source/; não decidível sem rede).
3. `CUsersAdministratortmpcl.rsp`: artefato de build da máquina dos autores (compilação ad-hoc de `debugger.cpp` em `C:\Users\Administrator\tmp\`), commitado por engano na raiz de `Source Client/` — 1º char do nome é U+F03A (private-use, provável artefato de encoding de `:`).
4. Release define **`LDS_PATCH_GLOBAL_100520`** (:157) — flag de patch cliente nova (não catalogada antes neste projeto).

## Artefatos
- Este relatório + Ledger §24 + atualizações CURRENT_STATE/MASTER_CHECKPOINT.
