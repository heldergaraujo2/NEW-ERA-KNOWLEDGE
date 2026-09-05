# NEW-ERA — PHASE 0E.1-N3: ConvertItemType — varredura em UIManager/ZzzInfomation/ZzzInterface/DSPlaySound (rede restrita)
> Microteste 0E.1-N3 · 2026-09-04 · 1 Contents API (200/613) + 3/6 raw@580472e (parar-cedo) · **FECHA o elo P1/P4/P5 do §31 e o vetor R→Models[] aberto desde §30**.

## 1. Passo 0 (offline)
Baseline confirmada: 6 ocorrências de `ConvertItemType(` em 78 evidências, todas CALLs.

## 2. Tabela alvo→existência→download (ordem fixa do comando)
| # | Alvo | Existia | Baixado | Bytes | SHA256 |
|---|---|---|---|---|---|
| 1 | UIManager.h | SIM | SIM | 2.439 | sidecar `0E1N3__1…` |
| 2 | ZzzInfomation.h | SIM | SIM | 3.778 | sidecar `0E1N3__2…` |
| 3 | ZzzInfomation.cpp | SIM | SIM (**FOUND-DEF, stop**) | 118.917 | `d4dd353569254d18931e5961da62b3df35be9f86e7464c12eb6e2cd4493b72c8` |
| 4 | ZzzInterface.h | SIM | NÃO (stop) | — | — |
| 5 | DSPlaySound.h | SIM | NÃO (stop) | — | — |
| 6 | UIManager.cpp | SIM | NÃO (stop) | — | — |

## 3. Achados
- **FOUND-PROTÓTIPO**: `ZzzInfomation.h:29` — `extern int  ConvertItemType(BYTE *Item);` (evidência `0E1N3__2`, 3.778 B).
- **FOUND-DEFINIÇÃO**: `ZzzInfomation.cpp:371-377` (evidência `0E1N3__3`, sha `d4dd3535…`):
```c
371: int ConvertItemType(BYTE *Item)
372: {
373:     int ret = 0;
374:     ret = Item[0]+(Item[3]&128)*2+ (Item[5]&240)*32;
375:     return ret;
376: }
```

## 4. Prova do RANGE do retorno (BYTE = 8 bits; bytes vem do pacote)
| Termo | Domínio | Contribuição máx |
|---|---|---|
| `Item[0]` | [0..255] | 255 (bits 0-7) |
| `(Item[3]&128)*2` | {0,128}×2 | 256 (bit 8) |
| `(Item[5]&240)*32` | {0,16..240}×32 | 7680 (bits 9-12) |
**ret ∈ [0, 8191]** — máximo exato **8191 = 0x1FFF** (empacotamento de 13 bits). Bate **exatamente** com a sentinela `0x1FFF` dos 3 receivers (§31) e com o lattice: bloco de itens termina em `MODEL_HELM2−1 = MODEL_ITEM+8191 = 9277`.

## 5. Impacto no §31 (P1/P4) e fechamento do vetor R→Models[]
| Caminho | Expressão | Range do model-id | < MAX_MODELS (10002)? |
|---|---|---|---|
| **P1** `ReceiveChangePlayer` 0x25 (WSclient:1959+) | `MODEL_ITEM + Type` | [1086 .. **9277**] | **SIM — PROVADO** |
| **P4** `CreateItem` (ZzzObject:6145) | `MODEL_SWORD + Type` | [1086 .. **9277**] | **SIM — PROVADO** |
| P5 `SetCharacterClass` (:11741+) | `p[…].Type + MODEL_ITEM` | idem (domínio herdado) | **SIM — PROVADO** |
Encaixe de design: ids de item ocupam [1086, 9277]; `MODEL_HELM2=9278` inicia logo acima — **zero overlap, folga 725** até 10002.
**Conclusão agregada do vetor rede→Models[] (§30+§31+§33+esta)**: TODOS os caminhos R conhecidos são estruturalmente in-bounds ou neutralizados (monstros: default-substitution; equip: máscaras/sentinelas; itens: 13-bit aritmético provado). O §29 fica definitivamente como **fragilidade latente** (0 guards), sem vetor ativo de rede.

## 6. Conclusão classificada: **(1) DEFINIÇÃO ENCONTRADA e RANGE PROVADO [0, 8191]** — P1/P4/P5 in-bounds.

## 7. Artefatos
Este relatório · evidências `0E1N3__1/2/3__*`(+.SHA256) · Ledger §34 · CURRENT_STATE/MASTER_CHECKPOINT.
