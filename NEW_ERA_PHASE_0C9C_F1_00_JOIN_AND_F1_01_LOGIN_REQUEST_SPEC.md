# NEW-ERA — PHASE 0C.9-C: 0xF1:0x00 Join handshake (S→C) + request 0xF1:0x01 (C→S) — spec (offline)
> Microteste 0C.9-C (C1 extração read-only + C2 consolidação) · 2026-09-04 · SEM REDE.

## 1. Fontes e hashes
| Path (NEW_ERA_EVIDENCE/) | Bytes | SHA256 |
|---|---|---|
| `0E1L__13__WSclient…cpp` | 415.884 | `6538d944126181dfa77d4419bf992ac77151eec5411dc3ab243f2926fec868bf` |
| `0E1L__14__WSclient.h` | 91.012 | `19d38a3b6506a745662c14dd7cdd6e41ca3ad29b28b17c76f860cd9e67799c10` |
| `0E1L__21__wsclientinline.h` | 62.684 | `67a6d5359bb702b38f2a43e7d8377f3c8e37b8af4f2a49b15b4417d4e98a0854` |
| `GS_Protocol.cpp__580472e__sha256_e8bb4dbb.cpp` | 135.014 | `e8bb4dbb28d1baf23222dbc72762e2ebd55b116dbb8d36e5f017dd744e510f13` |

## 2. SPEC S→C — C1 / Head=0xF1 / Sub=0x00 (JoinServer handshake)
**Dispatch**: WSclient.cpp `case 0x00:` :12830-:12832 → `ReceiveJoinServer(ReceiveBuffer)` (def :392-:460).
**Struct dedicada** `PRECEIVE_JOIN_SERVER` (WSclient.h **:270-:279**):
```c
typedef struct {
    PBMSG_HEADER Header;                    // [0]=0xC1 [1]=Size [2]=0xF1
    BYTE SubCode;                           // [3]=0x00
    BYTE Result;                            // [4]
    BYTE NumberH;                           // [5]  HeroKey = (NumberH<<8)+NumberL
    BYTE NumberL;                           // [6]
    BYTE Version[SIZE_PROTOCOLVERSION];     // [7..11]  SIZE_PROTOCOLVERSION=5 (H:13)
} PRECEIVE_JOIN_SERVER, * LPPRECEIVE_JOIN_SERVER;   // total 12 bytes
```
**Emissor (GS evidence)**: `GCConnectClientSend2` :3067-:3092 (`set(0xF1,0x00)` **:3071**, `DataSend` :3089) e `GCConnectClientSend` :3094-:3125 (`set(0xF1,0x00)` **:3099** — `#if NEW_PROTOCOL_SYSTEM==0`). Mapeamento campo-a-campo: `pMsg.result`→Result [4] (:3102); `index[0]=SET_NUMBERHB(aIndex)`→NumberH (:3104), `index[1]=SET_NUMBERLB(aIndex)`→NumberL (:3106); `ClientVersion[0..4]=m_ServerVersion[0..4]`→Version[5] (:3108-:3116).

**Semântica (cliente)**:
- `LogIn!=0` (:396): `HeroKey=(NumberH<<8)+NumberL` :398 + `g_csMapServer.SendChangeMapServer()` :399 — **path de map-server move** (não mostra login).
- `LogIn==0`: `switch(Result)` :405 — `case 0x01` :407: `ShowWin(m_LoginWin)` :408, `HeroKey` :409, `CurrentProtocolState=RECEIVE_JOIN_SERVER_SUCCESS` :410 (enum=2, H:19); `default` :429: `ErrorReport "Connectting error."` :430 + `PopUpMsgWin(MESSAGE_SERVER_LOST)` :432.
- **Regra de versão**: :435-:443 `Version[i]-(i+1) != Data2->Version[i]` → `MESSAGE_VERSION` + `"Version dismatch - Join server."` :441 — ou seja, o byte recebido deve ser `V[i]-(i+1)` (codificação compensada; ver §3 — o request usa a MESMA codificação :263).
- Enums de estado (H:17-20): WAITING=1 · SUCCESS=2 · FAIL_VERSION=3.

