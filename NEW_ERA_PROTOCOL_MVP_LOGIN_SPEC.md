# NEW-ERA — PROTOCOL SPEC (NORMATIVA) — MVP LOGIN (fluxo 0xF1)
> 0C.11-A · 2026-09-04 · consolidado de 0C.9-A..F + 0C.10-A/B (Ledger §36-§43) · commit **580472e** · SEM REDE.
> Status: **NORMATIVA para implementação** — itens [NOT RECOVERED]/[INFERIDO] explicitamente marcados.

## 1. Escopo e premissas
- **Build alvo**: `ENCRYPT_STATE=0` (GS stdafx.h:58-59, não sobrescrito no vcxproj) ⇒ whitening OFF; cripto ativa = C3/C4 (CPacketManager).
- **Fontes primárias** (tudo citado como arquivo:linha; relatórios 0C9B/C/D/F, 0C10A/B são cross-ref):

| # | Fonte (NEW_ERA_EVIDENCE/ salvo indicação) | Bytes | SHA256 |
|---|---|---|---|
| C1 | `0E1L__13__WSclient…cpp` | 415.884 | `6538d944126181dfa77d4419bf992ac77151eec5411dc3ab243f2926fec868bf` |
| C2 | `0E1L__14__WSclient.h` | 91.012 | `19d38a3b6506a745662c14dd7cdd6e41ca3ad29b28b17c76f860cd9e67799c10` |
| C3 | `0E1L__21__wsclientinline.h` | 62.684 | `67a6d5359bb702b38f2a43e7d8377f3c8e37b8af4f2a49b15b4417d4e98a0854` |
| C4 | `0E1L__10__StreamPacketEngine.h` | 2.603 | `797d26d9678852e2f586df967a090b451d1d5b07d97c6fba7ddbe21d39c2e684` |
| C5 | `0E1JN3__7__Winmain.cpp` | 45.584 | `8e77ee5c1f0b993946cfa25ebfd534b353aeee9c5e070bc1446f65354c428e5d` |
| G1 | `GS_Protocol.cpp__580472e…` | 135.014 | `e8bb4dbb28d1baf23222dbc72762e2ebd55b116dbb8d36e5f017dd744e510f13` |
| G2 | `0C9F__Util…cpp` | 11.245 | `6271b7befc4a905ec3a1b7a66163502971c79981557b859f546cb08f3d53d395` |
| G3 | `0C10A__PacketManager…cpp` | 11.001 | `3e82e5d21ecfb84367a4139293aedb0b3146a2f7e221ffe03d45c2499006cfe4` |
| G4 | `0C10B__SocketManager…cpp` | 21.153 | `8492b66a5954d7b2a910dae36dae57cb2c80b40da7c9404fe93b69e0d7e64408` |
| G5 | `0C10B__stdafx…h` | 2.590 | `d23c35c3c250502e3777810827ed689f6c3c8dad5312ee6de3c34cd82a0cce0b` |
| J1 | `muonline/Source Server/JoinServer/JoinServer/JoinServerProtocol.cpp` (repo, READ-ONLY) | 17.474 | `531fb3d527be9a6291fb957dbf9397856134bf1b8b68075332991e6da17f2446` |

- Premissas marcadas: MAX_ID/MAX_PASSWORD = 10 [INFERIDO — buffers GS :1990-:1996; defs fora do escopo]; tamanho 49 B do request [INFERIDO idem].

## 2. Visão geral do fluxo
```
 GS ──(1) C1 0xF1:00 JoinServer (12 B) ─────────────▶ cliente     [GCConnectClientSend/2  G1:3067-:3125]
 cliente ──(2) C3 0xF1:01 login (≈49 B lógicos) ───▶ GS           [SendRequestLogIn  C3:243-:269]
        GS: version/serial (→Value 6) · anti-replay · Bux-decrypt  [G1:1960-:2006]
        GS ── GJConnectAccountSend (interno, J1:76-:254) ──▶ JoinServer: result 0/1/2/3/4
 GS ◀── JS result ── (3) C1 0xF1:01 login-result [Value] ──▶ cliente [GCConnectAccountSend G1:3127-:3155]
 cliente: switch(Value) → sucesso (LogIn=2) | popup de falha        [C1:12833-:12906]
```
- (1) também ocorre no path **map-server move** (cliente `LogIn!=0`: HeroKey+SendChangeMapServer, C1:396-:399).
- (3) sai como **C1 plain** neste build (header C1 do caller + `ENCRYPT_STATE=0`; DataSend G4:417-:470 só re-encripta heads C3/C4).

