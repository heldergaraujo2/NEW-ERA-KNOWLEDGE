# NEW-ERA — PHASE 0E.1-N2: Encontrar definição/range de ConvertItemType (priorizado; rede restrita)
> Microteste 0E.1-N2 · 2026-09-04 · offline + 1 Contents API (200/613) + 10 raw@580472e (cap consumido; parar-cedo não disparou).

## 1. Seleção priorizada (PASSO B)
- **PRIORIDADE-1**: `ZzzInventory.cpp` ✓ EXISTS (baixado #1) · `ZzzInventory.h` ✓ (#2) · `ZzzItem.cpp/.h` ✗ inexistentes · `Inventory.cpp/.h` ✗ inexistentes.
- **PRIORIDADE-2** (regex zzzinventory|inventory|convert|item|equip|myinventory|newuimyinventory|drop|pick; excl. os 10 já baixados no 0E.1-N — dedupe documentado): NewUIItemEnduranceInfo.cpp/.h · NewUIItemExplanationWindow.cpp/.h · NewUIItemMng.cpp/.h · NewUILuckyItemWnd.cpp/.h (10 total; 13 P2 restantes não selecionados).

## 2. Downloads (10/10 HTTP 200, ~399 KB)
`0E1N2__01..10__*`(+.SHA256): ZzzInventory.cpp 333.218 B · ZzzInventory.h 6.850 · NewUIItemEnduranceInfo.cpp 19.526/.h 2.678 · NewUIItemExplanationWindow.cpp 10.022/.h 1.048 · NewUIItemMng.cpp 5.894/.h 1.505 · NewUILuckyItemWnd.cpp 16.035/.h 3.638.

## 3. Resultado: **NOT FOUND — classe (3)**
- Única ocorrência do token nos 10: **`ZzzInventory.cpp:7646 int Type = ConvertItemType(Item);` = CALL (a 6ª)**.
- Total acumulado: **6 calls / 0 defs / 0 protótipos em 78 arquivos de evidência** (WSclient ×4 :1951/:5647/:5722/:5887 · ZzzObject :6112 · ZzzInventory :7646).

## 4. Inferência nova (includes comuns dos 3 TUs portadores)
WSclient.cpp, ZzzObject.cpp e ZzzInventory.cpp compartilham includes **ainda ausentes do acervo**: **`UIManager.h`** (presente nos 3) · **`ZzzInfomation.h/.cpp`** (grafia "Infomation" — **não casa nenhuma regex usada em JN2/N/N2!**) · `ZzzInterface.h` · `DSPlaySound.h` · `GOBoid.h` · `CSQuest.h` · `GMHellas.h` · `PersonalShopTitleImp.h`(cpp truncado N). O protótipo/def de `ConvertItemType` está quase certamente nesse conjunto — alvo natural do próximo fechamento (4-6 raw).

## 5. Impacto no §31
P1/P4/P5 permanecem **CONDICIONAIS**; range INDETERMINADO (anti-invenção); evidência estrutural 13-bit inalterada. Risco §29 inalterado.

## 6. Conclusão: **(3) INCONCLUSIVO** — def não encontrada; faltou exatamente: headers/impl compartilhados não-varridos (§4), em especial `UIManager.h` e `ZzzInfomation.{h,cpp}`.

## 7. Artefatos
Este relatório · 10 evidências + sidecars `0E1N2__*` · Ledger §33 · CURRENT_STATE/MASTER_CHECKPOINT.
