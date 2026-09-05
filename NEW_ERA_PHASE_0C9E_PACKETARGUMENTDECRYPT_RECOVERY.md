# NEW-ERA — PHASE 0C.9-E: PacketArgumentDecrypt — recuperação da definição e equivalência com BuxConvert (offline)
> Microteste 0C.9-E · 2026-09-04 · SEM REDE · escopo: `muonline/` + `NEW_ERA_EVIDENCE/` (excl. *.SHA256.txt).

## 1. Escopo analisado
- `muonline/` (repo READ-ONLY, ~4,2k arq — incl. Source Server, Source Tools, MuServer, notas legadas)
- `NEW_ERA_EVIDENCE/` (~168 arq, excl. sidecars .SHA256.txt)
- Padrões: `PacketArgumentDecrypt` · `ArgumentDecrypt` · `BuxConvert`

## 2. Ocorrências completas
### PacketArgumentDecrypt / ArgumentDecrypt — **3 hits, TODOS CALL** (uso)
| Arquivo | Linha | Classificação | Contexto |
|---|---|---|---|
| `NEW_ERA_EVIDENCE/GS_Protocol.cpp__580472e__sha256_e8bb4dbb.cpp` | :1790 | **CALL** | `PacketArgumentDecrypt(account,lpMsg->account,sizeof(account));` (CGMapServerMoveAuthRecv) |
| idem | :1992 | **CALL** | `PacketArgumentDecrypt(account,lpMsg->account,(sizeof(account)-1));` (login) |
| idem | :1996 | **CALL** | `PacketArgumentDecrypt(password,lpMsg->password,(sizeof(password)-1));` (login) |
**Protótipo: 0 · Definição: 0** (em todo o workspace). Nem header do GS evidence a declara (arquivo único).

### BuxConvert — definições e usos
| Arquivo | Linha | Classificação | Nota |
|---|---|---|---|
| `NEW_ERA_EVIDENCE/0E1L__13__WSclient…cpp` | :209-:215 | **DEFINIÇÃO (cliente, canônica §39)** | chave `{0xfc,0xcf,0xab}`, `Buffer[i]^=bBuxCode[i%3]` |
| **`muonline/Source Tools/Encoder/Encoder/Util.cpp`** | **:4-:12** | **DEFINIÇÃO (tool Encoder — INÉDITA)** | `static BYTE bBuxCode[3]={0xfc,0xcf,0xab};` :4 · `pbyBuffer[i]^=bBuxCode[i%3];` :10 — **algoritmo e chave IDÊNTICOS ao cliente** |
| `muonline/Source Tools/Encoder/Encoder/Util.h` | :3 | PROTÓTIPO | `extern void BuxConvert(BYTE*, int);` |
| `muonline/Source Tools/Encoder/Encoder/Encoder.cpp` | :270 | CALL | ofusca MAIN_FILE_INFO do encoder de Main |
| Demais hits (usos cliente em evidência): WSclient.cpp ×4, wsclientinline.h ×5, SocketSystem/ProtocolSend/UIWindows/ZzzInfomation/CSItemOption/MoveCommandData/GlobalText | — | CALL | — |

## 3. Definição encontrada?
**PacketArgumentDecrypt: NÃO** (não existe definição nem protótipo no workspace; apenas as 3 chamadas no GS evidence).
**BuxConvert (novo achado)**: definição independente no **Encoder tool** com **transform idêntica byte a byte** ao cliente:
```c
// Cliente (WSclient.cpp:209-:215)                // Tool (Encoder/Util.cpp:4-:12)
static BYTE bBuxCode[3]={0xfc,0xcf,0xab};         static BYTE bBuxCode[3]={0xfc,0xcf,0xab};
void BuxConvert(BYTE*Buffer,int Size){            void BuxConvert(BYTE*pbyBuffer,int Size){
    for(int i=0;i<Size;i++)                           for(int i=0;i<Size;i++)
        Buffer[i]^=bBuxCode[i%3];                         pbyBuffer[i]^=bBuxCode[i%3];
}                                                 }
```
SHA256 `Encoder/Util.cpp`: `e0424697087e107a7b5e90570626fc4e6b0321d3c1d4b2d023cd1154dacec7f8`.

## 4. Comparação e conclusão
**Conclusão: (3) NOT RECOVERED** — a definição de `PacketArgumentDecrypt` não existe no workspace/evidência (só calls: GS_Protocol.cpp:1790/:1992/:1996).
**Corroboração forte da HIPÓTESE §39b** (equivalência PacketArgumentDecrypt≡BuxConvert): `BuxConvert` é um **padrão de codebase** (2 definições independentes — cliente e tool Encoder do server-pack — com algoritmo e chave idênticos {FC,CF,AB}); notas legadas do repo (`analysis_report.md:168`, `fix_log_v3.md:114`, não-canônicas) afirmam o mesmo: “BuxConvert(账号密码)” client-side e “BuxConvert XOR key: {0xFC,0xCF,0xAB} 循环”. A equivalência permanece **HIPÓTESE FORTE**, não fato.

## 5. Implicação para o MVP de login
- **PROVADO**: transform do cliente (XOR 3B {FC,CF,AB}) e sua ubiqüidade no codebase (cliente + tools).
- **HIPÓTESE (risco marcado)**: server decodifica com a mesma transform — para o MVP, implementar `decrypt=BuxConvert` é a decisão de menor risco; prova formal exige a def (arquivo GS não baixado — só via novo comando de rede).
- **Divergência a registrar** (nota legada, não-canônica): `fix_log_v3.md:116` cita “login packet **55 bytes**” (nosso wire inferido ≈49 B §38) e `analysis_report.md:168` cita **SimpleModulus (8B→11B)** no transporte C3 — ambas relevantes para o próximo passo (encryption flags), com provenance duvidosa (notas do autor do repo, build possivelmente diferente).

## 6. Errata ortográfica (registrada, sem correção in-place)
§39 contém “**Validção**” (grafia) — confirmado no append; texto permanece intacto (append-only).

## 7. Artefatos
Este relatório · Ledger §40 · CURRENT_STATE/MASTER_CHECKPOINT.
