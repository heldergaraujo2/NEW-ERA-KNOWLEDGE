# NEW-ERA — PROTOCOL SPEC — MVP VIEWPORT CHARACTER SPAWN (0x12 ReceiveCreatePlayerViewport)
> 1.3-G P1/2 · 2026-09-05 · SEM REDE (0 fetches). Status: NORMATIVA — classificação **(1) layout provado** (struct + loop + stride completos).

## 1. Fontes (somente leitura)
| # | Fonte | SHA256 | Papel |
|---|---|---|---|
| C1 | `0E1L__13__WSclient.cpp` | `6538d944…` | handler :2167-:2380; dispatch :13100-:13103 |
| H | `0E1L__14__WSclient.h` | `19d38a3b…` | PCREATE_CHARACTER :537-:550 · EQUIPMENT_LENGTH=17 **:71** · PWHEADER_DEFAULT_WORD :200-:203 · PWMSG_HEADER :83-:89 |
| D | `0E1JN3__4___define.h` | `c2fa25d74def49ff…` | MAX_ID_SIZE=10 :305 · MAX_BUFF_SLOT_INDEX=16 :613 |

## 2. Pacote
- **HeadCode 0x12, S→C** — `case 0x12: AddDebugText(ReceiveBuffer,Size); ReceiveCreatePlayerViewport(ReceiveBuffer,Size)` (:13100-:13103).
- **Framing C2** — cast `LPPWHEADER_DEFAULT_WORD` (:2169) sobre `PWMSG_HEADER{Code;SizeH;SizeL;HeadCode}` (H :83-:89; idem 0x13/§69). Header 5 B `[C2][SizeH][SizeL][0x12][count BYTE (:202)]`.
- **O parâmetro `Size` NÃO é usado no parse** — só em `AddDebugText` (:2244); offsets vêm 100% da struct. (size=(SizeH<<8)|SizeL = total do frame — mesma presunção validada do 0x13; golden da P2/2 fixa.)
- **Tamanho mínimo: 41 B** = 5 (header) + 36 (entidade, 0 buffs).

## 3. Layout por entidade — `PCREATE_CHARACTER` (H :537-:550), base `o`
| off | Campo | Decode (evidência) |
|---|---|---|
| 0/1 | KeyH,KeyL | Key=(KeyH<<8)+KeyL **BE** :2175; CreateFlag=Key>>15 :2176; **Key&=0x7FFF** :2177 |
| 2/3 | PositionX,Y | :2231/:2266-:2267 |
| 4 | Class | tipo-server → client :2235 (ChangeServerClassTypeToClientClassType); **&0x07 = pose/action**: 1=teleport :2242-:2243, 2=sit :2246, 3=pose :2252, 4=healing :2258 |
| 5..21 | Equipment[17] (:71) | raw → `ChangeCharacterExt(idx, Equipment)` :2320/:2322 (decode de item fora do escopo) |
| 22..31 | ID[10] (:305) | memcpy+NULL :2180-:2181/:2353-:2354; filtro "webzen" :2182 |
| 32/33 | TargetX,Y | :2268-:2269/:2282 (PathFinding2) |
| 34 | Path | **dual**: dir=Path>>4 c/ ângulo ((dir−1)×45°) :2271; **PK=Path&0xF** :2237 |
| 35 | s_BuffCount | :2361 |
| 36.. | s_BuffEffectState[n] | RegisterBuff + battleCastle :2361-:2368 |
- **Stride** (:2376): `sizeof(PCREATE_CHARACTER)−(16−s_BuffCount)` = **36 + s_BuffCount** (struct sizeof=52).

## 4. Semântica mínima
- Essenciais p/ desenhar/posicionar player/NPC: **Key, ID, Class (+pose &0x07), X/Y, TargetX/Y, dir (Path>>4)**; PK (Path&0xF) p/ nome colorido; Equipment p/ visual; CreateFlag ⇒ efeito de spawn :2275-:2281.
- Loop NÃO aborta por entidade (sem checagem de NULL como no 0x13 — CreateCharacter pode falhar e o código continua).

## 5. Core desta parte
`SpawnCharacter` + `ParseViewportCharacterSpawnPlain_C2(frame, std::vector<SpawnCharacter>&, err)` (bloco 1.3-G): valida C2/head 0x12/size consistente/count; extrai {key, id[11], classByte, poseAction, x/y, targetX/Y, dir, pk, angleDeg, createFlag, equipment[17] raw, buffs}; bounds estrito (36 B fixos + n≤16 + buffs + resíduos ⇒ erro `0x12:`). Equip decodificado como **opaco** (ChangeCharacterExt é render-layer).

## 6. Conclusão
**(1) layout provado** — struct completa (H :537-:550), loop/stride completos (:2172-:2377), defines provados (:71/:305/:613). Campo não decodificado (opaco por escopo): Equipment[17] (conteúdo de item). Presunção única (não-bloqueante): size C2 = total do frame.
