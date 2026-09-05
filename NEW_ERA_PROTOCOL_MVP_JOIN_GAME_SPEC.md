# NEW-ERA — PROTOCOL SPEC — MVP JOIN GAME / JOIN MAP SERVER (F3:0x03)
> 1.3-C · 2026-09-05 · offline + 1 fetch raw@580472e (ProtocolAsio.h, /tmp). Status: NORMATIVA PARCIAL — classificação **(2)** (ver §6).

## 1. Fontes
| # | Fonte | Bytes | SHA256 | Papel |
|---|---|---|---|---|
| I | `0E1L__21__wsclientinline.h` | 62.684 | `67a6d535…` | TX clássico :322-:330 |
| C1 | `0E1L__13__WSclient.cpp` | 415.884 | `6538d944…` | RX :871-:1000+; dispatch :12969-:12974 |
| H | `0E1L__14__WSclient.h` | 91.012 | `19d38a3b…` | structs :398-:403/:406-:452 |
| G1 | `GS_Protocol.cpp…e8bb4dbb.cpp` | 135.014 | `e8bb4dbb…` | F3:03 :967-:969; CGCharacterInfoRecv :2135-:2151 |
| M | `0C10B__SocketManagerModern…cpp` | (evidência 0C.10-B) | — | **ponte** :122-:131 |
| P7 | `0E1L__07__ProtocolSend.h` | (evidência L/N) | — | enum :7-:26; SendPacket :145 |
| **F1** | upstream `Source Client/source/ProtocolAsio.h` @580472e (**fetch /tmp**) | 20.884 | `8489f782392d96d718aa9bf65c47d2de5798a770dc850ee7f32c78368052df8c` | framing olc::net :33-:37/:373-:397 |

## 2. Arquitetura descoberta (2 sistemas paralelos)
- **Clássico**: C1/F3, structs PBMSG — RX do join 100% clássico (dispatch F3:03).
- **Novo (NEW_PROTOCOL_SYSTEM)**: `olc::net`/ASIO, heads `enum class ProtocolHead : uint16_t` (P7 :7-:26). **`BOTH_CONNECT_JOIN_GAME = 11`** (posição 12ª do enum, valores 0..12).
- **PONTE no GS** (M :122-:131): `case BOTH_CONNECT_JOIN_GAME:` → monta `PMSG_CHARACTER_INFO_RECV` clássico `header.set(0xF3,0x03,…)` + name (≤10 B do body) → **chama `CGCharacterInfoRecv(&pMsg,aIndex)`** (:129) — o request novo vira o request clássico.

## 3. TX (cliente)
- **Macro clássico** `SendRequestJoinMapServer(p_ID)` (I :322-:330): NÃO usa `spe` — copia `nameBuf[10]` e chama `gProtocolSend.SendPacket(ProtocolHead::BOTH_CONNECT_JOIN_GAME, nameBuf, 10)`.
- `SendPacket` (P7 :145): `SocketConnect->DataSend(head, message, size)` → olc::net.
- **Wire novo**: `[id:u16=0x000B][size:u32=10][body=ID[10] zero-padded]` (header `message_header{T id; uint32_t size}` F1 :33-:37; envio header→body F1 :378-:390). **NÃO é C1/F3** — não há XOR/SimpleModulus neste path.
- Contraparte clássica do request (se o GS for 100% clássico): `PREQUEST_JOIN_MAP_SERVER` 14 B `[C1][0E][F3][03][ID10]` (H :398-:403) — macro `spe` de envio clássico [NOT RECOVERED no inline].

## 4. RX (cliente) — CLÁSSICO F3:03
- Dispatch: `case 0x03: if(!ReceiveJoinMapServer(ReceiveBuffer,bEncrypted)) return FALSE;` (C1 :12969-:12974). Check de `bEncrypted` está **comentado** (:873-:880) — não falha por isso.
- `PRECEIVE_JOIN_MAP_SERVER` (H :406-:452) = **66 B**: `[C1][42][F3][03][PositionX][PositionY][Map][Angle][btMExp1..8][btMNextExp1..8][LevelUpPoint:W][Str/Dex/Vit/Ene/Life/LifeMax/Mana/ManaMax/Shield/ShieldMax/SkillMana/SkillManaMax: 12×W][Gold:DW][PK][CtlCode][AddPoint:sh][MaxAddPoint:sh][Charisma:W][wMinusPoint:W][wMaxMinusPoint:W]`.
- Consumo (C1 :883-:1000): Exp/NextExp = 8 bytes BE encadeados (:886-:938, i64); stats :941-:962; Gold :964; Map/posição/ângulo :967-:980; PK/CtlCode :988-:989.

## 5. Servidor (GS)
- Request clássico: `case 0x03: CGCharacterInfoRecv((PMSG_CHARACTER_INFO_RECV*)lpMsg,aIndex)` (G1 :967-:969).
- `CGCharacterInfoRecv` (G1 :2135-:2151): exige `Connected==OBJECT_LOGGED`; copia `name`; chama `GDCharacterInfoSend(aIndex,name)` (GS→**DataSource/JoinServer**, round-trip fora do escopo).
- **Sender S→C do response 66 B: [NOT RECOVERED]** — não está em G1 (nenhum `set(0xF3,0x03,…)` de resposta); deve viver no callback DG (arquivo GS fora da evidência atual).

## 6. Classificação: **(2) parcialmente fechada**
Faltam exatamente: (a) **sender GS do response** (arquivo X = callbacks DG do GS, fora do escopo — layout já 100% provado pelo lado cliente §4); (b) macro clássico `spe` do request (só a struct existe). O MVP 1.3-C pode prosseguir com: TX = path novo ([u16][u32][ID10]) OU clássico 14 B (struct); RX = parser 66 B §4 — ambos provados.
