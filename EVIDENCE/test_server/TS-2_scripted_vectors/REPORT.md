# EVIDENCE — NEW-ERA Test Server TS-2 (scripted vectors over BOTH_MESSAGE)

## Objetivo
Criar um executável de **Test Server** (não MU completo) para validar o MVP NEW-ERA em sessão TCP real:
- aceita TCP
- lê envelope moderno: `id:u16 LE + size:u32 LE` (6 bytes)
- suporta `BOTH_MESSAGE (0x000C)` como túnel de frames clássicos (C1/C2/C3/C4)
- envia respostas **scriptadas** usando `frame_hex` dos **JSON normativos** do repo (sem inventar bytes)

## Arquivos adicionados
- `NEW_ERA_IMPLEMENTATION/test_server/new_era_test_server.cpp`
- `NEW_ERA_IMPLEMENTATION/test_server/CMakeLists.txt`

## Script TS-2 (após primeiro RX do cliente)
Envia 3 frames clássicos encapsulados em `BOTH_MESSAGE (0x000C)`:
1) F3:03 CharacterInfo (vector: `charinfo_basic_nontrivial` em `rx_character_info_f3_03_vectors.json`)
2) F3:10 Inventory count=1 (vector: `inv_f3_10_count1_slot0_pattern` em `rx_inventory_f3_10_vectors.json`)
3) F3:10 Inventory count=0 (vector: `inv_f3_10_count0_empty` em `rx_inventory_f3_10_vectors.json`)

## Build (Windows 11)
Exemplo (PowerShell):

```powershell
cd C:\path\to\NEW-ERA-KNOWLEDGE
cmake -S NEW_ERA_IMPLEMENTATION\test_server -B build\test_server
cmake --build build\test_server --config Release
.\build\test_server\Release\new_era_test_server.exe 55901 C:\path\to\NEW-ERA-KNOWLEDGE
```

## Robustez
- clamp do `size` moderno (u32) em 256 KiB
- disconnect em size absurdo / EOF / leitura curta
- valida que frame clássico começa com `C1/C2/C3/C4`
