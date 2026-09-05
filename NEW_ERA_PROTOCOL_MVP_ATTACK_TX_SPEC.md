# NEW-ERA — PROTOCOL SPEC — MVP ATTACK TX (SendRequestAttack, C1 PACKET_ATTACK)
> 1.3-N P1/2 · 2026-09-05 · SEM REDE. Status: NORMATIVA — classificação **(1) layout TX provado** (payload/bytes autoritativos pela macro do cliente); transporte GS-side parcial (ver §5).

## 1. Fontes (somente leitura)
| # | Fonte | SHA256 | Papel |
|---|---|---|---|
| I | `0E1L__21__wsclientinline.h` | `67a6d535…854` | **macro SendRequestAttack :518-:527** (TX autoritativo) |
| E | `0E1JN4__enum_h` | `7a3eba98…d90` | **AT_ATTACK1=120** :1497 (AT_ATTACK2=121) |
| S | `0E1L__10__StreamPacketEngine.h` | `797d26d9…684` | `spe.Init(0xC1,head)` :29 · `spe.Send(bEnc=FALSE,bForceC4=FALSE)` :120-:125 |
| P7 | `0E1L__07__ProtocolSend.h` | `3ff13302…684` | `SendPacket(ProtocolHead,…)` :145 (olc) · **`SendPacketClassic(buf,size)` :146 (cru)** |
| P6 | `0E1L__06__ProtocolSend.cpp` | `609e7e93…b14` | túnel BOTH_MESSAGE **RX** :99-:137 (sem wrapper TX C→S) |
| G | `GS_Protocol.cpp` | `e8bb4dbb…f13` | `switch(head)` :96 · **`case PROTOCOL_CODE2: gAttack.CGAttackRecv((PMSG_ATTACK_RECV*)lpMsg,aIndex)` :112-:113** |

## 2. Request C→S (layout provado)
- **Sender**: macro **`SendRequestAttack(p_Key, p_Dir)`** :518-:527 — o menor attack TX do cliente (4 B de payload; irmãos: `SendRequestMagicAttack` 0xDB :600-:633 com Type/x/y/Serial/Count/Key/SkillSerial — skill, fora do MVP).
- **`spe.Init(0xC1, PACKET_ATTACK=0x11)`** ⇒ **C1, 7 B FIXO**:
```
[C1][07][11][KeyH][KeyL][0x78][Dir]
 0   1   2   3     4    5     6
```
| off | Campo | Evidência |
|---|---|---|
| 3/4 | KeyH, KeyL | `(p_Key)>>8`, `(p_Key)&0xff` — **BE SEM máscara** :524 (o cliente envia o key pós-viewport cru; b15 não usado no TX) |
| 5 | AT_ATTACK1 = **0x78 (120)** | enum_h :1497 (AT_ATTACK2=121 = combo/2º hit) |
| 6 | Dir | `(p_Dir)` byte :524 (direção do golpe) |
- `spe.Send()` com defaults ⇒ **C1 plain** (bEncrypt=FALSE ⇒ NÃO C3; bForceC4=FALSE ⇒ NÃO C4).

## 3. Semântica
- "Normal attack" do hero contra `Key` do alvo (entity do viewport) com direção `Dir`. Guard curioso da macro :520: `if(!FindText2(Hero->ID,"webzen"))` — bloqueia envio se o ID contém "webzen" (guard de QA/upstream; documentado, sem efeito no layout).

## 4. Head é BIDIRECIONAL (0x11)
- S→C 0x11 = PACKET_ATTACK/ReceiveAttackDamage (1.3-M). C→S 0x11 = este request. Mesmo head, payloads DIFERENTES (RX 7 B payload struct PRECEIVE_ATTACK; TX 4 B payload macro) — padrão MU clássico.

## 5. Transporte (C1 direto vs túnel 0x000C)
- `spe.Send()` :123 chama a global `SendPacket(char*,WORD,BOOL,BOOL)` — definição **[NOT RECOVERED]** nos arquivos evidenciados (path clássico do cliente).
- ProtocolSend.h :146 `SendPacketClassic(buf,size) → SocketConnect->DataSend(message,size)` — a connection olc **transporta bytes clássicos crus** ⇒ C1 direto no wire é consistente e é o wire-real do sender.
- **NÃO há wrapper C→S que encapsule C1 no olc 0x000C** (o túnel BOTH_MESSAGE :99-:137 é RX S→C). Helper `BuildAsio_BOTH_MESSAGE_FromClassicC1` fornecido no core apenas como utilitário de framing (formato do frame provado no RX); **uso C→S não evidenciado — não assumir**.

## 6. GS RX (confirmação parcial)
- `switch(head)` :96 → `case PROTOCOL_CODE2` :112 → **`gAttack.CGAttackRecv((PMSG_ATTACK_RECV*)lpMsg, aIndex)`** :113 (Attack.h incluso :4).
- `PROTOCOL_CODE2` valor numérico **[NOT RECOVERED]** nos evidenciados; correlação forte `PROTOCOL_CODE2 == 0x11` (mesma constelação clássica; irmão `PROTOCOL_CODE3` → CGPositionRecv). `PMSG_ATTACK_RECV` **[NOT RECOVERED]** (Attack.h não evidenciado) — leitura de campos do servidor não extraída; o layout TX autoritativo é o da macro.
- Nota: `case 0x11` :263 é de um SUB-switch (MuRummy, F3) — NÃO é o attack.

## 7. Core desta parte (bloco 1.3-N)
`kAt_ATTACK1=0x78` · `BuildC1_AttackRequestWire(targetKey, dir, out, err)` — bytes exatos §2, SEM máscara (fiel :524), bounds-check (out limpo; erro "0x11 TX:") · `BuildAsio_BOTH_MESSAGE_FromClassicC1(innerC1, out, err)` — exige inner C1 ≥3 B c/ [0]==0xC1; produz `[0C 00][size:u32 LE][inner]` (framing §5).

## 8. Conclusão
**(1) layout TX provado** (macro autoritativa byte-a-byte + enum + defaults C1 plain). Parciais documentados: global SendPacket [NOT RECOVERED]; PROTOCOL_CODE2/PMSG_ATTACK_RECV GS [NOT RECOVERED] (correlação forte); túnel C→S não evidenciado.
