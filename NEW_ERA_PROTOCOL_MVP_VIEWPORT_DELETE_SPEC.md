# NEW-ERA — PROTOCOL SPEC — MVP VIEWPORT DELETE (0x14 ReceiveDeleteCharacterViewport)
> 1.3-I P1/2 · 2026-09-05 · SEM REDE. Status: NORMATIVA — classificação **(1) layout provado** (struct + loop completos).

## 1. Fontes (somente leitura)
| # | Fonte | SHA256 | Papel |
|---|---|---|---|
| C1 | `0E1L__13__WSclient.cpp` | `6538d944…` | handler :2793-:2848; dispatch :13116-:13119 |
| H | `0E1L__14__WSclient.h` | `19d38a3b…` | PHEADER_DEFAULT :113-:118 · PBMSG_HEADER :76-:81 · PDELETE_CHARACTER :622-:626 |

## 2. Pacote
- **HeadCode 0x14, S→C** — `case 0x14: ReceiveDeleteCharacterViewport(ReceiveBuffer)` (:13116-:13119; "delete characters & monsters" — serve chars E monsters).
- **Framing C1** — cast `LPPHEADER_DEFAULT` (:2795) sobre `PHEADER_DEFAULT{PBMSG_HEADER Header{Code;Size;HeadCode}; BYTE Value}` (H :113-:118; PBMSG 3 B :76-:81). **Único do ciclo até aqui em C1** (0x12/0x13 são C2/PWMSG).
- Header 4 B `[C1][size][0x14][count BYTE]`; **tamanho mínimo 6 B** (4+2).

## 3. Layout
| off | Campo | Tipo | Evidência |
|---|---|---|---|
| 0 | Code=0xC1 | BYTE | :116/:76 |
| 1 | size (= total do frame, C1 padrão) | BYTE | :77 — consistência validada pelo parser |
| 2 | HeadCode=0x14 | BYTE | :78 |
| 3 | **count** | BYTE | Value :117; loop `i<Value` :2797 |
| 4+2i | KeyH,KeyL (por entidade) | 2×BYTE | PDELETE_CHARACTER :622-:626 |
- **Key**: `(KeyH<<8)+KeyL` **BE** :2814; **DeleteFlag = b15** (:2815 — **dead read**: lido e nunca usado; remoção ocorre com ou sem ele) ; **Key &= 0x7FFF** :2817 → `DeleteCharacter(Key)` :2839.
- **Stride FIXO 2** (`Offset += sizeof(PDELETE_CHARACTER)` :2846) — sem campos variáveis.
- Limite de count: derivado do size (`(size-4)/2`; count maior ⇒ frame truncado ⇒ erro).

## 4. Semântica
- Remove a entidade (char OU monster) do mundo do cliente: UnRegisterBuff de todos os buffs :2832-:2837 → `DeleteCharacter(Key)` :2839 (+ limpezas de UI: shop/castle switch :2801-:2827/:2840-:2844 — fora do escopo MVP).
- No WorldState MVP: `ApplyFrame_DeleteEntities_C1(frame, ws, err)` — parse em vetor local, apply só se OK; `entities.erase(key)` **ignorando keys inexistentes** (o cliente não falha se a key não existe; DeleteCharacter é tolerante).

## 5. Core desta parte (bloco 1.3-I)
`ParseViewportDeletePlain_C1(frame, std::vector<uint16_t>& outKeys, err)` — valida C1/head 0x14/size/count; extrai keys **BE &0x7FFF** (deleteFlag descartado — dead read); bounds estrito (entidades + resíduos ⇒ erro `0x14:`). `ApplyFrame_DeleteEntities_C1` — remove do `WorldState.entities`. Sem TX (S→C).

## 6. Conclusão
**(1) layout provado** — structs (H :113-:118/:622-:626), loop/stride fixo (:2797-:2847), decodes BE/máscara (:2814-:2817). Ciclo spawn/delete fechado no papel; move/position ficam para depois.
