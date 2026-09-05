# NEW-ERA — PROTOCOL SPEC — MVP SKILL TX (SendRequestMagicAttack, C1 0xDB)
> 1.3-O · 2026-09-05 · SEM REDE. Status: NORMATIVA — classificação **(1) layout TX provado** (macro autoritativa; criptografia C3 e tradução GS parcial — ver §5/§6).

## 1. Fontes (somente leitura)
| # | Fonte | SHA256 | Papel |
|---|---|---|---|
| I | `0E1L__21__wsclientinline.h` | `67a6d535…854` | **macro :600-:616** + inline :618-:633 · **`PACKET_MAGIC_ATTACK 0xDB` :25** |
| S | `0E1L__10__StreamPacketEngine.h` | `797d26d9…684` | `Init(0xC1,head)` :29 · **`Send(BOOL bEncrypt=FALSE,…)` :120** |
| G | `GS_Protocol.cpp` | `e8bb4dbb…f13` | `switch(head)` :96 · **`case 0x19: gSkillManager.CGSkillAttackRecv((PMSG_SKILL_ATTACK_RECV*)lpMsg,aIndex)` :121-:122** |

## 2. Request C→S (layout provado — macro :600-:616, idêntica à inline :618-:633)
`spe.Init(0xC1, 0xDB)` ⇒ **C1, size = 9 + 3·Count** (variável, multi-alvo):
```
[C1][size][DB][TypeH][TypeL][x][y][Serial][Count] + por alvo i<Count: [KeyH][KeyL][SkillSerial]
 0   1    2   3     4     5  6   7       8                       …+3·Count
```
| off | Campo | Evidência |
|---|---|---|
| 3/4 | TypeH, TypeL | `HIBYTE(Type)` `LOBYTE(Type)` :607 ⇒ skill id **WORD BE** |
| 5/6 | x, y | `(BYTE)p_x` `(BYTE)p_y` :607 (posição do cast) |
| 7 | Serial | `MakeSkillSerialNumber(&p_Serial)` :607 — **[NOT RECOVERED]** (def fora dos evidenciados; contador client-side; valor não afeta layout) |
| 8 | Count | nº de alvos :607 (loop :609-:613) |
| 9+3i | KeyH, KeyL | `pKey[i]>>8`, `pKey[i]&0xff` :611 — **BE sem máscara** (int* truncado p/ BYTE) |
| 11+3i | SkillSerial | `(BYTE)p_SkillSerial` :612 — **1 B POR ALVO** (WORD truncada; fora de #ifdef — monk-irrelevante) |
- **Count=1 ⇒ 12 B fixo** (caso golden). Size u8 ⇒ Count ≤ 82.

## 3. Semântica
Skill `Type` cast em (x,y) atingindo `Count` alvos (keys do viewport), serial de sessão e skill-serial por alvo. Guard "webzen" :602/:619 idêntico ao 1.3-N.

## 4. Transporte / criptografia
- **`spe.Send(TRUE)`** :614/:632 — bEncrypt=TRUE (única diferença vs attack 1.3-N que usa default FALSE): o sender PEDE criptografia ⇒ path C3-capable. A criptografia em si acontece na global `SendPacket(char*,WORD,BOOL,BOOL)` **[NOT RECOVERED]** — neste MVP o builder produz o **C1 0xDB PLAIN pré-criptografia** (bytes exatos de `m_byBuffer`, autoritativos pela macro); wrap C3 = state-dependent **[NOT RECOVERED]** (não inventar; evidência de login: ENCRYPT_STATE=0).

## 5. GS RX
- `case 0x19` :121-:122 → `CGSkillAttackRecv` (PMSG_SKILL_ATTACK_RECV **[NOT RECOVERED]** — SkillManager.h fora dos evidenciados).
- **0xDB NÃO ocorre no GS_Protocol.cpp** ⇒ skill chega ao GS como head **0x19**; a tradução 0xDB→0x19 **[NOT RECOVERED]** (camada de conexão — análoga à tradução olc→F3:03 do join documentada na 1.3-C; ou convenção de heads distintos client/GS do fork).

## 6. Core (bloco 1.3-O)
`BuildC1_SkillRequestWire(skillType, x, y, serial, count, targetKeys*, skillSerial, out, err)` — fiel §2 (Type BE, keys BE sem máscara, SkillSerial por alvo); guards: count 1..82, size u8; erro "0xDB TX:".

## 7. Conclusão
**(1) layout TX provado** (macro byte-a-byte, ambas as variantes idênticas). Parciais: Serial def [NOT RECOVERED] (valor de teste documentado no golden); wrap C3 [NOT RECOVERED] (builder = plain pré-encrypt); tradução GS 0xDB→0x19 [NOT RECOVERED].
