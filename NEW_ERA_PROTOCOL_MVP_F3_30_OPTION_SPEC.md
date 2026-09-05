# NEW-ERA — PROTOCOL SPEC — MVP F3:0x30 OPTION DATA
> 1.1-E · 2026-09-05 · SEM REDE · offline. Status: NORMATIVA.

## 1. Fontes (NEW_ERA_EVIDENCE/)
| # | Fonte | Bytes | SHA256 | Papel |
|---|---|---|---|---|
| G1 | `GS_Protocol.cpp…e8bb4dbb.cpp` | 135.014 | `e8bb4dbb…` | dispatch :991-:993 |
| C1 | `0E1L__13__WSclient.cpp` | 415.884 | `6538d944…` | RX :9388-:9435+ |
| H | `0E1L__14__WSclient.h` | 91.012 | `19d38a3b…` | PRECEIVE_OPTION :1204-:1215 |
| **I** | `0E1L__21__wsclientinline.h` | 62.684 | `67a6d535…` | **TX macro :1597-:1603** |

## 2. Request C→S — `SendRequestHotKey(option)` (wsclientinline.h :1597-:1603)
```
spe.Init(0xC1,0xF3) << 0x30; spe.AddData(option, 30); spe.Send();
```
- Wire: **[C1][0x22][F3][30][option[30]] = 34 B** — payload de exatamente **30 B** fornecido pelo caller (espelha o payload do response: HotKey[20]+GameOption+KeyQWE[3]+ChatLogBox+KeyR+QWERLevel[4] = 30).
- Server: `CGOptionDataRecv((PMSG_OPTION_DATA_RECV*)lpMsg)` :991-:993 (def [NOT RECOVERED]).
- ⚠️ **Send() default = `bEncrypt=FALSE`** (StreamPacketEngine.h :120) ⇒ **no wire real este request vai C1 PLAIN** (apenas Xor de stream do AddData). O **C3 golden NEW-ERA** (builder Encrypt c/ Enc1) é **convenção nossa de teste do pipeline** — divergência deliberada, registrada no Ledger §60.
- XOR: AddData aplica o encadeado ⇒ XOR32 em [3..34) no plain.

## 3. Response S→C — `PRECEIVE_OPTION` (WSclient.h :1204-:1215, pack(1))
```
[C1][0x22][F3][30][HotKey[20]][GameOption][KeyQWE[3]][ChatLogBox][KeyR][QWERLevel(i32)] = 34 B
```
Consumo no cliente (`ReceiveOption` :9388-:9435+):
- `HotKey[20]` = **10 WORDs com par de bytes TROCADADO**: `iHotKey = MAKEWORD(HotKey[2i+1], HotKey[2i])` :9398 ⇒ valor = `HotKey[2i+1] | (HotKey[2i]<<8)`; `0xFFFF` = vazio (:9400).
- `GameOption`: bits `AUTOATTACK_ON`/`WHISPER_SOUND_ON`/`SLIDE_HELP_OFF` :9413-:9435 (valores dos defines [NOT RECOVERED] neste escopo — parser expõe o byte cru).
- `KeyQWE[3]`/`ChatLogBox`/`KeyR`/`QWERLevel`: campos da struct (:1209-:1214); consumo além de :9435 não auditado aqui — parser expõe crus.
- GS-style: Encrypt SEM Xor ⇒ `streamXored=false`; C3 57 B no golden (inner 33 B).

## 4. Formato NEW-ERA decidido
- `BuildC1_F3_30_OptionRequestPlain(const uint8_t option[30])` → 34 B + XOR32 [3..34).
- `BuildC3_F3_30_OptionRequestEncrypted(option, err)` → inner serial+32=33 B (4 blocos cheios + **parcial de 1 B**) → ct 55 → **C3 57 B**.
- `ParseC1_F3_30_OptionResponsePlain` → `ParsedOption{ hotKeys[10] (swap evidenciado :9398), gameOption, keyQWE[3], chatLogBox, keyR, qwerLevel }`; bounds-check; 34 B mínimos.

## 5. Descobertas colaterais (cross-check wsclientinline — Ledger §60)
- `SendRequestCharactersList` :288-:296: `<< 0x00 << byLanguage` ⇒ wire real = C1 **5 B** (1.1-B/C usou 4 B; server ignora lpMsg — divergência documentada).
- `SendRequestDeleteCharacter` :310-:320: `AddData(p_Resident, 20)` ⇒ Resident = **20 B no wire** (struct dizia 10; 1.1-D usou 10 — divergência documentada).
- Todos os 3 usam `Send()` ⇒ C1 plain no wire real (C3 só no login `Send(TRUE)`).
