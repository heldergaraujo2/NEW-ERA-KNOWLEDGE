# NEW-ERA — PHASE 0C.9-B: 0xF1:0x01 (Login Result) — spec operacional + origem server (offline)
> Microteste 0C.9-B · 2026-09-04 · SEM REDE · fecha o fluxo de login do head 0xF1.

## 1. Fontes
| Fonte | Papel | SHA256 / tamanho |
|---|---|---|
| `NEW_ERA_EVIDENCE/GS_Protocol.cpp__580472e__sha256_e8bb4dbb.cpp` | GameServer @580472e (evidência) | `e8bb4dbb…10f13` |
| `NEW_ERA_EVIDENCE/0E1L__13__WSclient…cpp` | Cliente | `6538d944…c868bf` (415.884 B) |
| `NEW_ERA_EVIDENCE/0E1L__14__WSclient.h` | Header cliente | sidecar 0E1L__14 |
| `muonline/Source Server/JoinServer/JoinServer/JoinServerProtocol.cpp` | JoinServer (repo, READ-ONLY in place) | `531fb3d527be9a6291fb957dbf9397856134bf1b8b68075332991e6da17f2446` (671 L) |
| `…/JoinServerProtocol.h` | idem (defs) | `3c9248ea5e1160258ad72c9d505a00f13a5486022a49b0418733ae463d91a6ee` (314 L) |
| ConnectServer (repo) | **Sem papel**: 0 ocorrências de `0xF1`/`ConnectAccount` | — |
| GameServer local (repo) | **VAZIO de protocolo** (só `Time/`, `lua/`) — GS existe só na evidência @580472e | — |

## 2. CADEIA COMPLETA (provada)
```
Cliente ── C1 0xF1:0x01 (account/password crypted) ──▶ GS CGConnectAccountRecv (GS_Protocol.cpp:1960-:2006)
   GS: version/serial mismatch → result 6 (:1972/:1978); senão PacketArgumentDecrypt (:1992/:1996)
GS ── GJConnectAccountSend (interno, head JS 0x01) ──▶ JoinServer GJConnectAccountRecv (JoinServerProtocol.cpp:76-:254)
JS: result 0=senha errada · 1=ok · 2=conta inválida/inexistente/DB · 3=já conectado (:219-:224) · 4=cheio (:95-:104)
    + BlockCode (:188) · sno__numb (:186) · AccountLevel/ExpireDate (:200-:202) · Lock (:206-:215)
GS ── GCConnectAccountSend(aIndex,result) → header.set(0xF1,0x01) (:3127-:3142, set :3132; overload c/ SOCKET :3144-:3155, set :3149) ──▶ Cliente
Cliente: TranslateProtocol → case 0xF1 :12825 → case 0x01 :12833 → switch(Data->Value) :12835-:12906
```
**Correspondência JS/GS result ↔ client Value 1:1 para os códigos núcleo: 0→0x00 · 1→0x01 · 2→0x02 · 3→0x03 · 4→0x04 · 6→0x06.**

