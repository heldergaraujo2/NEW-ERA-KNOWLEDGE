# NEW-ERA — PHASE 0E.1-J-NET: MAX_MODELS via GitHub Code Search + raw@580472e (REDE RESTRITA)
> Microteste 0E.1-J-NET · 2026-09-04 · READ-ONLY · 1 chamada search + raw fixado no commit (regra) · resultado: **BLOQUEADO NA ETAPA A**.

## 1. Regras de rede usadas
- Endpoint A (1 única chamada): `https://api.github.com/search/code?q=MAX_MODELS+repo:wongddd/muonline&per_page=25`
- Endpoint B (raw@580472e): **autorizado somente para paths retornados pelo search** — não acionado (0 paths).
- Sem navegação HTML, sem outros hosts, sem ZIP/tarball, sem git, sem compilação/execução.

## 2. Resultado do search (PASSO A)
| Campo | Valor |
|---|---|
| HTTP status | **401** |
| Corpo da resposta | `{"message": "Requires authentication", "documentation_url": "https://docs.github.com/rest", "status": "401"}` (120 B) |
| total_count / itens | **0 / 0** (nenhum path retornado) |
| Evidência persistida | `NEW_ERA_EVIDENCE/MAX_MODELS__search_code_api__response_401.json` + `.SHA256.txt` |

Causa (fato documentado pelo próprio endpoint): o **Code Search da API do GitHub exige autenticação** (token). Nenhum token foi fornecido pelo coordenador e nenhum credencial não-autorizada foi usada.

## 3. Tabela de evidências baixadas (PASSO B)
**NENHUMA** — 0/0 paths → 0 downloads (limite 25 não atingido; regra "somente paths do search" cumprida estritamente). Nenhuma evidência de código baixada; nada a validar quanto ao token `\bMAX_MODELS\b`.

## 4. Ocorrências / Definição (PASSO C)
Não executável — sem arquivos baixados. O estado do conhecimento sobre `MAX_MODELS` permanece o do 0E.1-J (Ledger §24): 4 arquivos-usuário no GitHub @580472e [CHAT-ONLY §21.1], definição não encontrada nos 948 de `/source` (scan sem enum-rx), não é build-define (Main.vcxproj local).

## 5. Conclusão classificada
**(4) INCONCLUSIVO mesmo com rede** — bloqueio na etapa A: endpoint de search exige autenticação (401), não há token autorizado; downloads de raw não foram autorizados fora dos paths do search (0 retornados).
Caminhos de fechamento (exigem novo comando do coordenador — NÃO executados):
1. Fornecer `GITHUB_TOKEN` (com escopo mínimo de leitura pública) → re-run deste microteste;
2. Autorizar varredura via **trees API** (`api.github.com/repos/wongddd/muonline/git/trees/384a0f7b…?recursive=1` — tree já conhecida de fases anteriores) + download raw dos candidatos (raiz de `Source Client/`, `sdk-directx9/`, e enum-rx nos 948 de `/source`);
3. Autorizar download direto dos 4 paths-usuário conhecidos [§21.1] + headers de include adjacentes (menor custo de rede: ~6-10 arquivos).

## 6. Limitações
- Search da API indexa branch default (main) — neste repo coincide com o commit canônico (580472e = HEAD de main [LEDGER §0]); a ressalva "branch vs commit" não chegou a ser exercitada (401 antes).
- Sem token, nenhuma alternativa dentro das regras deste comando era legal — encerrado sem escopo extra por governança.

## 7. Artefatos
- Este relatório · `NEW_ERA_EVIDENCE/MAX_MODELS__search_code_api__response_401.json`(+.SHA256) · Ledger §25 · atualizações CURRENT_STATE/MASTER_CHECKPOINT.
