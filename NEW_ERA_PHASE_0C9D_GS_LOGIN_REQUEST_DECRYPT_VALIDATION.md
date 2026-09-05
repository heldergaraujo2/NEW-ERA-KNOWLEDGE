# NEW-ERA — PHASE 0C.9-D: GS — decrypt/validação do request 0xF1:0x01 (CGConnectAccountRecv)
> Microteste 0C.9-D · 2026-09-04 · SEM REDE · fecha o lado servidor do request de login.

## 1. Fontes
| Fonte | Bytes | SHA256 |
|---|---|---|
| `NEW_ERA_EVIDENCE/GS_Protocol.cpp__580472e__sha256_e8bb4dbb.cpp` (principal) | 135.014 | `e8bb4dbb28d1baf23222dbc72762e2ebd55b116dbb8d36e5f017dd744e510f13` |
| `NEW_ERA_EVIDENCE/0E1L__13__WSclient…cpp` (BuxConvert) | 415.884 | `6538d944126181dfa77d4419bf992ac77151eec5411dc3ab243f2926fec868bf` |
| `NEW_ERA_EVIDENCE/0E1L__21__wsclientinline.h` (uso) | 62.684 | `67a6d5359bb702b38f2a43e7d8377f3c8e37b8af4f2a49b15b4417d4e98a0854` |

## 2. Fluxo do request (provado)
```
dispatch switch(head) case 0xF1 :942 → switch(lpMsg[3]) case 0x01 :945 → CGConnectAccountRecv :946
CGConnectAccountRecv :1960-:2006
  ├─ Connected != OBJECT_CONNECTED → CloseClient (kick, :1964-:1968)
  ├─ memcmp(m_ServerVersion, lpMsg->ClientVersion) != 0 → GCConnectAccountSend(aIndex,6); return  (:1970-:1974)
  ├─ memcmp(m_ServerSerial,  lpMsg->ClientSerial)  != 0 → GCConnectAccountSend(aIndex,6); return  (:1976-:1980)
  ├─ guard lpObj->LoginMessageSend == 0 (anti-replay: 1ª msg only, :1982-:1985)
  ├─ lpObj->ClientTickCount = lpMsg->TickCount (:1987)
  ├─ PacketArgumentDecrypt(account,  lpMsg->account,  10) (:1990-:1992)
  ├─ PacketArgumentDecrypt(password, lpMsg->password, 10) (:1994-:1996)
  └─ GJConnectAccountSend(aIndex,account,password,IpAddr) (:2004) → JoinServer (§37)
```

## 3. Campos lidos do `PMSG_CONNECT_ACCOUNT_RECV` (por evidência de acesso)
| Campo | Linha | Uso | Wire (spec 0C.9-C) |
|---|---|---|---|
| `ClientVersion[5]` | :1970 | `memcmp` vs `gServerInfo.m_ServerVersion` | `[VerEnc5]` (cliente envia `V[i]-(i+1)` — inline:263) |
| `ClientSerial[16]` | :1976 | `memcmp` vs `gServerInfo.m_ServerSerial` | `[Serial16]` cru (inline:265) |
| `TickCount` | :1987 | copiado para `lpObj->ClientTickCount` (sync de relógio) | `[Tick4]` |
| `account[10]` | :1992 | `PacketArgumentDecrypt(dest[11], src, 10)` | `[ID10]` BuxConvert’ed |
| `password[10]` | :1996 | idem | `[PASS10]` BuxConvert’ed |
**Struct [NOT RECOVERED]**: def fora do GS evidence e do repo local (GameServer local vazio). Tamanhos 10/10/4/5/16 consistentes com a spec 0C.9-C (≈49 B).

## 4. Decrypt — BuxConvert (cliente, provado) × PacketArgumentDecrypt (GS, def não recuperada)
**Cliente** (WSclient.cpp **:209-:215**):
```c
:209: static BYTE bBuxCode[3] = {0xfc,0xcf,0xab};
:211: void BuxConvert(BYTE *Buffer,int Size)
:213:     for(int i=0;i<Size;i++)
:214:         Buffer[i] ^= bBuxCode[i%3];
```
→ **XOR cíclico de 3 bytes** com chave {FC, CF, AB}. Aplicado em ID/senha ANTES do AddData (inline:257-:258 — evidência §38).
**Servidor**: `PacketArgumentDecrypt` — **somente 3 chamadas no GS evidence** (:1790 em `CGMapServerMoveAuthRecv` com size 11; :1992/:1996 no login com size 10); **def [NOT RECOVERED]** (não está no arquivo nem no repo local).
**Conclusão (HIPÓTESE FORTE, não fato)**: `PacketArgumentDecrypt` ≡ `BuxConvert` (mesmo papel de contrafunção, mesmos tamanhos 10 = MAX_ID/PASS, buffers dest[11] com NUL). O servidor **desfaz** o XOR do cliente — os dados viajam BuxConvert’ed no wire. Prova formal exige a def (outro arquivo GS — novo comando de rede).
**Nota de correção conceitual**: a hipótese H1 (chat-only, §21.1 — “XOR 32B”) NÃO se aplica a ID/senha: o ofuscador de credenciais provado é **XOR 3B** {FC,CF,AB}. H1 permanece NÃO-CANÔNICA; nenhuma seção do Ledger afirmava 32B para este fluxo.

## 5. Result codes — quem decide o quê
| result | Quem | Condição | Ação |
|---|---|---|---|
| kick | GS :1964 | `Connected != OBJECT_CONNECTED` | `CloseClient` (sem 0xF1) |
| **6** | **GS** :1970-:1974 / :1976-:1980 | version **ou** serial mismatch | `GCConnectAccountSend(aIndex,6)` + `return` — **aborta ANTES do JS** |
| 0/1/2/3/4 | JS (§37) | senha/ok/conta/já-conectado/cheio | repassado como Value (§37 1:1) |
- GS só encaminha ao JS se version+serial OK e `LoginMessageSend==0` (1ª mensagem — anti-replay).
- HIPÓTESE §38 mantida: `m_ServerVersion` armazena a forma codificada `V[i]-(i+1)` (conteúdo de `gServerInfo` [NOT RECOVERED]).

## 6. Limitações
1. `PMSG_CONNECT_ACCOUNT_RECV` def [NOT RECOVERED] (ordem/tamanho exatos inferidos pelos acessos + spec 0C.9-C).
2. `PacketArgumentDecrypt` def [NOT RECOVERED] — equivalência com BuxConvert é HIPÓTESE FORTE (§4).
3. `gServerInfo.m_ServerVersion/m_ServerSerial` (conteúdo/forma) [NOT RECOVERED].

## 7. Conclusão: **(1) validação server-side fechada com evidência** (result 6 = version||serial, abort pré-JS; anti-replay; decrypt 3-chamadas mapeadas; BuxConvert provado XOR 3B {FC,CF,AB}).
