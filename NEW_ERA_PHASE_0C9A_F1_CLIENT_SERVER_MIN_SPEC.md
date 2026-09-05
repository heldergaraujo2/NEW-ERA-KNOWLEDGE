# NEW-ERA — PHASE 0C.9-A: Head 0xF1 — reconciliação + spec mínima cliente↔servidor (offline)
> Microteste 0C.9-A (partes A1/A2/A3) · 2026-09-04 · SEM REDE · fecha a série 0C.9.

## 1. Fontes
| Lado | Evidência | SHA256 |
|---|---|---|
| Servidor GS | `GS_Protocol.cpp__580472e__sha256_e8bb4dbb.cpp` | `e8bb4dbb28d1baf23222dbc72762e2ebd55b116dbb8d36e5f017dd744e510f13` |
| Cliente | `0E1L__13__WSclient…cpp` | `6538d944126181dfa77d4419bf992ac77151eec5411dc3ab243f2926fec868bf` |
| Header | `0E1L__14__WSclient.h` | sidecar `0E1L__14…` |

## 2. Servidor — case 0xF1 (**:942**) → `switch(lpMsg[3])` :943-:953 — 3 subcases, sem default
| Sub | Linha | Handler (C→S) | Cast |
|---|---|---|---|
| 0x01 | :945 | `CGConnectAccountRecv` | `(PMSG_CONNECT_ACCOUNT_RECV*)` |
| 0x02 | :948 | `CGCloseClientRecv` | `(PMSG_CLOSE_CLIENT_RECV*)` |
| 0x03 | :951 | **VAZIO** (só `break;`) | — |
*Structs PMSG_CONNECT_ACCOUNT_RECV/PMSG_CLOSE_CLIENT_RECV: **não encontradas** no escopo de evidências (defs server-side).*

## 3. Cliente — case 0xF1 (**:12825**, 1º case de `TranslateProtocol` :12821) → `switch(Data->SubCode)` :12828-:12939 — 7 subcases, sem default
| Sub | Linha | Handler (S→C) | Fase |
|---|---|---|---|
| 0x00 | :12830 | `ReceiveJoinServer` | join server |
| 0x01 | :12833 | login inline + `switch(Data->Value)` :12835 (~20 códigos: 0x20/0x01 sucesso `LogIn=2;CheckHack()`; 0x00 pwd · 0x02 ID · 0x03 connected · 0x04 busy · 0x05 block · 0x06 version · … · 0x11 under-15 · 0x40 charged · 0xc0/0xd0/0xc1/0xd1 point/date) | login |
| 0x02 | :12908 | `ReceiveLogOut` | logout |
| 0x12 | :12914 | create-account inline + `switch(Data->Value)` :12915 (0x00 fail-ID · 0x01 SUCCESS · 0x02 fail-RESIDENT) | criação de conta |
| 0x03 | :12928 | `ReceiveConfirmPassword` | senha |
| 0x04 | :12932 | `ReceiveConfirmPassword2` | senha |
| 0x05 | :12936 | `ReceiveChangePassword` | senha |

## 4. Prova estrutural do header (WSclient.h :129-134)
```c
typedef struct
{
    PBMSG_HEADER Header;   // {Code; Size; HeadCode} = 3 bytes (C1)
    BYTE         SubCode;  // = byte[3]
    BYTE         Value;    // = byte[4]
} PHEADER_DEFAULT_SUBCODE, * LPPHEADER_DEFAULT_SUBCODE;
```
Sem branch C1/C2 neste head (cast único — pacotes de login/conta são C1).

## 5. Reconciliação
| Conjunto | Subcodes | N |
|---|---|---|
| **COMUNS** | 01 02 03 | **3** |
| **SOMENTE-SERVIDOR** | — | **0** |
| **SOMENTE-CLIENTE** | 00 04 05 12 | **4** |

**Explicação (evidência)**: como no F3 (§35), **assimetria direcional por design** — servidor trata requests C→S (`CG*Recv`: connect-account/close-client), cliente trata respostas S→C (`Receive*`/inline). Além disso, o namespace 0xF1 **cobre múltiplos peers e fases**: o lado do cliente aqui é a **fase Connect/JoinServer** (join, login, contas, senhas — coerente com `TranslateProtocol` ser o dispatcher pré-jogo), enquanto o lado GS trata account-connect/close dentro do jogo. O **subcode é reutilizado entre peers com semânticas distintas**: 0x01 = login-result (S→C do JoinServer) × connect-account (C→S ao GS); 0x02 = logout × close-client (par semântico de ciclo de vida); **0x03 = ConfirmPassword (cliente) × case VAZIO (servidor GS)** — não é contradição: são peers diferentes; o GS simplesmente ignora 0xF1:0x03.

## 6. SPEC MÍNIMA (MVP) — 0xF1
**Cliente (implementar no cliente novo, direção S→C, framing C1 `{C1,len,0xF1,SubCode,Value,…}`):**
| Sub | Semântica | Estrutura |
|---|---|---|
| 0x00 | join-server ok | PHEADER_DEFAULT_SUBCODE + payload |
| 0x01 | resultado de login | SubCode + **Value enum** (sucesso: 0x20/0x01; falhas: 0x00,0x02-0x06,0x07+,0x08-0x0d,0x11,0x40,0xc0/0xc1/0xd0/0xd1) |
| 0x02 | logout | handler retorna bool |
| 0x03/0x04 | confirmação de senha (v1/v2) | PHEADER_DEFAULT_SUBCODE |
| 0x05 | troca de senha | idem |
| 0x12 | resultado criação de conta | Value enum (0x00/0x01/0x02) |
**Servidor (C→S, no GS)**: 0x01 connect-account (struct não encontrada no escopo) · 0x02 close-client (idem) · 0x03 no-op.

## 7. Conclusão: **(1) MISMATCH EXPLICADO (direcional + multi-peer/fase) + SPEC MÍNIMA PRONTA** (client 7 / server 3; comuns {01,02,03}; deltas explicados).

## 8. Artefatos
Este relatório · Ledger §36 · CURRENT_STATE/MASTER_CHECKPOINT. (Nenhuma nova evidência.)
