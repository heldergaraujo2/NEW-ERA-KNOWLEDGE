# NEW-ERA — PROTOCOL SPEC — MVP DAMAGE RX (PACKET_ATTACK 0x11, C1)
> 1.3-M P1/2 · 2026-09-05 · SEM REDE. Status: NORMATIVA — classificação **(1) layout provado**.

## 1. Fontes (somente leitura)
| # | Fonte | SHA256 | Papel |
|---|---|---|---|
| I | `0E1L__21__wsclientinline.h` | `67a6d5359bb702b38f2a43e7d8377f3c8e37b8af4f2a49b15b4417d4e98a0854` | **`#define PACKET_ATTACK 0x11` :26** |
| C1 | `0E1L__13__WSclient.cpp` | `6538d944126181dfa77d4419bf992ac77151eec5411dc3ab243f2926fec868bf` | handler **ReceiveAttackDamage :2984-:3191** · dispatcher **:13143-:13144** |
| H | `0E1L__14__WSclient.h` | `19d38a3b6506a745662c14dd7cdd6e41ca3ad29b28b17c76f860cd9e67799c10` | **PRECEIVE_ATTACK :674-:685** · PRECEIVE_DAMAGE :908-:916 |
| P6 | `0E1L__06__ProtocolSend.cpp` | `609e7e935fc88a02f662032ff82d0179d67e34910ee48006b2aa890d6efd5b14` | túnel BOTH_MESSAGE :99-:137 |
| P7 | `0E1L__07__ProtocolSend.h` | `3ff13302150537b3ed7db40aef8c3dd6f5e5b81927e39584b0510d598d97d684` | enum :7-:26 ⇒ **BOTH_MESSAGE = ordinal 12 = 0x000C** |

## 2. Identificação
- **Headcode 0x11 `PACKET_ATTACK` (S→C)** · handler **`ReceiveAttackDamage(ReceiveBuffer)`** chamado em `case PACKET_ATTACK:` :13143-:13144 do dispatcher clássico `TranslateProtocol` (switch sobre head C1).
- **Framing C1, 10 B FIXO** — struct do cast **PRECEIVE_ATTACK :674-:685** = `PBMSG_HEADER`(3) + 7 BYTEs ⇒ `[C1][0x0A][0x11][KeyH][KeyL][DamageH][DamageL][DamageType][ShieldDamageH][ShieldDamageL]`.

## 3. Layout (10 B)
| off | Campo | Decode (evidência) |
|---|---|---|
| 0/1/2 | C1 · size=0x0A · head=0x11 | framing C1 (PBMSG_HEADER no cast :2986) |
| 3/4 | KeyH, KeyL | raw=(KeyH<<8)+KeyL **BE** :2995; **Success = raw>>15 (b15)** :2996; **Key &= 0x7FFF** :2997 (máscara PRESENTE — nuance vs 0x15/0x0006 sem máscara) |
| 5/6 | DamageH, DamageL | Damage = WORD **BE** :3002 |
| 7 | DamageType | type=&0x3F · bDouble=>>6&1 · bCombo=>>7&1 :3009-:3012 (branch não-Monk ativa: `PBG_ADD_NEWCHAR_MONK_SKILL` **não definido em nenhum header evidenciado**) |
| 8/9 | ShieldDamageH, ShieldDamageL | WORD **BE** :3013 |
- Alvo: `FindCharacterIndex(Key)` :2999 (chars E monsters, como 0x15). Handler NÃO valida tamanho (cast direto) ⇒ parser exige 10 B exatos (sizeof da struct).

## 4. Semântica observável (apply)
- **Success=1** ⇒ shock + ponto vermelho :3017-:3020; **Key==HeroKey (self)** ⇒ `Life -= Damage` e `Shield -= ShieldDamage` **com clamp em 0** :3031-:3041 (espelhado :3044-:3056); outros alvos ⇒ efeitos visuais apenas.
- **`c->Hit = Damage` :3188** — último dano registrado no registro do alvo (todos os caminhos) ⇒ no MVP: `EntityRecord.lastDamage`.
- HP/Shield **NÃO vêm no viewport** (0x12/0x13 sem campos Life) ⇒ `hp`/`shield` opcionais no WorldState (populados por stats futuro); clamp aplicado somente se populado.
- Debug :3190 imprime label **"0x15"** — mislabel STALE upstream (pacote real 0x11; anotado, sem impacto).

## 5. Transporte no wire moderno (BOTH_MESSAGE = túnel)
- `case BOTH_MESSAGE` (olc **id=0x000C**) :99-:137: body olc = **pacote clássico cru**; autodetect `recv[0]`∈{C1,C3} (size=recv[1], head=recv[2]) ou {C2,C4} (size=MAKEWORD(recv[2],recv[1]) = szH<<8|szL, head=recv[3]) :111-:121 ⇒ **`TranslateProtocol(head, recv, size, 0)` :135** = o MESMO dispatcher clássico.
- ⇒ no build moderno o damage RX anda **embutido**: `[id:u16=0x000C LE][size:u32 LE][C1 0A 11 … 10 B]`. Parser P1 = C1 clássico (payload do túnel); unwrap olc documentado (futuro: ApplyBothMessage wrapper).
- Irmão NÃO escolhido: **self-dano F3 sub 0x07** `ReceiveDamage` :2851-:2865 (PRECEIVE_DAMAGE :908-:916 c/ SubCode; sub-switch F3 :12992) — incl. **bug upstream** :2861-:2863 `Shield = ShieldDamage` (atribui em vez de subtrair) — documentado como observado, fora do MVP.

## 6. Core desta parte (bloco 1.3-M)
`DamageEvent{key,success,damage,shieldDamage,damageTypeRaw,type,doubleEnable,comboEnable}` · `ParseDamageRxPlain_C1(frame, out, err)` (C1/0x0A/0x11/10 B; erro "0x11:") · `ApplyFrame_DamageRx_C1(frame, ws, err, missed=nullptr)` — key existente ⇒ `lastDamage=damage` (+clamp de hp/shield opcionais); inexistente ⇒ ignora+missed; frame inválido não altera estado. `EntityRecord` += `optional<int> hp, shield` + `int lastDamage=-1`.

## 7. Conclusão
**(1) layout provado** — headcode (0x11), framing (C1 10 B fixo), struct (PRECEIVE_ATTACK), endianness/máscara (Key BE + b15 + &0x7FFF), semântica (clamp self + c->Hit) e transporte moderno (túnel BOTH_MESSAGE 0x000C) todos evidenciados por linha. Opaco: nenhum (monk-flags cedo: define ausente ⇒ branch #else ativa).
