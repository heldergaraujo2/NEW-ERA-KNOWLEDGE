# NEW-ERA — PROTOCOL SPEC — MVP F3:0x00 CHARACTER LIST (WIRE-REAL)
> 1.2-A1 · 2026-09-05 · SEM REDE · offline. Status: NORMATIVA (supersede o formato C3 de teste usado em 1.1-B/C — payload/response inalterados).

## 1. Fontes (NEW_ERA_EVIDENCE/)
| # | Fonte | Bytes | SHA256 | Papel |
|---|---|---|---|---|
| I | `0E1L__21__wsclientinline.h` | 62.684 | `67a6d535…` | TX :288-:296 |
| C1 | `0E1L__13__WSclient.cpp` | 415.884 | `6538d944…` | RX :523-:600 |
| H | `0E1L__14__WSclient.h` | 91.012 | `19d38a3b…` | structs :129-:146/:355-:364 |
| G1 | `GS_Protocol.cpp…e8bb4dbb.cpp` | 135.014 | `e8bb4dbb…` | dispatch :958-:960 |

## 2. Request C→S — WIRE REAL (wsclientinline.h :288-:296)
```
#define SendRequestCharactersList( byLanguage )
  spe.Init(0xC1,0xF3) << (BYTE)0x00 << (BYTE)byLanguage; spe.Send();
```
- **[C1][05][F3][00][byLanguage] = 5 B** (subcode 0x00 :293; byLanguage 1 B :294).
- `Send()` sem argumento = default `bEncrypt=FALSE` (StreamPacketEngine.h :120) ⇒ **C1 plain no wire**.
- Stream-xor: o builder legado encadeia XOR32 a partir de [3] (AddData bXor=TRUE, §43) ⇒ bytes [3..5) viajam XORed no wire (subcode + byLanguage); golden fixa os bytes exatos. Não há outro transform (sem BuxConvert/Encrypt neste request).
- Server: `case 0x00: CGCharacterListRecv(aIndex);` (GS :958-:960 — lpMsg NÃO é lido; server ignora o payload).

## 3. Response S→C (inalterado vs 1.1-B)
`PHEADER_DEFAULT_CHARACTER_LIST` (H :137-:146): [C1][sz][F3][00][MaxClass][MoveCount][**Value=count**] + N slots `PRECEIVE_CHARACTER_LIST` de **33 B** (:355-:364; MAX_ID_SIZE=10, EQUIPMENT_LENGTH=17 — provados §54); loop do `ReceiveCharacterList` (:529/:537-:575); Index>4 ⇒ aborta (:560). Exemplo 1 char: **C1 40 B**.

## 4. Nota de migração
C3 foi usado apenas nos testes antigos (1.1-C); **golden/loopback passam a ser C1 wire-real** (este passo). O builder C3 de teste permanece no core marcado DEPRECATED.

## 5. Formato NEW-ERA
- `BuildC1_F3_00_RequestCharListWire(uint8_t byLanguage)` → 5 B + XOR32 [3..5).
- Parser `ParseC1_F3_00_CharacterListPlain` inalterado (consome o response).