## 3. Framing e criptografia (ordem exata, TX login C→S)
1. **Credenciais**: `BuxConvert` XOR cíclico 3 B, chave `{0xFC,0xCF,0xAB}` (C1:209-:215; aplicado inline:257-:258) — server desfaz com `PacketArgumentDecrypt` ≡ (G2:101-:109, tabela local `XorTable[3]` idêntica — §41 PROVADO).
2. **Stream builder**: `CStreamPacketEngine` monta pacote interno **C1** `Init(0xC1,0xF1)` (C4:29-:46); cada campo `AddData(bXor=TRUE)` aplica **XOR encadeado** `buf[i] ^= buf[i-1]^Filter[i%32]` (C4:66-:80) com tabela 32 B `E7 6D 3A 89 BC B2 9F 73 23 A8 FE B6 49 5D 39 5D 8A CB 63 8D EA 7D 2B 5F C3 B1 E9 83 29 51 E8 56` — **32/32 idêntica** ao `m_XorFilter` do GS (G3:85-:116).
3. **Serial**: byte de serial do cliente inserido (`g_byPacketSerialSend++`, C3:92-:95); GS valida por conexão (`gSerialCheck`) e S→C devolve serial próprio (G4:449/:462; check cliente C1:11703 — falha ⇒ logging + `SendHackingChecked`).
4. **Cipher de bloco (SimpleModulus/CPacketManager)**: por bloco, `Enc = ((Key·d) % Modulus) ^ Xor` encadeado (G3:337-:343); decrypt espelhado (G3:396-:403); chaves em arquivos: cliente `Data\Enc1.dat`/`Dec2.dat` (C5:1459-:1460), GS `Load*Key` formato `ENCDEC_HEADER+ENCDEC_DATA{Modulus,Key,Xor}[4]` com tabelas ofuscadas (`^m_SaveLoadXor`, G3:185-:192). Camada extra Crypto++ DES (`DES_XEX3`) se `GAMESERVER_UPDATE>=701` (G3 ctor :29-:30/.h:9-:13) [valor de build NOT RECOVERED].
5. **Envelope**: ciphertext <256 B e !bForceC4 ⇒ **C3** `[0xC3][len][ct…]` (C3:102-:112); senão **C4** `[0xC4][lenL][lenH][ct…]` (:114-:126). Login ≈49 B ⇒ **C3** [premissa 49 B].
- RX simétrico no cliente (C1:11679-:11710) e no GS (G4:271-:371 → `ProtocolCore` :882 com `encrypt`+`serial`).

## 4. Layouts de pacotes (byte-level)
### 4.1 S→C `0xF1:0x00` — JoinServer handshake — **12 B** (struct `PRECEIVE_JOIN_SERVER` C2:270-:279)
| Off | Campo | Valor/semântica |
|---|---|---|
| 0 | Code | 0xC1 |
| 1 | Size | 0x0C |
| 2 | Head | 0xF1 |
| 3 | Sub | 0x00 |
| 4 | Result | 0x01 = SUCCESS (mostra LoginWin) · outro ⇒ `MESSAGE_SERVER_LOST` (C1:405-:433) |
| 5-6 | NumberH/NumberL | `HeroKey=(H<<8)+L` (C1:398/:409) |
| 7-11 | Version[5] | cliente exige `Version[i]-(i+1)` (C1:435-:443); GS envia `m_ServerVersion` TAL QUAL (G1:3108-:3116) ⇒ **HIPÓTESE**: m_ServerVersion armazena forma codificada [gServerInfo NOT RECOVERED] |
### 4.2 C→S `0xF1:0x01` — login request — **49 B lógicos** [INFERIDO 10/10] (C3:243-:266)
| Off | Campo | Transform |
|---|---|---|
| 0-3 | C1,size,0xF1,0x01 | header interno (vira payload do C3) |
| 4-13 | ID[10] | `^BuxConvert` (XOR3) |
| 14-23 | Password[10] | `^BuxConvert` |
| 24-27 | TickCount (DWORD LE) | cru (C3:261) |
| 28-32 | VersionEnc[5] | `Version[i]-(i+1)` (C3:263) |
| 33-48 | Serial[16] | cru (C3:265; SIZE_PROTOCOLSERIAL=16 C2:14) |
Após o build: +serial byte, XOR-encadeado por campo, SimpleModulus ⇒ **C3** (§3).
### 4.3 S→C `0xF1:0x01` — login result (G1:3127-:3142)
`[C1][Size≥5][0xF1][0x01][Value]` — bytes ≥5 ignorados pelo handler (C1:12835).