## 3. Cliente — tabela canônica de Value (S→C; 23 labels / 21 outcomes; **default existe**: `case 0x07: default:` :12866-:12867)
| Value | Linha | Popup/enum (WSclient.h:27-46) | Ação | Classe |
|---|---|---|---|---|
| 0x20 | :12837 | — | `CurrentProtocolState=RECEIVE_LOG_IN_SUCCESS; LogIn=2; CheckHack()` | **SUCESSO** |
| 0x01 | :12845 | — | idem | **SUCESSO** (JS result 1) |
| 0x00 | :12842 | FAIL_PASSWORD(21) | popup | falha de senha (JS 0) |
| 0x02 | :12850 | FAIL_ID(22) | popup | conta inexistente/inválida (JS 2) |
| 0x03 | :12853 | FAIL_ID_CONNECTED(23) | popup | já conectado (JS 3) |
| 0x04 | :12856 | FAIL_SERVER_BUSY(24) | popup | cheio (JS 4) |
| 0x05 | :12859 | FAIL_ID_BLOCK(25) | popup | bloqueio (BlockCode) |
| 0x06 | :12862 | FAIL_VERSION(26) | popup + `g_ErrorReport.Write("Version dismatch. - Login")` | versão (GS 6) |
| 0x07/default | :12866 | FAIL_CONNECT(27) | popup | **fallback genérico** |
| 0x08 | :12870 | FAIL_ERROR(28) | popup | erro genérico |
| 0x09 | :12873 | FAIL_NO_PAYMENT_INFO(37) | popup | pagamento |
| 0x0a | :12876 | FAIL_USER_TIME1(29) | popup | tempo de uso |
| 0x0b | :12879 | FAIL_USER_TIME2(30) | popup | tempo de uso |
| 0x0c | :12882 | FAIL_PC_TIME1(31) | popup | tempo PC-bang |
| 0x0d | :12885 | FAIL_PC_TIME2(32) | popup | tempo PC-bang |
| 0x11 | :12888 | FAIL_ONLY_OVER_15(38) | popup | idade <15 (sno__numb) |
| 0x40 | :12891 | FAIL_CHARGED_CHANNEL(39) | popup | canal pago (AccountLevel) |
| 0xc0/0xd0 | :12894-:12895 | FAIL_POINT_DATE(34) | popup | ponto/data |
| 0xc1/0xd1 | :12898-:12899 | FAIL_POINT_HOUR(35) | popup | ponto/hora |
| 0xc2/0xd2 | :12902-:12903 | FAIL_INVALID_IP(36) | popup | IP inválido |

## 4. Mini-spec MVP (implementável)
**Response S→C** (cliente lê APENAS até Value — sem struct dedicada; `PHEADER_DEFAULT_SUBCODE` WSclient.h:129-134):
```
[0]=0xC1  [1]=Size(≥5)  [2]=0xF1  [3]=0x01  [4]=Value   (bytes ≥5 são ignorados neste handler)
```
**Request C→S** (derivado do corpo de CGConnectAccountRecv :1970-:2004): `ClientVersion[5]`, `ClientSerial`, `TickCount`, `account[10]` e `password[10]` **cifradros** (`PacketArgumentDecrypt`, buffers dest[11]) — layout exato: **[NOT RECOVERED]** (def de `PMSG_CONNECT_ACCOUNT_RECV` fora do escopo).
**Semântica**: sucesso = LogIn=2 + CheckHack(); falha = popup por enum; **qualquer Value desconhecido cai no default = FAIL_CONNECT** (robustez herdada).
**Nota NEW-ERA**: o GS @580472e já possui path alternativo `NEW_PROTOCOL_SYSTEM==1` → `gSocketManagerModern.PacketSend(…, ProtocolHead::BOTH_CONNECT_LOGIN, …)` (:3137-:3138) — stub de protocolo moderno no próprio legado.

## 5. Limitações
1. **Receiver JG→GS que mapeia `SDHP_CONNECT_ACCOUNT_SEND`/BlockCode→Value final: [NOT RECOVERED]** — GameServer local sem fontes de protocolo; exigiria novo download (novo comando de rede) do par GJ do GS @580472e.
2. Defs `PMSG_CONNECT_ACCOUNT_RECV/SEND` (headers GS): [NOT RECOVERED] no escopo.
3. Sender do request 0xF1:0x01 no cliente: **não localizado no WSclient.cpp** (0 envios de 0xF1 no arquivo — provável fonte de UI de login fora do escopo).
4. Mapeamentos 5/0x11/0x40/0x0a-0x0d/0xc0-0xd2: inferidos pelos campos JS (BlockCode/sno__numb/AccountLevel) — HIPÓTESE até recuperar (1).

## 6. Conclusão: **(1) par request/response fechado no que o MVP exige** (framing, 21 outcomes com classes, condições server provadas para 0/1/2/3/4/6, fallback default); lacunas marcadas [NOT RECOVERED]/HIPÓTESE, sem bloqueio para o MVP.
