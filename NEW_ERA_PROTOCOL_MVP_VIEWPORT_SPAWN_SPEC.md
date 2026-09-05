# NEW-ERA — PROTOCOL SPEC — MVP VIEWPORT SPAWN (0x13 ReceiveCreateMonsterViewport)
> 1.3-E P1/2 · 2026-09-05 · SEM REDE (0 fetches — tudo já em evidência). Status: NORMATIVA — classificação **(1) layout provado** (struct + handler completos).

## 1. Fontes (somente leitura)
| # | Fonte | SHA256 | Papel |
|---|---|---|---|
| C1 | `0E1L__13__WSclient.cpp` | `6538d944…` | handler :2585-:2712; dispatch :13104-:13107 |
| H | `0E1L__14__WSclient.h` | `19d38a3b…` | PWMSG_HEADER :83-:89 · PWHEADER_DEFAULT_WORD :200-:203 · PCREATE_MONSTER :588-:600 |
| D | `0E1JN3__4___define.h` (18.981 B) | `c2fa25d74def49ff…` | `#define MAX_BUFF_SLOT_INDEX 16` :613 |

## 2. Pacote escolhido
- **HeadCode 0x13** — `case 0x13: ReceiveCreateMonsterViewport(ReceiveBuffer)` (:13104-:13107; HeadCode extraído pelo caller de `TranslateProtocol(HeadCode,…)` :12821 — camada upstream fora do trecho).
- Direção **S→C** (spawn de monstros no viewport).
- **Framing C2** — PROVADO pelo cast incondicional `LPPWHEADER_DEFAULT_WORD` (:2587) sobre `PWMSG_HEADER{Code;SizeH;SizeL;HeadCode}` 4 B (:83-:89; contraste: `PBMSG_HEADER{Code;Size;HeadCode}` 3 B = C1 :76-:81). Um buffer C1 seria misparseado (Value leria byte de entidade) ⇒ handler assume layout C2.
- **Tamanho mínimo: 15 B** = header 5 B (`[C2][SizeH][SizeL][0x13][count]`) + 1 entidade (10 B, 0 buffs).

## 3. Layout (provado)
### Header (5 B)
| Offset | Campo | Tipo | Evidência |
|---|---|---|---|
| 0 | Code=0xC2 | BYTE | PWMSG_HEADER :85 |
| 1..2 | SizeH,SizeL — size=(SizeH<<8)\|SizeL | 2×BYTE BE | :86-:87 (semântica "total do frame" **presumida por analogia C1**; parser valida consistência) |
| 3 | HeadCode=0x13 | BYTE | :88 |
| 4 | **count** (nº de entidades) | BYTE | PWHEADER_DEFAULT_WORD `Value` :202 — BYTE, apesar do nome "_WORD"; loop `i<Data->Value` :2589 |

### Entidade (10 B fixos + buffs variáveis; início em off=5, stride :2710)
| off+ | Campo | Decod (evidência) |
|---|---|---|
| 0/1 | KeyH,KeyL | Key=(KeyH<<8)+KeyL **BE** :2592; CreateFlag=Key>>15 :2602; TeleportFlag=(KeyH&0x40)>>6 :2603; **Key&=0x7FFF** :2605 |
| 2/3 | TypeH,TypeL | Type=((TypeH&0x03)<<8)+TypeL (10 bits) :2597; bMyMob=TypeH&0x80 :2595; byBuildTime=(TypeH&0x70)>>4 :2596 |
| 4/5 | PositionX, PositionY | :2593-:2596/:2606/:2658-:2659 |
| 6/7 | TargetX, TargetY | :2701 PathFinding2 |
| 8 | Path | **dir=Path>>4** :2637; ângulo=((dir−1)×45°) :2637 (low nibble não lida aqui) |
| 9 | s_BuffCount | nº de bytes de buff que SEGUEM :2614-:2619 |
| 10.. | s_BuffEffectState[s_BuffCount] | RegisterBuff :2616 (array declarado [16] :599; no wire só count bytes) |
- **Stride** (:2710): `Offset += sizeof(PCREATE_MONSTER) − (MAX_BUFF_SLOT_INDEX − s_BuffCount)` = **10 + s_BuffCount**.

## 4. Semântica mínima
- `CreateMonster(Type, PositionX, PositionY, Key)` :2606 — essenciais p/ desenhar/posicionar: **Type, X, Y, Key, dir(Path>>4)**.
- CreateFlag ⇒ `AppearMonster(c)` :2679-:2681; buffs ⇒ RegisterBuff :2616; TargetX/Y ⇒ pathfinding :2701.
- Loop aborta se `CreateMonster` devolve NULL :2610.

## 5. Core desta parte
`ParseC2_ViewportMonsterSpawnPlain(pkt, std::vector<SpawnEntity>&, err)` (bloco 1.3-D→1.3-E em `mvp_login_client.cpp`): valida C2/head 0x13/size consistente; extrai {type,key,x,y,targetX/Y,dir,angleDeg,createFlag,teleportFlag,myMob,buildTime,buffs} por entidade; **bounds-check estrito** (entidade+buffs dentro do buffer; bytes residuos ⇒ erro). Nome diverge do "ParseC1_…" do comando porque a evidência provou **C2** (anti-invenção; o próprio passo A.3 pedia registrar C1×C2).

## 6. Conclusão
**(1) layout provado** — struct na evidência (H :588-:600) + handler completo (C1 :2585-:2712) + stride explícito (:2710) + MAX_BUFF_SLOT_INDEX (D :613). Presunção única (não-bloqueante): size C2 = total do frame (validado como consistência no parser; golden da P2/2 vai fixá-lo). Viewport de players (0x12, `ReceiveCreatePlayerViewport` :2167-:2379) fica como próximo candidato natural.