## 5. Validações server-side (GS = G1; JS = J1)
| Etapa | Linha | Regra |
|---|---|---|
| GS kick | G1:1964-:1968 | `Connected != OBJECT_CONNECTED` → CloseClient (sem 0xF1) |
| GS **Value 6** | G1:1970-:1974 / :1976-:1980 | `memcmp` ClientVersion **ou** ClientSerial ≠ config → `GCConnectAccountSend(aIndex,6)` + return (aborta pré-JS) |
| GS anti-replay | G1:1982-:1985 | `LoginMessageSend==0` (1ª msg apenas) |
| GS clock sync | G1:1987 | `ClientTickCount = lpMsg->TickCount` |
| GS decrypt | G1:1990-:1996 | `PacketArgumentDecrypt(…,10)` em ID/senha |
| JS result **0** (senha) | J1:123-:129 / :154-:160 | strcmp/MD5 mismatch (aceita `GlobalPassword` — **não reproduzir no NEW-ERA**, nota de segurança) |
| JS result **2** (conta) | J1:88-:93/:111-:117/:140-:146/:170-:176/:178-:184/:192-:198 | sintaxe inválida/conta inexistente/DB |
| JS result **3** (já conectado) | J1:219-:225 | + `JGAccountAlreadyConnectedSend` (kick do outro GS) |
| JS result **4** (cheio) | J1:95-:107 | `GetAccountCount() >= MAX_ACCOUNT` |
| JS result **1** (ok) | J1:86/:227-:253 | `WZ_CONNECT_MEMB` + AccountInfo (BlockCode :188, sno__numb :186, AccountLevel :200, Lock :206-:215) |
- Mapeamento result→Value **1:1** para 0/1/2/3/4/6 (§37). Values 5/0x11/0x40/0x0a-0x0d/0xc0-0xd2: emissor presumido no receiver JG→GS [NOT RECOVERED].

## 6. Tabela canônica de Value codes (cliente C1:12835-:12906; enums C2:27-:46)
| Value | Significado | Ação cliente | Neste build |
|---|---|---|---|
| **0x01** | login OK | `LogIn=2; CheckHack()` :12845-:12849 | **EMITIDO** (JS 1) |
| **0x20** | login OK (alt) | idem :12837-:12841 | compat [emissor NOT RECOVERED] |
| **0x00** | senha errada | popup 21 :12842 | **EMITIDO** (JS 0) |
| **0x02** | conta inválida/inexistente | popup 22 :12850 | **EMITIDO** (JS 2) |
| **0x03** | já conectado | popup 23 :12853 | **EMITIDO** (JS 3) |
| **0x04** | servidor cheio | popup 24 :12856 | **EMITIDO** (JS 4) |
| **0x05** | conta bloqueada | popup 25 :12859 | compat (BlockCode→?) |
| **0x06** | versão mismatch | popup 26 + ErrorReport :12862 | **EMITIDO** (GS 6) |
| **0x07/default** | falha genérica | popup 27 :12866-:12869 | **DEFAULT obrigatório** |
| 0x08/0x09/0x0a-0x0d | erro/pagamento/tempos | popups 28/37/29-32 :12870-:12887 | compat |
| 0x11 | menor de 15 | popup 38 :12888 | compat (sno__numb) |
| 0x40 | canal pago | popup 39 :12891 | compat (AccountLevel) |
| 0xc0/0xd0 · 0xc1/0xd1 · 0xc2/0xd2 | point-date · point-hour · IP inválido | popups 34/35/36 :12894-:12905 | compat |

## 7. Implementation Notes (NEW-ERA)
**Reproduzir (compat mínima)**: framing C1/C3(+C4 RX) · serial por pacote/conexão · XorData 32 B (tabela §3) · SimpleModulus por bloco (fórmula §3; chaves Enc1/Dec2 ou regeneradas) · BuxConvert 3 B · layouts §4 · Values núcleo {0,1,2,3,4,6}+0x20+**default 0x07** · validações §5 (sem GlobalPassword) · anti-replay.
**Stub permitido**: Values compat (5/0x08-0x0d/0x11/0x40/0xc0-0xd2 → popup genérico) · DES ≥701 (se build não usar) · HackPacketCheck (auditoria opcional) · procedures WZ_* (substituíveis).
**Abertos [NOT RECOVERED]**: `gServerInfo.m_ServerVersion` (forma codificada?) · `PMSG_CONNECT_ACCOUNT_RECV/SEND` defs · receiver JG→GS (mapeamento BlockCode/sno__numb/AccountLevel→Values) · defs MAX_ID/MAX_PASSWORD (10 inferido) · conteúdo de Enc1.dat/Dec2.dat.
**Referência rápida dos fluxos derivados**: create-account 0x12 (C1:12914-:12927), logout 0x02, passwords 0x03-0x05 — fora do escopo deste MVP.
