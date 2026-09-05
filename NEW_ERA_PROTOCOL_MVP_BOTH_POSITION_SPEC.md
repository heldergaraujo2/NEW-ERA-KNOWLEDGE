# NEW-ERA — PROTOCOL SPEC — MVP BOTH_POSITION (olc id=0x0006 ↔ ReceiveMovePosition)
> 1.3-L P1/2 · 2026-09-05 · SEM REDE. Status: NORMATIVA — classificação **(1) funcional** (campos funcionais provados; 3 bytes de header embutido OPACOS — ver §3).

## 1. Fontes (somente leitura)
| # | Fonte | SHA256 | Papel |
|---|---|---|---|
| P7 | `0E1L__07__ProtocolSend.h` | — | enum :7-:26 ⇒ **BOTH_POSITION = ordinal 6 = 0x0006** · TX structs :44-:55 |
| P6 | `0E1L__06__ProtocolSend.cpp` | `609e7e935fc88a02…` | ponte :92-:94 · TX SendPositionNew :212-:221 |
| C1 | `0E1L__13__WSclient.cpp` | `6538d944…` | handler :1746-:1767 (= 1.3-J) |
| H | `0E1L__14__WSclient.h` | `19d38a3b…` | PRECEIVE_MOVE_POSITION :892-:898 (**header INCONDICIONAL**) |
| I | `0E1L__21__wsclientinline.h` | `67a6d535…` | PACKET_POSITION=0x15 :24 |

## 2. Wire
- **olc**: `[id:u16=0x0006 LE][size:u32 LE][body 7 B]` — total **13 B**, 1 entidade.
- Ponte (P6 :92-:94): `case ProtocolHead::BOTH_POSITION: ReceiveMovePosition((BYTE*)msg.body.data())`.
- Divergência de migração (§75): ao contrário do MOVE (struct headerless), o **POSITION NÃO foi migrado** — `PRECEIVE_MOVE_POSITION` mantém `PBMSG_HEADER` incondicional ⇒ **o body olc DEVE conter os 3 bytes de header embutidos** no início (necessidade estrutural: o handler lê `KeyH` no offset 3 do cast; sem os 3 bytes, misparse).

## 3. Layout do body (7 B)
| off | Campo | Decode (evidência) |
|---|---|---|
| 0..2 | **PBMSG_HEADER embutido (OPACO)** | Code/Size/HeadCode — **NÃO lidos pelo handler** (:1746-:1767 só usa KeyH..Y); valores **[NOT RECOVERED]** (presença provada por necessidade; provavelmente [C1][07][15] do clássico) |
| 3/4 | KeyH,KeyL | Key=(KeyH<<8)+KeyL **BE SEM máscara** :1749 |
| 5 | PositionX | :1762 |
| 6 | PositionY | :1763 |
- Sem dir/angle no pacote (idem 0x15); sem lista; body exatamente 7 (= sizeof da struct).

## 4. Comparação com 0x15 clássico (1.3-J)
| Aspecto | 0x15 clássico | BOTH_POSITION olc |
|---|---|---|
| Envelope | C1 `[C1][07][15]` (7 B total) | `[id=0x0006:u16][size:u32]` + 3 B header embutido (13 B total) |
| Campos funcionais | Key BE sem máscara; X; Y | **idênticos** (mesma struct/handler) |
| Semântica apply | x/y=Data **e** target=Data (:1762-:1765); dir/angle inalterados | **idêntica** |
- TX lateral (documentado): `SendPositionNew` :212-:221 envia body **2 B** `PMSG_POSITION_SEND{BYTE x; BYTE y}` (P7 :44-:48 — self, sem key; headerless). TX≠RX (normal: self × any-entity).

## 5. Core desta parte (bloco 1.3-L)
`kProto_BOTH_POSITION=0x0006` · `ParseBOTH_POSITION_Asio(frame, MoveUpdate&, err)` (valida id/size/body==7; extrai key BE/x/y; bytes 0..2 opacos) · `ApplyFrame_BOTH_POSITION_Asio(frame, ws, err, missed=nullptr)` — reusa `MoveUpdate` (1.3-J); key existente ⇒ x/y/target=Data (dir/angle inalterados); inexistente ⇒ ignora+missed; frame inválido não altera estado.

## 6. Conclusão
**(1) funcional** — id (0x0006), framing olc, body 7 B com header embutido, key/x/y e semântica provados (mesma struct/handler do 0x15 já provado + ponte + enum). Único opaco: valores dos 3 bytes de header embutido [NOT RECOVERED] (irrelevantes ao handler — não validados pelo parser).
