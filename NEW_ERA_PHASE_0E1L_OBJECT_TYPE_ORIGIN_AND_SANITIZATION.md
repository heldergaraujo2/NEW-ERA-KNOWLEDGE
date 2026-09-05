# NEW-ERA — PHASE 0E.1-L: Origem e sanitização de OBJECT::Type (vetor para Models[] OOB)
> Microteste 0E.1-L · 2026-09-04 · READ-ONLY · offline + rede ultra-restrita (1 Contents API + 23 raw@580472e ≤25) · fecha a pendência de origem do 0E.1-K.

## 1. Fontes analisadas
- **Offline**: 35 evidências JN2/JN3/JN4 (repo local: 0 hits — §29).
- **Rede**: Contents API `…/contents/Source%20Client/source?ref=580472e` → **HTTP 200, 613 itens**. Mandatórios: ZzzObject.h/ZzzCharacter.cpp/.h **já em evidência** (sem re-download, dedupe); **6 inexistentes no diretório** (Protocol.h/.cpp, wsProtocol.cpp, Connection.cpp, SocketManager.cpp, StreamPacketEngine.cpp — registrados). Candidatos regex `(protocol|socket|connect|recv|receive|packet|network|stream|object|character|ws)`: 26, **23 baixados (23/23 HTTP 200, 1.288.497 B), truncagem 0** → `NEW_ERA_EVIDENCE/0E1L__01..23__*` + 23 `.SHA256.txt`.
- Chaves: **WSclient.cpp** 415.884 B `6538d944126181dfa77d4419bf992ac77151eec5411dc3ab243f2926fec868bf` · **ZzzObject.cpp** 378.840 B `2974f5bb48a983400cd9f1eee638ee4b32bc5f16da7a08a1203c26b2504ad8ed` · **wsclientinline.h** 62.684 B `67a6d5359bb702b38f2a43e7d8377f3c8e37b8af4f2a49b15b4417d4e98a0854`.

## 2. Definição de OBJECT e tipo de `Type`
- `struct OBJECT` **NÃO está definida em nenhuma das 58 evidências** (ZzzObject.h = só protótipos; ZzzObject.cpp = usos :54-57 `OBJECT_BLOCK ObjectBlock[256]` etc.). Definição provável: `_struct.h`/`_types.h` (existem no listing; **não casam a regex de seleção do comando** — fora do escopo autorizado) → tipo C exato = `[PENDING — struct def fora do escopo]`. Evidência indireta: handlers compõem `WORD Type` (16-bit unsigned) e `CreateCharacterPointer(c, int Type, …)` recebe `int` → runtime values 0..65535.

## 3. Cadeia REDE→criação (toda em arquivo)
| Handler (opcode) | Extração de Type | Chamada |
|---|---|---|
| `ReceiveCreateMonsterViewport` **0x13** (WSclient.cpp:2585; dispatch :13106) | `WORD Type = ((WORD)(Data2->TypeH&0x03)<<8)+Data2->TypeL` :2597 — **máscara 0x03 → Type ≤ 1023 (bit-field estrutural, não validação)** | `CreateMonster(Type,…)` :2606 |
| `ReceiveCreateSummonViewport` **0x1F** (:2714; dispatch :13110) | `WORD Type = ((WORD)(Data2->TypeH)<<8)+Data2->TypeL` :2723 — **16-bit SEM máscara** | `CreateMonster(Type,…)` :2729 (152-158 → CreateHellGate) |
| `ReceiveCreateTransformViewport` **0x45** (:2382; dispatch :13114) | idem 16-bit :2425 — **SEM máscara** | `CreateMonster(Type,…)` :2427 |
| `ReceivePreviewPort` case 2/3 (:11183+:11226) | idem 16-bit :11226 | `CreateMonster(Type,…)` :11228 |

**`CreateMonster(int Type,…)`** (ZzzCharacter.cpp:12496-14355): 1º tenta subsistemas (CursedTemple/Hellas/BattleCastle/… — todos `Setting_Monster(c,Type,…)`), depois **um único `switch(Type)` gigante com casos LITERAIS** (`case 78: OpenMonsterModel(19); c=CreateCharacter(Key,MODEL_MONSTER01+19,…)` :12826+; `case 577: OpenNpc(MODEL_KARUTAN_NPC_REINA)…` :14336) e **UM `default:` em :13386**: `OpenMonsterModel(0); c = CreateCharacter(Key,MODEL_MONSTER01,…)` — **Type desconhecido → modelo 0 (SUBSTITUIÇÃO, não crash)**. Fecho: `Setting_Monster(c,Type,…)` :14352.
- **`OpenMonsterModel(int Type)`** (ZzzOpenData.cpp:2356): cabeçalho `int Index = MODEL_MONSTER01+Type` :2360 · `BMD *b = &Models[Index]` :2362 · `AccessModel(Index,…)` :2366 — **indexador SEM guard**; porém **TODOS os callers em 58 evidências são LITERAIS** (`OpenMonsterModel(53/70/19/0/71+randType…)`; plural `OpenMonsterModels()` :3957 boot-time) — grep variável = **0 hits**. A seção `Models[MODEL_MONSTER01+Type]` :3175-3340 está DENTRO de `switch(Type)` com cases literais → efetivamente constante por case.
- **`Setting_Monster`** (:12396): `c->MonsterIndex = Type` :12422 (**16-bit cru armazenado**) · classificação Kind por faixas :12426-12450 · loop `MonsterScript[i].Type` comparações :12414-12420 — **`MonsterIndex` NUNCA é usado como índice** (só `==`, ex. :8530) ✓.
- **`o->Type` só recebe ENUMS**: `CreateCharacterPointer` :11182 → `o->Type = Type` :11219, e todos os callers de `CreateCharacter` passam `MODEL_*` (censo K + :12617-14350) → **(I) interno**.