## 3. SPEC C→S — C1 / Head=0xF1 / Sub=0x01 (Request login — macro `SendRequestLogIn`)
`wsclientinline.h` **:243-:269** (`#define SendRequestLogIn(p_lpszID, p_lpszPassword)`):
```
LogIn=1; strcpy(LogInID,…); CurrentProtocolState=REQUEST_LOG_IN(19)   (:245-:247)
CStreamPacketEngine spe; spe.Init(0xC1, 0xF1);                          (:248-:249)  → [0]=C1 [2]=0xF1
spe << (BYTE)0x01;                                                      (:250)       → [3]=SubCode
BuxConvert(lpszID); BuxConvert(lpszPass);                               (:257-:258)  → ofuscação ID/senha
spe.AddData(lpszID, MAX_ID_SIZE); spe.AddData(lpszPass, MAX_PASSWORD_SIZE) (:259-:260)
spe << GetTickCount();                                                  (:261)       → DWORD
for i<5: spe << (BYTE)(Version[i]-(i+1));                               (:262-:263)  → Version[5] CODIFICADA
for i<16: spe << Serial[i];                                             (:264-:265)  → Serial[16]
spe.Send(TRUE);                                                         (:266)
```
**Wire order**: `[C1][Size][F1][01][ID[MAX_ID_SIZE]][PASS[MAX_PASSWORD_SIZE]][TickCount(4LE)][VersionEnc[5]][Serial[16]]`.
**Tamanhos**: `SIZE_PROTOCOLVERSION=5`, `SIZE_PROTOCOLSERIAL=16` (WSclient.h:13-14); `MAX_ID_SIZE`/`MAX_PASSWORD_SIZE` defs fora do escopo — **inferência 10/10** pelos buffers do GS (`char account[11]`/`password[11]`, decrypt com tamanho 10, GS:1990-:1996) ⇒ Size esperado ≈ 4+10+10+4+5+16 = **49 (0x31)** [inferido].
**Contrapartes server-side**: `BuxConvert` ↔ `PacketArgumentDecrypt` (GS:1992/:1996 — mesma função XOR 32B da H1); `memcmp` de `ClientVersion`/`ClientSerial` (GS:1970/:1976) — nota: cliente envia Version **codificada** `V[i]-(i+1)`; hipótese (não verificada): `m_ServerVersion` do GS armazena a forma já codificada, ou a codificação é compensada na comparação.

## 4. Relação com 0C.9-B
**Fecha a lacuna “sender do request no cliente” do §37 (0C.9-B)** — item 3 das limitações: o request 0xF1:0x01 existe, é a macro `SendRequestLogIn` (wsclientinline.h:243), com layout completo agora documentado (inclui Serial, que a análise do corpo GS não revelava). Além disso, define a semântica exata do 0xF1:0x00 S→C (handshake pré-login + path de map-server move) e o emissor GS (`GCConnectClientSend`/`2`).

## 5. Limitações / próximo passo mínimo (1)
- `PMSG_CONNECT_ACCOUNT_RECV/SEND` e `PMSG_CONNECT_CLIENT_SEND(2)`: defs **fora do escopo** ([NOT RECOVERED]); tamanho exato do pacote S→C 0x00 (sizeof) não confirmável sem a def (cliente lê até [11]).
- Callers de `GCConnectClientSend*` no GS evidence (quem dispara o handshake) não mapeados.
- **Próximo sugerido**: mapear o decrypt/validação server-side do request 0xF1:0x01 (GS evidence: `PacketArgumentDecrypt`, memcmp version/serial :1970-:1979, e callers de `GCConnectClientSend*`).
