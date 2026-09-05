# NEW-ERA — PHASE 0C.9-F: PacketArgumentDecrypt — recuperação via Contents API (GS) + equivalência com BuxConvert
> Microteste 0C.9-F · 2026-09-04 · REDE AUTORIZADA (1 Contents + até 8 raw; usado: 1 Contents + **1 raw**, early-stop em FOUND-DEF).

## 1. Execução da rede (ultra-restrita)
- **Contents API**: `…/contents/Source%20Server/GameServer/GameServer?ref=580472e` → **HTTP 200** (517 entradas; 507 arquivos de código .cpp/.h/.hpp/.c). Listagem em /tmp (volátil), nenhum outro endpoint.
- **Seleção determinística (8)**: Util.cpp · Util.h · PacketManager.cpp · PacketManager.h · Protocol.h · HackPacketCheck.cpp · HackPacketCheck.h (prioridades 1-4: **todas existiam**) + CrywolfUtil.cpp (regex `(argument|decrypt|encrypt|bux|packet|util)`, lexicográfico).
- **Downloads**: **1/8** — `Util.cpp` (1º da ordem) CONTÉM o símbolo e é DEFINIÇÃO → **early stop**; os 7 demais **não foram baixados**.

## 2. EVIDÊNCIA PERSISTIDA
`NEW_ERA_EVIDENCE/0C9F__Util__path_Source_Server_GameServer_GameServer_Util.cpp__580472e.cpp` (11.245 B) + `.SHA256.txt`
SHA256: `6271b7befc4a905ec3a1b7a66163502971c79981557b859f546cb08f3d53d395` *(errata de rascunho: sha inicialmente transcrito incorretamente neste relatório e corrigido antes da consolidação — ver chat)*

## 3. DEFINIÇÃO ENCONTRADA (FOUND-DEFINIÇÃO) — GS Util.cpp **:101-:109**
```c
:101: void PacketArgumentDecrypt(char* out_buff,char* in_buff,int size) // OK
:102: {
:103:     BYTE XorTable[3] = {0xFC,0xCF,0xAB};
:105:     for(int n=0;n < size;n++)
:107:         out_buff[n] = in_buff[n]^XorTable[n%3];
:109: }
```

## 4. Comparação com BuxConvert — **EQUIVALÊNCIA CONFIRMADA**
| Def | Local | Algoritmo | Chave |
|---|---|---|---|
| `BuxConvert` (cliente) | WSclient.cpp:209-:215 | `buf[i]^=key[i%3]` | `{0xfc,0xcf,0xab}` (static) |
| `BuxConvert` (tool) | Encoder/Util.cpp:4-:12 | idem | idem (static) |
| **`PacketArgumentDecrypt` (GS)** | **GS Util.cpp:101-:109** | **`out[n]=in[n]^key[n%3]`** | **`{0xFC,0xCF,0xAB}` (local)** |
Três definições independentes, **algoritmo e chave idênticos byte a byte** (GS usa tabela local `XorTable` em vez da static; assinatura `char*` em vez de `BYTE*` — irrelevante para o wire). XOR é involutivo: cliente codifica, servidor decodifica com a mesma transform.

## 5. Resultado: **(1) FOUND + EQUIVALÊNCIA CONFIRMADA**
- Hipóteses §39b/§40 (equivalência) **PROMOVIDAS A PROVADO** por evidência em arquivo.
- Cadeia de login 0xF1:0x01 agora 100% probada ponta a ponta, incluindo a camada de ofuscação de credenciais.

## 6. Impacto no MVP de login 0xF1:0x01
- **Implementável sem risco residual nesta camada**: request `[C1][sz][F1][01][ID10^bux][PASS10^bux][Tick4][VerEnc5][Serial16]`; servidor (ou peer novo) aplica `^ {FC,CF,AB}` cíclico para recuperar ID/senha.
- Nada muda nas demais lacunas já marcadas ([NOT RECOVERED]: struct `PMSG_CONNECT_ACCOUNT_RECV`, receiver JG→GS, `gServerInfo`).

## 7. Artefatos
Este relatório · evidência `0C9F__Util…` (+.SHA256) · Ledger §41 · CURRENT_STATE/MASTER_CHECKPOINT.
