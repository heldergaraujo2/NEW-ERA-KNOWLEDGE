# NEW-ERA Test Server

Servidor de teste (laboratório) para validar o MVP NEW-ERA **ao vivo** pela rede.

## O que ele faz (até TS-2)
- TCP listen em `0.0.0.0:PORT` (default `55901`)
- Aceita 1 cliente por vez
- RX: lê envelope moderno `id:u16 LE + size:u32 LE` (6 bytes) + body
- Suporta `BOTH_MESSAGE (0x000C)` como túnel: body carrega o frame clássico (C1/C2/...)
- TS-2: após o **primeiro** pacote recebido do cliente, envia 3 respostas **scriptadas** (cada uma encapsulada em `BOTH_MESSAGE`), carregadas dos JSON normativos do repo:
  - F3:03 `charinfo_basic_nontrivial`
  - F3:10 count1 `inv_f3_10_count1_slot0_pattern`
  - F3:10 count0 `inv_f3_10_count0_empty`

## Build (Windows 11)
Exemplo (PowerShell) usando CMake + Visual Studio:

```powershell
cd C:\path\to\NEW-ERA-KNOWLEDGE
cmake -S NEW_ERA_IMPLEMENTATION\test_server -B build\test_server
cmake --build build\test_server --config Release
```

## Run
O executável aceita:
- `port` (opcional)
- `repoRoot` (opcional): caminho para a raiz do repo (para achar os JSON `rx_*_vectors.json`)

Exemplos:

```powershell
# rodando a partir da raiz do repo (repoRoot = .)
.\build\test_server\Release\new_era_test_server.exe 55901 .

# rodando de outro lugar, apontando para a raiz do repo
.\new_era_test_server.exe 55901 C:\path\to\NEW-ERA-KNOWLEDGE
```

## Segurança/robustez
- clamp de `size` moderno (u32) em 256 KiB; desconecta em size absurdo
- desconecta em EOF/leitura curta
