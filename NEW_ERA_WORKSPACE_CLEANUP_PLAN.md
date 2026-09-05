# NEW-ERA — WORKSPACE CLEANUP PLAN (proposta ONLY — nada foi deletado/movido/compactado)
> Gerado 2026-09-04 · companion do `NEW_ERA_EVIDENCE_MANIFEST.md` · requer comando futuro explícito para qualquer deleção.

## 1. Objetivo e diagnóstico

- Meta sugerida: workspace < 110 MiB → seria preciso liberar **~19 MiB**.
- Atual: **135.125.516 B (128,87 MiB) / 4.374 arquivos** — **no teto de snapshot (~128 MB)**.
- **O driver NÃO é evidência**: `NEW_ERA_EVIDENCE/` = 2,91 MiB (116 arquivos; 0 duplicados; 2 sidecars não-ref = 188 B). Docs NEW-ERA = 0,24 MiB. Outros = 0,15 MiB.
- **O driver É o repo legado `muonline/` = 125,57 MiB (4.240 arquivos)** — protegido por governança (READ-ONLY absoluto: não modificar/deletar/mover).

## 2. Composição de muonline/ (informativo — PROTEGIDO)

| Subpasta | Arquivos | Bytes |
|---|---|---|
| muonline/Client/ | 1957 | 102,049,313 B (97.32 MiB) |
| muonline/Source Client/ | 1010 | 10,347,867 B (9.87 MiB) |
| muonline/Source Server/ | 648 | 9,062,711 B (8.64 MiB) |
| muonline/MuServer/ | 575 | 8,868,827 B (8.46 MiB) |
| muonline/client_screen.png/ | 1 | 831,539 B (0.79 MiB) |
| muonline/Source Tools/ | 23 | 395,701 B (0.38 MiB) |
| muonline/客户端与服务端匹配分析报告.md/ | 1 | 26,019 B (0.02 MiB) |
| muonline/analysis_report.md/ | 1 | 13,048 B (0.01 MiB) |
| muonline/fix_log_v2.md/ | 1 | 10,126 B (0.01 MiB) |
| muonline/fix_log_v1.md/ | 1 | 7,713 B (0.01 MiB) |
| muonline/fix_log_v6.md/ | 1 | 6,442 B (0.01 MiB) |
| muonline/docs/ | 1 | 6,350 B (0.01 MiB) |
| muonline/待修复日志.md/ | 1 | 6,091 B (0.01 MiB) |
| muonline/CLAUDE.md/ | 1 | 5,547 B (0.01 MiB) |
| muonline/Sync-Version.ps1/ | 1 | 5,509 B (0.01 MiB) |
| muonline/fix_log_v3.md/ | 1 | 4,843 B (0.00 MiB) |
| muonline/fix_log_v4.md/ | 1 | 3,625 B (0.00 MiB) |
| muonline/fix_log_v5.md/ | 1 | 3,047 B (0.00 MiB) |
| muonline/fix_sql_reg2.bat/ | 1 | 2,890 B (0.00 MiB) |
| muonline/README.md/ | 1 | 2,543 B (0.00 MiB) |
| muonline/sql_console_out.txt/ | 1 | 2,191 B (0.00 MiB) |
| muonline/install_sql_service.bat/ | 1 | 2,068 B (0.00 MiB) |
| muonline/fix_sql_reg.bat/ | 1 | 2,051 B (0.00 MiB) |
| muonline/fix_sql_perms.bat/ | 1 | 1,733 B (0.00 MiB) |
| muonline/start_muserver.bat/ | 1 | 1,459 B (0.00 MiB) |
| muonline/check_sql_error.bat/ | 1 | 929 B (0.00 MiB) |
| muonline/sql_debug.bat/ | 1 | 914 B (0.00 MiB) |
| muonline/start_sql.bat/ | 1 | 579 B (0.00 MiB) |
| muonline/.gitignore/ | 1 | 381 B (0.00 MiB) |
| muonline/build_server.bat/ | 1 | 233 B (0.00 MiB) |
| muonline/set_sql_auto.bat/ | 1 | 160 B (0.00 MiB) |
| muonline/sql_console_err.txt/ | 1 | 0 B (0.00 MiB) |

Maiores itens: Main.exe.original 5,84 MB · glew.h 1,31 MB · DLLs runtime duplicadas ×5 (msvcr100.dll 770 KB ×5 = 3,85 MB; msvcp100.dll 421 KB ×5 = 2,11 MB — duplicação intra-repo INVENTARIADA, não tocada) · assets de áudio/textura (.wav/.OZJ/.OZT, vários 0,4-0,7 MB) · GameServer.suo 750 KB · PredictedInputCache…dat 524 KB.

## 3. Proposta KEEP (prioridade máxima)

- **KEEP 100% de `NEW_ERA_EVIDENCE/`** (116 arquivos): 114 referenciados + 2 sidecars parceiros (proveniência; 188 B).
- **KEEP 100% de `muonline/`** (governança READ-ONLY; base de pesquisa da FASE 0).
- **KEEP** docs NEW-ERA (Ledger/State/Checkpoint/relatórios PHASE_*).

## 4. Candidatos a remoção (NÃO EXECUTADO)

| Categoria | Itens | Bytes recuperáveis | Observação |
|---|---|---|---|
| Duplicados em NEW_ERA_EVIDENCE/ | 0 | 0 B | nenhum grupo encontrado |
| Não-referenciados em NEW_ERA_EVIDENCE/ | 2 sidecars | 188 B | recomendado KEEP (proveniência) |
| .error.txt antigos | 0 | 0 B | não existem |
| muonline/ (binários/assets) | — | — | **PROTEGIDO por READ-ONLY** — qualquer proposta exigiria comando explícito do coordenador alterando a governança |

**Estimativa total recuperável DENTRO da governança atual: 0 B (ou 188 B se os 2 sidecars forem descartados — irrelevante).**

## 5. SAFE-DELETE SET (para comando futuro)

**VAZIO** — não há como atingir a meta de 110 MiB sem tocar no repo legado, o que viola a regra READ-ONLY vigente. Nenhum arquivo foi removido; deleção requer comando separado e aprovação explícita.

## 6. Recomendações ativas (sem deleção)

1. **Disciplina de budget para novos downloads**: evidências futuras devem ser curadas (só arquivos-portadores; evitar re-download de já-evidenciados — prática já seguida no 0E.1-L).
2. **Se o coordenador decidir abrir mão de partes do repo** (ex.: `muonline/Client/Data/` assets de áudio/textura ~dezenas de MiB, ou DLLs/binários), isso requer comando explícito QUEBRA a governança READ-ONLY — registrado aqui apenas como opção de decisão, não como recomendação do guardião.
3. Manter /tmp para transitórios (scripts/JSONs de análise) — fora do snapshot por natureza.
4. Re-executar este manifesto após cada fase com downloads.

## 7. Assinatura

Workspace no momento do plano: 4,374 arquivos / 135,125,516 B. Manifest: ver `NEW_ERA_EVIDENCE_MANIFEST.md`. **Nenhum arquivo foi deletado, movido ou compactado.**
