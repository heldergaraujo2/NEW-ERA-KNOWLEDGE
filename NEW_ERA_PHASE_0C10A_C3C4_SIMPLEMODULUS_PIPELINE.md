# NEW-ERA — PHASE 0C.10-A: Pipeline C3/C4 + SimpleModulus (cliente + GS)
> Microteste 0C.10-A · 2026-09-04 · offline-first + REDE AUTORIZADA (1 Contents HTTP 200 + **4/6 raw**, seleção determinística; SimpleModulus.cpp/.h NÃO existem no GS dir).

## 1. Fontes
| Fonte | SHA256 |
|---|---|
| `0E1L__13__WSclient…cpp` / `0E1L__21__wsclientinline.h` / `0E1L__14__WSclient.h` / `0E1JN3__7__Winmain.cpp` (cliente, prévias) | 6538d944… / 67a6d535… / 19d38a3b… / (0E1JN3) |
| `GS_Protocol.cpp__580472e…` (prévia) | e8bb4dbb… |
| **`0C10A__PacketManager…cpp`** (11.001 B) **novo** | `3e82e5d21ecfb843…` (sidecar) |
| **`0C10A__PacketManager…h`** (1.521 B) **novo** | `09c33b87d54b2953…` |
| **`0C10A__HackPacketCheck…cpp`** (4.695 B) **novo** | `74467012d5881e35…` |
| **`0C10A__HackPacketCheck…h`** (1.347 B) **novo** | `2b08a8cba8162f12…` |
| `0C9F__Util…cpp` (prévia) | 6271b7be… |

## 2. CLIENTE (offline) — provado
- **Init(0xC3/0xC4) nos sends: ZERO** (inline.h: 193× C1, 1× C2; WSclient.cpp: 0) — **C3/C4 não é construído pelo builder**, e sim pelo **wrapper `SendPacket`** (inline.h:67-:127): `bEncrypt=FALSE` → envia C1/C2 cru (`sSend` :83-:86); `bEncrypt=TRUE` → `g_SimpleModulusCS.Encrypt` :100 e monta **C3** (`bc.Code=0xC3` :106, se cifrado <256 B e !bForceC4) ou **C4** (:118), com serial de pacote (`g_byPacketSerialSend++` :94) e byte aleatório de padding (:90).
- **SimpleModulus no cliente: SIM** — `CSimpleModulus g_SimpleModulusCS/SC` (WSclient.cpp:123-:124); chaves carregadas incondicionalmente no boot: `LoadEncryptionKey("Data\Enc1.dat")` / `LoadDecryptionKey("Data\Dec2.dat")` (Winmain:1459-:1460; também de buffer :685/:708).
- **RX** (WSclient.cpp:11679-:11710): C3/C4 → `g_SimpleModulusSC.Decrypt` (:11686/:11692) → `bEncrypted=TRUE`; serial checado (:11703); falha → `SendHackingChecked(0x06,0)` + descarta (:11696-:11700).
- **MVP login**: request 0xF1:0x01 (macro `SendRequestLogIn`) termina em `spe.Send(TRUE)` (inline:266) → **C3 presumido** [inferência: def de `CStreamPacketEngine::Send` (StreamPacketEngine.h) fora do escopo]; response 0xF1:0x00 chega por DataSend GS (cliente aceita C1 e C3/C4 no RX).

## 3. GS — provado (4 novos arquivos)
- **`case 0xC3/0xC4` em GS_Protocol.cpp:670/:673 são FALSOS POSITIVOS** — heads do namespace friend (FriendDel/FriendState), não transporte.
- **Motor de cifra GS = `CPacketManager`** (PacketManager.cpp): `Encrypt` :214 / `Decrypt` :268 / `EncryptBlock` :319 / `DecryptBlock` :369 — aritmética de **modulo** clássica (`((Key·data)%Modulus)^Xor` :337/:403) + tabelas `XorFilter[32]` (:88-:116); chaves em ARQUIVOS (`LoadEncryptionKey/LoadDecryptionKey` :121/:134, formato `ENCDEC_HEADER+ENCDEC_DATA{Modulus,Key,Xor}[4]`, tabelas ofuscadas `m_SaveLoadXor^table` :191) — **análogo ao Enc1.dat/Dec2.dat do cliente**. GAMESERVER_UPDATE≥701 adiciona camada **Crypto++ DES** (`DES_XEX3`, .h:9-:13, ctor :29-:30).
- **Flag de build**: `#if(ENCRYPT_STATE==1) EncryptData(lpMsg,size); #endif` dentro de `DataSendSocket` (0C9F Util.cpp:240-:244) — gate compile-time do S→C (valor de ENCRYPT_STATE no build: **[NOT RECOVERED]**; def de `EncryptData`: fora dos arquivos baixados).
- **Validação por pacote**: `HackPacketCheck` lê script com campos `Index/Value/Encrypt/MaxDelay/MinCount` (:71-:79) e **rejeita pacote se o flag Encrypt esperado ≠ recebido** (:141-:143, log "[HackPacketCheck] Packet encryption error (Encrypt: [esp][recv])") — cripto é expectation auditável, não opcional, quando configurada.

## 4. Conclusão: **(1) TRANSPORTE C3/C4 ATIVO E NECESSÁRIO** (com ressalvas)
Evidência: cliente carrega chaves incondicionalmente no boot, mantém máquina de serial TX/RX e reporta hacking em falha de decrypt — pipeline load-bearing; GS tem motor completo (modulo + DES≥701), gate `ENCRYPT_STATE` e validação per-packet (HackPacketCheck).
**Ressalvas [NOT RECOVERED]**: (a) valor de `ENCRYPT_STATE` no build; (b) ponto exato do dispatch C3/C4 no recv do GS (arquivo fora da seleção permitida — ex. camada de socket); (c) semântica exata de `spe.Send(TRUE)`; (d) crypto do `DataSend` (só `DataSendSocket` tem gate provado).

## 5. MVP login
- Cliente novo DEVE: falar C1 (texto) e C3 (quando bEncrypt) com SimpleModulus + serial por pacote; carregar Enc1/Dec2 equivalentes; tolerar C1/C3/C4 no RX.
- Nota legada §40 (“login 55 B”, “8B→11B”) segue não-canônica; expansão 8→11 B é consistente com block-cipher do motor (EncryptBlock por blocos), sem confirmação exata.

## 6. Artefatos
Este relatório · 4 evidências `0C10A__*` (+.SHA256) · Ledger §42 · CURRENT_STATE/MASTER_CHECKPOINT.