## 4. Atribuições a Type — tabela resumida (14 grupos)
| # | Atribuição | Origem | Sanitização |
|---|---|---|---|
| 1 | `o->Type = Type` ZzzCharacter:11219 (CreateCharacterPointer) | (I) enum MODEL_* | nenhuma necessária (constantes) |
| 2 | `Object->Type = Type` :6445 (RenderLinkObject) | (I)/(F) param de render (callers: enums/:11741+) | nenhuma |
| 3 | `w->Type = MODEL_BOSS_HEAD/PRINCESS/…` :8502-8534 | (I) | — |
| 4 | `p->Type = MODEL_ANGEL` :9122/:9134 | (I) | — |
| 5 | **`c->Weapon[0].Type = MODEL_SWORD + ExtType`** :12034 (ChangeCharacterExt :12005; chamado por handlers 0x13-adjacentes/:571/:1941/:2320/:2489/:11217 c/ `Equipment` do pacote) | **(R) `Type=Equipment[0]` :12016; `ExtType=(Equipment[11]&240)<<4\|Type` :12019-12020 — 12-bit** | **ESTRUTURAL: sentinela `ExtType==0x0FFF → -1` :12022; máx MODEL_SWORD+4094=5180 < 10002 → in-bounds por bit-width** (sem check explícito) |
| 6 | `c->Weapon/Wing/Helper.Type = p[EQUIPMENT_*].Type+MODEL_ITEM` :11741/:11750/:11759/:11768/:11807 | (R)-indireto (p=itens; parse exato fora do trecho inspecionado) | não observada no trecho |
| 7-10 | inits `-1` :11289-11446 (Weapon/Wing/Helper/Flag/BodyPart) | (I) sentinela | — |
| 11 | `c->BodyPart[HEAD].Type = MODEL_BODY_HELM + GetSkinModelIndex(c->Class)` :11647/:11803 | (I)+(F? Class da rede via char-list — índice de tabela interna) | tabela interna (indireta) |
| 12 | `c->MonsterIndex = Type` :12422 | **(R) cru 16-bit** | **nenhuma no armazenamento** (uso só comparativo ✓) |
| 13 | handlers 0x13/0x1F/0x45/Preview :2597/:2723/:2425/:11226 | (R) seletor | 0x13 máscara 0x03 (≤1023); 0x1F/0x45/Preview **16-bit sem máscara** → neutralizados pelo default-substitution :13386 |
| 14 | `OpenNpc(Type)` ZzzOpenData:1854 (`Models[Type]` :2163+/:2274+) | (I) callers com enums (:14337…) | constante por chamada |
**Contagem**: 14 grupos / ~45 sites; **sanitização EXPLÍCITA contra MAX_MODELS: 0**; sanitização ESTRUTURAL (default-substitution/bit-mask/sentinela): todos os caminhos R.

## 5. Correlação com 0E.1-K (PASSO D)
- **Wrapper AccessModel sem guard (×793)**: recebe `Index` de callers **todos enum/literais/estruturais** no escopo → superfície LATENTE, vetor ativo não confirmado.
- **173 sites `o->Type`/`Owner->Type` sem guard**: origem = enums internos (:11219) → sem OOB observável.
- **91 sites param `Type`/`MODEL_MONSTER01+Type`**: `OpenMonsterModel` é o único com base+param cru no CABEÇALHO (:2360-2366) — **mas 0 callers variáveis**; `ZzzOpenData:3175+` são cases literais.
- **Reavaliação do top-1 do K**: risco praticamente imediato MENOR que o hipotetizado — a rede NÃO injeta índice direto em `Models[]`; injeta um SELETOR traduzido por switch com fallback 0. Risco real restante: (a) fragilidade (0 guards — qualquer path novo/futuro que passe Type cru vira OOB ≥10002 silencioso/heap-corruption); (b) Weapon.Type via equipamento in-bounds só por bit-width 12-bit (se layout mudar, quebra).

## 6. Conclusão canônica: **(3) Type NÃO vem de rede diretamente para Models[]** (com nuance (2))
- `OBJECT::Type` = sempre enums internos (I) via :11219; o seletor de rede 16-bit **não validado** é neutralizado por **default-substitution** (`default: OpenMonsterModel(0)` :13386) — sanitização estrutural, não intencional explícita.
- Única injeção R em campo indexável: `Weapon[].Type = MODEL_SWORD+ExtType` (:12034, 12-bit+sentinela) — **in-bounds por construção** (≤5180 < 10002); Wing/Helper análogos na mesma função (não inspecionados linha-a-linha — declarado).
- `MonsterIndex` guarda 16-bit cru mas só é comparado.

## 7. Limitações
- `struct OBJECT` def fora do escopo regex (_struct.h/_types.h existem no listing) → tipo C exato PENDING.
- Equipamentos Wing/Helper/Body: parse irmão de :12034 não dito linha-a-linha; `p[EQUIPMENT_*]` populate não traçado (arquivo de items fora do conjunto).
- ReceivePreviewPort sem opcode confirmado no dispatcher (função presente :11183; 3 opcodes já mapeados bastam).

## 8. Próximo sugerido (1)
**0E.1-M — mapa de recebimento de itens/inventário** (`ReceiveInventory`/item parse → `p[EQUIPMENT_*].Type`): fechar a última origem R indireta (largura do campo tipo de item → teto efetivo de `Weapon/Wing/Helper.Type`), fechando por completo a superfície R→Models[].

## 9. Artefatos
Este relatório · 23 evidências + sidecars `NEW_ERA_EVIDENCE/0E1L__*` · Ledger §30 · CURRENT_STATE/MASTER_CHECKPOINT.
