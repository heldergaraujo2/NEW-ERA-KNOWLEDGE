# NEW-ERA — PHASE 0C.10-B: Fechamento — RECV C3/C4 no GS + ENCRYPT_STATE + prova do C3 no login
> Microteste 0C.10-B · 2026-09-04 · offline (cliente) + REDE AUTORIZADA (1 Contents HTTP 200 + **6/6 raw**, early-stop em `#define ENCRYPT_STATE 0` no último candidato).

## 1. Fontes
| Fonte | SHA256 |
|---|---|
| `0E1L__21__wsclientinline.h` / `0E1L__10__StreamPacketEngine…h` (cliente, prévias) | 67a6d535… / `797d26d9678852e2f586df967a090b451d1d5b07d97c6fba7ddbe21d39c2e684` |
| **`0C10B__SocketManager…cpp`** (21.153 B) **novo** | `8492b66a5954d7b2…` |
| **`0C10B__SocketManagerModern…cpp`** (4.859 B) **novo** | `82f0629c5c0007fa…` |
| **`0C10B__stdafx…h`** (2.590 B) **novo** | `d23c35c3c250502e…` |
| **`0C10B__GameServer…vcxproj`** (30.262 B) **novo** | `3a3a057d9b7db3d2…` |
| (descartados sem tokens: SocketManager.h, SocketManagerModern.h — não persistidos) | — |

## 2. CLIENTE — prova C3 para o request 0xF1:0x01 (sem inferência de caminho)
```
SendRequestLogIn (inline:243) … spe.Send( TRUE);                     (:266 — 1 arg ⇒ bForceC4=FALSE)
CStreamPacketEngine::Send(BOOL,FALSE) → SendPacket(m_byBuffer,m_wSize,TRUE,FALSE)  (StreamPacketEngine.h:120-:123)
SendPacket (inline:67): bEncrypt=TRUE → g_SimpleModulusCS.Encrypt :100
  se iSize<256 && bForceC4==FALSE  (:102) → bc.Code=0xC3  (:106)   [senão 0xC4 :118]
```
Login ≈49 B plain (§38) → cifrado ≈67 B < 256 ⇒ **sai como C3 — PROVADO** (decisão por tamanho no código; única premissa = tamanho 49 B do §38, marcado como inferido).

## 3. GS — RECV pipeline (provado, `SocketManager.cpp` novo)
```
IOCP recv :590-:598  [#if(ENCRYPT_STATE==1) DecryptData — gate OFF (ver §4)] → DataRecv :598
parse: C1/C3 → size@1 :271 · C2/C4 → size@2-3 :277
C3: gPacketManager.Decrypt(&DecBuff[1], &lpMsg[count+2], size-2) :301 → AddData(serial) :311 → ExtractPacket :311
C4: Decrypt(+3) :335 → AddData/ExtractPacket :346
C1/C2 plain: AddData + ExtractPacket :371
queue → worker: ProtocolCore(head, buff, size, index, encrypt, serial)  :882  ← ENTREGA AO DISPATCH (§23)
```
**S→C (`DataSend` :417-:470)**: se o buffer já é C3/C4 → re-encripta com **serial por conexão** (`gSerialCheck.GetSendSerial()` :449/:462 — par do `g_byPacketSerialRecv` do cliente WSclient.cpp:11703); C1 segue plain (gate :482 ver §4).

## 4. ENCRYPT_STATE — FECHADO
`stdafx.h` **:58-59**: `#ifndef ENCRYPT_STATE / #define ENCRYPT_STATE 0` → **ENCRYPT_STATE = 0** (default de compilação; **não sobrescrito** no GameServer.vcxproj — PreprocessorDefinitions sem ENCRYPT_STATE). Conseqüências: gates OFF em `DecryptData` recv (SocketManager.cpp:590-594), `EncryptData` DataSendSocket (Util.cpp:240-244) e DataSend (:482) — **a camada ENCRYPT_STATE é transport-whitening desativada; a cripto ativa do pipeline é a do CPacketManager (C3/C4)**.

## 5. BÔNUS estrutural (refina a spec §38)
`CStreamPacketEngine::XorData` (StreamPacketEngine.h:66-:80): **XOR encadeado** `buf[i] ^= buf[i-1]^filter[i%32]` aplicado a CADA campo streamado (`AddData` bXor=TRUE :82-:94) — tabela 32 B `E7 6D 3A 89 BC B2 9F 73 23 A8 FE B6 49 5D 39 5D 8A CB 63 8D EA 7D 2B 5F C3 B1 E9 83 29 51 E8 56` **≡ `m_XorFilter` do CPacketManager GS (PacketManager.cpp:85-:116), 32/32 bytes idênticos**. Ordem física do login request: BuxConvert(ID/PASS) → XOR-encadeado (stream) → SimpleModulus → C3. Layout lógico §38 inalterado; bytes físicos têm 2 camadas além do BuxConvert.

## 6. Conclusão: **(1) PIPELINE FECHADO + ENCRYPT_STATE FECHADO (=0)**
Ressalvas §42 encerradas: recv GS (SocketManager.cpp:271-:371→:882), ENCRYPT_STATE (0, stdafx.h:59), `spe.Send(TRUE)`→C3 (§2), crypto DataSend (serial-only p/ C3/C4; gates ENCRYPT_STATE off). Nota: `SocketManagerModern.cpp` (novo em evidência) = caminho NEW_PROTOCOL_SYSTEM com ExtractPacket próprio (82f0629c…).

## 7. Artefatos
Este relatório · 4 evidências `0C10B__*` (+.SHA256) · Ledger §43 · CURRENT_STATE/MASTER_CHECKPOINT.
