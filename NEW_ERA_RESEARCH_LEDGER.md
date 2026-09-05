# NEW-ERA RESEARCH LEDGER

================================================================================
> LIVRO-MESTRE DE PESQUISA (DIÁRIO/LIVRO-CAIXA) — PROJETO NEW-ERA
> Reconstrução histórica da trajetória de pesquisa da FASE 0.
> Este documento PRESERVA; não substitui fontes; não autoriza implementação.
================================================================================

## 0. IDENTIDADE DO DOCUMENTO

| Campo | Valor |
|---|---|
| Nome do projeto | **NEW-ERA** (pesquisa sobre a base legada `wongddd/muonline`) |
| Objetivo do documento | Preservar a TRAJETÓRIA completa da pesquisa (microtestes, evidências, conclusões, correções, hipóteses, pendências) antes de continuar o roadmap |
| Estado atual da pesquisa | FASE 0 — PESQUISA/AUDITORIA (READ-ONLY). Implementação NEW-ERA **NÃO INICIADA** |
| Data de criação do Ledger | **2026-09-02** (fuso America/Sao_Paulo) |
| Repositório de pesquisa | `https://github.com/wongddd/muonline` |
| Commit canônico | **`580472e0d5723f9709cbad594f233deb07f9f351`** (branch `main`, HEAD "v8") |
| Outros commits utilizados | **Nenhum outro commit foi usado como fonte de evidência.** Todos os fetches RAW foram pinados neste commit. Registro 0A menciona `b9d07d2` (origin/master, 8 commits atrás) apenas como **informação de histórico git**, sem evidência extraída dele |
| Regra READ-ONLY | ABSOLUTA em relação ao legado: nada de modificar/corrigir/otimizar/refatorar/renomear/deletar; nada de "New-Era" dentro do repo; missão = PRESERVAR + MAPEAR + ENTENDER + DOCUMENTAR |
| Documento vivo | **SIM** — deve ser ATUALIZADO nas pesquisas futuras (adicionar, nunca reescrever por cima). Conclusões SUPERSEDED permanecem registradas |

Arquivos previamente existentes que este Ledger NÃO substitui e NÃO altera:
- `/home/user/NEW_ERA_PHASE_0A_REPOSITORY_AUDIT.md` (591 linhas / 49.701 B)
- `/home/user/NEW_ERA_PHASE_0B_GAMELOOP_FPS_AUDIT.md` (318 linhas / 27.919 B)
- `NEW_ERA_MASTER_CHECKPOINT.md` / `NEW_ERA_CURRENT_STATE.md` — **[NOT RECOVERED — não presentes no workspace deste Agent]** (provavelmente vivem no lado do coordenador).

Legenda de proveniência usada em todo o documento:
- **[FILE]** = relatório persistido no workspace (0A/0B)
- **[MEM]** = memória de sessão (resultados entregues em chat antes da compactação do histórico)
- **[CHAT]** = histórico direto da sessão corrente (comandos e outputs verbatim)
- **[RAW]** = evidência obtida por fetch raw.githubusercontent.com pinado no commit canônico

--------------------------------------------------------------------------------

## 1. NORTH STAR

> "Construir um cliente moderno capaz de preservar a essência do MU, mas remover
> as limitações técnicas históricas do cliente original."

Intenção arquitetural conhecida do NEW-ERA [MEM]:
- arquitetura própria;
- moderna;
- modular;
- data-driven;
- diagnosticável;
- clocks separados;
- preparada para evolução multiplataforma;
- legado utilizado como referência de comportamento, protocolo, formatos, limitações e requisitos;
- **NEW-ERA NÃO é simplesmente Louis + modificações.**

--------------------------------------------------------------------------------

## 2. REGRAS PERMANENTES DA PESQUISA

### 2.1 Regras operacionais [MEM]
- READ-ONLY no legado (0A/0B/0C, standing).
- Não modificar código; não corrigir problemas encontrados; não refatorar; não renomear; não deletar; nada "auxiliar" é descartável — preservar TODOS os arquivos.
- Não implementar NEW-ERA durante a FASE 0; direção New-Era = registro, nunca código; NÃO propor soluções durante microtestes.
- Uma investigação por vez: microteste único → evidência → auditoria → aprovação → próximo (phase-gating explícito: cada mensagem define UM escopo e proíbe o resto; tópicos deferidos não podem ser analisados adiante).
- Evidência antes de conclusão; separar EVIDÊNCIA / CONCLUSÃO / HIPÓTESE; não avançar por suposição.
- Preservar evidências antigas; registrar conclusões SUPERSEDED; não apagar histórico; não inventar evidências.
- Relatórios de fase em `/home/user/` FORA do repo; nunca git-add/commit reports. (Série 0C: entregue EM CHAT por decisão aceita pelo usuário — o Ledger é a exceção autorizada.)
- Working tree limpa; sem artefatos de build dentro do repo.
- Nem toda constante numérica é limitação: classificar com cadeias causais.

### 2.2 Ordens permanentes sobre o workspace [MEM]
- Workspace over-budget: NÃO novo clone, NÃO re-baixar/re-copiar repositório, NÃO zip, NÃO deletar para liberar espaço, NÃO resolver o limite durante análises. `PYTHONDONTWRITEBYTECODE=1`.
- **NÃO CRIE OUTRA LUMEN**; nunca criar cópia do projeto; não criar fork/clone/backup/ZIP adicionais.
- Nunca materializar `Client/` (907 MB); consulta sob demanda (GitHub/raw). Scratch re-fetches em `/tmp/` (fora do workspace) — padrão aceito.

### 2.3 Classificações
`CONFIRMED` · `PROBABLE` · `UNCERTAIN` · `PENDING` · `DISCARDED` · `SUPERSEDED`

--------------------------------------------------------------------------------

## 3. LIMITAÇÕES DO AMBIENTE (descobertas durante a pesquisa — não são conclusões sobre o código)

| # | Limitação | Evidência/origem |
|---|---|---|
| A1 | `/tmp` é EFÊMERO e foi limpo **≥3 vezes ENTRE e DURANTE a sessão** (uma delas no meio da auditoria 0C.6-A da fase 0C.5-B; `/tmp/mu/` sumiu repetidamente) | [CHAT]+[MEM] |
| A2 | Workspace DEGRADADO: `muonline/` sem `.git`; `Source Client/source` = 345 arquivos (apenas subdirs; .cpp das raízes sumiram); `Source Server` = ConnectServer/JoinServer/DataServer FULL, GameServer = só `Time/` + lua; `MuServer/` configs intactas. GitHub = source of truth | [MEM]+[CHAT] |
| A3 | `git log master` falha (só `main`); `git` impossível no estado atual (sem .git) | [MEM] |
| A4 | `rg` NÃO instalado — traduzir scripts `rg` → `grep -rnP` / `grep -rlw` | [MEM]+[CHAT] |
| A5 | GitHub raw exige `%20` para "Source Client"/"Source Server"; fetches podem falhar transitoriamente (sempre `mkdir -p` e tentar 2ª vez; `curl -o` com dir faltante exit 2 imita falha de rede) | [MEM]+[CHAT] |
| A6 | Linhas de código SEMPRE associadas ao commit `580472e` (todos os fetches pinados) | [CHAT] |
| A7 | Quirks conhecidos do repo: `MuOnline.sql` UTF-16 (iconv); dir exceção `ExternalObject/Leaf`; `Shader.cpp` stub vazio; `protocol_test.c` ausente; `MapServerInfo.cpp/.h` NÃO está na raiz de GameServer (404 — caminho real desconhecido); `SendPacket` é `__forceinline` em wsclientinline.h:67; `CChatRoomSocketList` indefinido no cliente | [MEM] |
| A8 | Alguns comandos/textos completos das fases 0C.1–0C.5-A (pré-compactação) não estão disponíveis verbatim — ver §18 | [MEM] |

--------------------------------------------------------------------------------

## 4. ÍNDICE CRONOLÓGICO DE MICROTESTES

| ID | Objetivo | Arquivos | Commit | Resultado | Classificação | Status | Observação |
|---|---|---|---|---|---|---|---|
| 0A | Inventário/arquitetura do repo | todo o repo | 580472e | Entregue em `/home/user/NEW_ERA_PHASE_0A_REPOSITORY_AUDIT.md` | CONFIRMED | APROVADO | Clone íntegro na época (17 commits) |
| 0B | Game loop/FPS/timing (cliente) | ZzzScene.cpp, ZzzAI.cpp, Winmain.cpp, Time/Timer.cpp | 580472e | Entregue em `/home/user/NEW_ERA_PHASE_0B_GAMELOOP_FPS_AUDIT.md` | CONFIRMED | APROVADO | tick 40ms/25Hz; limiter 25FPS |
| 0C.1 | Connection map (portas/topologia) | configs MuServer/ + fontes | 580472e | Entregue em chat [MEM] | CONFIRMED | APROVADO | ver §8.1 |
| 0C.2 | Receive do cliente | WSctlc.cpp, WSclient.cpp, ProtocolAsio.h, ProtocolSend.cpp | 580472e | Entregue em chat [MEM] | CONFIRMED | APROVADO | ver §8.2 |
| 0C.3 | Send do cliente | wsclientinline.h, WSclient.cpp, ProtocolAsio/ProtocolSend | 580472e | Entregue em chat [MEM] | CONFIRMED | APROVADO | ver §8.3 |
| 0C.4-A..D | Framing C1/C2/C3/C4 | WSclient.cpp, wsclientinline.h | 580472e | Entregue em chat [MEM] | CONFIRMED | APROVADO | ver §8.4 |
| 0C.4-E/F | BOTH_MESSAGE + header ASIO | ProtocolSend.h, ProtocolAsio.h | 580472e | BOTH_MESSAGE=0x000C (corrige 0x000B) | CONFIRMED (com 1 SUPERSEDED interno) | APROVADO | 0x000B → 0x000C |
| 0C.4-G/H | Fila ASIO + error recovery send | ProtocolAsio.h, ZzzScene.cpp, ProtocolSend.cpp | 580472e | sem retry/callback; leak fila; polling IsConnected | CONFIRMED | APROVADO | ver §5 |
| 0C.5-A | Dispatch cliente (mapa case→handler) | WSclient.cpp | 580472e | switch(HeadCode):12823; contagem preliminar 125 top-level | CONFIRMED→parcial SUPERSEDED | APROVADO | contagem corrigida em 0C.5-B/C |
| 0C.5-B | Mapa HeadCode→Handler (script do usuário) | WSclient.cpp | 580472e | 157 entries; artefatos do parser documentados | CONFIRMED | APROVADO (entregue em chat na sessão corrente) | ver §5 |
| 0C.5-B-audit | Reconciliação 157 vs 125 | WSclient.cpp (cópia /tmp) | 580472e | 28 suspeitos = TODOS top-level reais (brace-depth string-aware) | CONFIRMED | ENTREGUE (chat) | base da correção 125→156 |
| 0C.5-C | Lista canônica top-level (script do usuário) | WSclient.cpp | 580472e | 157 labels = 156 cases + default; 18 diretivas #ifdef | CONFIRMED | ENTREGUE (chat) | crash `os` previsto+corrigido |
| 0C.6-A | Entrypoints servidor (receive+dispatch) | Protocol.cpp, SocketManager.cpp, SocketManagerModern.cpp + árvore GitHub | 580472e | ProtocolCore:76/switch:98; 2 caminhos recv; 5 runs (2 microtestes de guard) | CONFIRMED | APROVADO | ver §5 (trilha de correções) |
| 0C.6-A-2 | Evidência mínima dos 3 arquivos | idem | 580472e | run verbatim limpo; PROTOCOL_CODE2/3 nos cases | CONFIRMED | APROVADO | ver §5 |
| 0C.6-B | Enum ProtocolHead servidor | SocketManagerEnum.h (+cadeia de includes) | 580472e | def :3-22; BOTH_MESSAGE=12=0x000C; idêntico ao cliente | CONFIRMED | APROVADO | 2 microtestes de guard |
| 0C.6-C | Resolver PROTOCOL_CODE2/3 | Protocol.h, stdafx.h | 580472e | LANG=1 → CODE2=0x11, CODE3=0x15 (+CODE1=0xD4, CODE4=0xDB); static_asserts | CONFIRMED | APROVADO | 8 branches por GAMESERVER_LANGUAGE |
| 0C.6-D | Framing moderno servidor | Time/include/olcPGEX_Network.h | 580472e | message_header{T id; u32 size}=6B; sem hton/ntoh | CONFIRMED | APROVADO | fallback path do script inexistente |
| 0C.6-E | Validação header.size (server recv) | olcPGEX_Network.h | 580472e | só `size>0`; resize sem teto; disconnect só por ec | CONFIRMED | APROVADO | espelha cliente |
| 0C.6-F | Cópia BOTH_MESSAGE→recv[8024] | SocketManagerModern.cpp | 580472e | SEM check; loop u16; 3 regimes de cópia no arquivo | CONFIRMED | EXECUTADO/ENTREGUE (chat) — auditoria formal pendente no livro-mestre | ver §5 |
| 0C.6-F-v2 | Idem, redes largas | idem | 580472e | +memset/clamp CharacterInfo; sem divergência | CONFIRMED | EXECUTADO/ENTREGUE | |
| 0C.6-G | Parse/handoff BOTH_MESSAGE | SocketManagerModern.cpp | 580472e | ExtractPacket original comentado (:180-203); sem else no parse; single-shot | CONFIRMED | EXECUTADO/ENTREGUE | reexecutado 2× idêntico |
| 0C.6-G-snip | Dumps 105-175/175-220 | idem | 580472e | BOTH_ATTACK3 sem case tipado; mapa tipado 8 linhas | CONFIRMED (+1 inferência rotulada) | EXECUTADO/ENTREGUE | case @104 inferido BOTH_CONNECT_LOGIN |
| 0C.6-H | PacketManager (AddData/ExtractPacket/XorData) | PacketManager.cpp/.h | 580472e | guard (0,2048); C1/C2-only default-reject; XOR 32B ativo; C3/C4 mortos no moderno | CONFIRMED | EXECUTADO/ENTREGUE | ver §5 |
| 0C.6-GH-pack | Pacote de evidência G+H | 3 arquivos | 580472e | 13 âncoras, zero divergência | CONFIRMED | EXECUTADO/ENTREGUE | |
| 0C.6-I | Receive clássico (defesas/C3/C4/threads) | SocketManager.cpp | 580472e | cadeia de defesas completa :257-295; Decrypt :301/:335; Encrypt :451/:464; IOCP+ServerQueueThread | CONFIRMED | EXECUTADO/ENTREGUE | quirk :336 registrada |
| 0C.6-I-pack | Evidência A/B/C (send+worker) | SocketManager.cpp | 580472e | serial injection :449/:462; guard+Disconnect :474; ENCRYPT_STATE hook :482 | CONFIRMED | EXECUTADO/ENTREGUE | |
| 0C.6-J | ENCRYPT_STATE/EncryptData | stdafx.h, HackCheck.h, Util.cpp | 580472e | ENCRYPT_STATE=0 (morta); PROTECT_STATE=1; callsites mapeados | CONFIRMED | EXECUTADO/ENTREGUE | |
| 0C.6-K | Pacote camada transporte (4 arquivos) | stdafx.h, HackCheck.h, SocketManager.cpp, Util.cpp | 580472e | **Gateway global DataSend→BOTH_MESSAGE (Util.cpp:214-218)**; tier Free/Premium; side-buffer | CONFIRMED | EXECUTADO/ENTREGUE | levantou incerteza XOR s→c |
| 0C.6-L | Implementação EncryptData/MHP | HackCheck.cpp | 580472e | ADD+XOR 2 chaves; derivação por licença; VM markers | CONFIRMED | EXECUTADO/ENTREGUE | |
| 0C.6-M | Origem m_CustomerName/m_ServerSerial | ServerInfo.h/.cpp, GameServer.cpp, .dat | 580472e | ReadStartupInfo ← GameServerInfo - Common.dat; valores reais | CONFIRMED | EXECUTADO/ENTREGUE | MuOnline / TbYehR2hFUPBKgZj |
| 0C.6-M-pack | Evidência + .dat via RAW | idem + 2 .dat | 580472e | DATA/ maiúsculo canônico; valores repo-side | CONFIRMED | EXECUTADO/ENTREGUE | |
| 0C.6-N | Existência MHPServer.ini | 8 caminhos RAW + workspace | 580472e | NÃO existe → MHP keys=0 → camada morta | CONFIRMED | EXECUTADO/ENTREGUE | |
| 0C.6-O | VM_START/VM_END | ThemidaSDK.h, HackCheck.cpp | 580472e | SDK Themida/WinLicense (Oreans) real; no-op sem protector | CONFIRMED | EXECUTADO/ENTREGUE | VMProtectSDK*.h inexistentes |
| 0C.6-O-pack | Evidência include+defines | idem | 580472e | pacote com 2 contextos largos | CONFIRMED | EXECUTADO/ENTREGUE | |
| 0C.4-I | Censo decode/crypto no cliente | WSclient.cpp | 580472e | BuxConvert (campo); sem XOR de frame no cliente; XOR unidirecional c→s | CONFIRMED | EXECUTADO/ENTREGUE | resolve incerteza 0C.6-K |
| (aux) dump 55-75/120-150 | Trechos SMM (comando mínimo) | SocketManagerModern.cpp | 580472e | DecBuff[8192] static :150; síntese 0xF3/03 :125 | CONFIRMED | EXECUTADO/ENTREGUE | |

Status de aprovação: 0A/0B/0C.1–0C.5 e 0C.6-A..E = **APROVADOS** (livro-mestre do coordenador, mensagem de auditoria 0C.6-E no chat [CHAT]). 0C.6-F..O e 0C.4-I = **executados e entregues em chat**, aguardando registro/aprovação formal do coordenador (o livro-mestre mais recente visível nesta sessão parava em 0C.6-E com "próximo: 0C.6-F").

--------------------------------------------------------------------------------

## 5. HISTÓRICO DETALHADO DE CADA MICROTESTE

### 0C.4-H — ASIO SEND ERROR RECOVERY (cliente) [MEM]
- Objetivo: comportamento do envio ASIO em falha e recovery de conexão.
- Arquivos: `Source Client/source/ProtocolAsio.h`, `ZzzScene.cpp`, `ProtocolSend.cpp`.
- Evidências: fila `tsqueue<message<T>> m_qMessagesOut` (:611; deque+scoped_lock :141-233); `Send` :350-366 via `asio::post`, gate `bWritingMessage` = 1 `async_write` por vez; falha → `cout` + `m_socket.close()` SEM retry (:412-414/:443-445).
- Conclusões: recovery SEM notificação — detecção por polling `IsConnected` (ZzzScene.cpp:2531 → MessageBox CServerLostMsgBoxLayout); SEM reconexão automática; SEM OnDisconnect; fila nunca limpa — `release()` (não `reset`) = LEAK; nova conexão = new CustomClient (ProtocolSend.cpp:19).
- Classificação: CONFIRMED. Auditoria: APROVADO (livro-mestre).
- Comando original: [PENDING — EVIDÊNCIA ORIGINAL NÃO RECUPERADA] (fase pré-compactação; conclusões preservadas em [MEM]).

### 0C.5-A — DISPATCH CLIENTE (mapa case→handler) [MEM]
- Assinatura: `BOOL TranslateProtocol(int HeadCode, BYTE*, int, BOOL bEncrypted)` — WSclient.cpp **:12821** (protótipo :11646; chamada :11763; fim da função :14317).
- `switch(HeadCode)` **:12823**, bloco **12824-14314**; 367 labels = 365 cases + 2 defaults.
- Contagem preliminar por indentação (`^\tcase `): **125 top-level + ~240 aninhados em ~20 sub-switches** (maiores: 0xF1=34, login-Value=24, 0xF3=23, :13783=26). **SUPERSEDED por 0C.5-B/C** (motivo: indentação inconsistente escondia 28 cases; ver §13).
- default: `break;` e a função **sempre retorna TRUE** (opcode desconhecido ignorado sem log).
- Consts `PACKET_MOVE/PACKET_POSITION/PACKET_ATTACK` em wsclientinline.h:27-30.
- Armadilhas de heurística do script (histórico): `def_re` casava a CHAMADA `if (!TranslateProtocol(...)){` :11763 → fix: ancorar em início de linha exigindo tipo de retorno.
- Mapa de famílias: preservado em §10.

### 0C.5-B — MAPA HeadCode→HANDLER (script literal do usuário) [CHAT]
- Script autossuficiente: download raw@580472e (415.884 B) → `/tmp/new_era_WSclient.cpp`; localiza def real (exclui protótipo terminado em `;` — armadilha corrigida por microteste após run 1 casar o protótipo :11646).
- Resultado run 2: def 12821, switch 12823, **ENTRIES 157** (156 case rows) → `/tmp/new_era_0C5B_opcode_map.md`; output `/tmp/0C5B_out.txt`.
- Artefatos do parser documentados (não corrigir, registrar): (i) profundidade atualizada com src[i-1] após i++ → ~31 labels aninhados contados como top-level em variants de contagem; (ii) `first_handler_call` pula linhas iniciadas por `if` → `<none>` FALSO em 0x19/0x69/0x1E/0x16/0x9C/0x30/0x36 (corpos `if (!ReceiveX(...))`; handlers reais conhecidos da fase A: ReceiveMagic, ReceiveMonsterSkill, ReceiveMagicContinue, ReceiveDieExp, ReceiveDieExpLarge, ReceiveTalk, ReceiveTrade — mapeamento 1:1 exato por opcode [PENDING — não recuperável com segurança]); (iii) `switch`/`if` legítimos como "handler" nas famílias sub-dispatch (F1/F3/F4/EB/BC/8E/F6/F7/F8/F9/AA/3F/B2/BD/D1/BF).

### 0C.5-B-audit — RECONCILIAÇÃO 157 vs 125 [CHAT]
- Incidente: `/tmp/mu/WSclient.cpp` sumiu (3ª limpeza de /tmp) no meio da auditoria → auditoria re-feita contra `/tmp/new_era_WSclient.cpp`.
- Método: contagem de profundidade de chaves REAL (ignorando strings/comentários) — o que o compilador vê; a indentação (2-3 tabs) provou não ser prova de escopo.
- Veredito: os 28 suspeitos (0x2C 0x34 0x44 0x46 0x47 0x4A 0x4B 0x68 0x94 0x95 0x96 0x9B 0x9F 0xA0 0xA7 0xA8 0xA9 0xAF 0xB1 0xB6 0xB7 0xB8 0xB9 0xBB 0xD2 0xDE 0xEE 0xEF) são **TODOS top-level reais**.
- Handlers dos 28 recuperados da tabela do script (lista completa em §10.3).

### 0C.5-C — LISTA CANÔNICA TOP-LEVEL (script do usuário) [CHAT]
- Run 1 verbatim: crasheou na gravação do markdown (`NameError: name 'os'` — o script usava `os.environ.get` importando só `re, sys`) — **previsível e previsto antes do run**; TODA a evidência tinha sido impressa antes do crash.
- Microteste: `import re, sys, os`. Run 2: EXIT=0 → `/tmp/new_era_0C5C_top_level_headcodes.md`.
- Evidências: def :12821 `BOOL TranslateProtocol( int HeadCode, BYTE *ReceiveBuffer, int Size, BOOL bEncrypted)`; corpo 12822..14317; switch :12823 `switch( HeadCode )`; bloco 12824..14314; **TOTAL_TOP_LEVEL_LABELS=157 (156 cases + default@14312)**.
- 18 diretivas de pré-processador no bloco; cases condicionais: **0x4A/0x4B** (`PBG_ADD_NEWCHAR_MONK_SKILL` 14305/14308), **0xD2** (`KJH_PBG_ADD_INGAMESHOP_SYSTEM` 14242), **0xDE** (`LJH_ADD_SYSTEM_OF_EQUIPPING_ITEM_FROM_INVENTORY` 14220); demais diretivas (ASG_ADD_TIME_LIMIT_QUEST, ASG_ADD_GENS_SYSTEM, PBG_ADD_GENSRANKING, PBG_ADD_SECRETBUFF) afetam sub-cases (0xF6/0xF8/0xBF/0xD2).
- Triplo cross-check independente: 0C.5-B (parser entries)=157 ✔ · 0C.5-C (parser depth)=157 ✔ · auditoria string-aware das 28 divergências=todas depth 1 ✔ → **número canônico fechado: 156**.

### 0C.6-A — SERVER ENTRYPOINTS [CHAT]
- Trilha de execução (importante para a história): run 1 verbatim EXIT=0 mas respondeu só CS/JS/DS (best=`JoinServerProtocol.cpp`, `switch(head)`@18) — a descoberta LOCAL achou candidatos (workspace tem CS/JS/DS) e curto-circuitou o fallback RAW; GameServer (alvo preferencial) ausente do workspace truncado.
- Microteste v1 (guard `"gameserver" in path"` no ramo de load): insuficiente — resquício local `GameServer/Time/include/olcPGEX_Network.h` (basename casa hint `net`) manteve o ramo LOCAL.
- Microteste v2 (guard exige basename `protocol|iocp|socket|connection|recv` no GS local): ramo local desarmado, mas STOP determinístico — causa raiz: **dois portões** (Step 2 `if not local_candidates:` nunca populava `remote_paths`) + `fallback_common` com 10 paths **todos 404** (layout real é `GameServer/GameServer/` duplicado). Sonda descartou hipótese de rede (API 200 nos dois endpoints).
- Microteste v2b (mesmo predicado no portão do Step 2): **SUCESSO** — árvore GitHub não-truncada (18.372 entries) → 65 candidatos RAW → best `Source Server/GameServer/GameServer/Protocol.cpp` (score 7).
- Evidências-chave: `ProtocolCore(BYTE head, BYTE* lpMsg, int size, int aIndex, int encrypt, int serial)` **Protocol.cpp:76**; `switch(head)` **:98**; sub-switches `switch(lpMsg[3])` :176/221/241/291/381/419/433; recv clássico `SocketManager.cpp` (WSASocket :141-159; `head=lpMsg[count+2]` :275 / `[count+3]` :281; `head=DecBuff[2|3]` :306/:340; drain→`ProtocolCore` :882); moderno `SocketManagerModern.cpp` (BOTH_MESSAGE :132; recv[8024] :134; `ProtocolCore` :171); log PacketIN :83-93; familíares inter-server: CSProtocol.cpp:81 ChatServerProtocolCore, DSProtocol.cpp:58, JSProtocol.cpp:15, ESProtocol.cpp:89, ConnectServerProtocol.cpp:10; Connection.cpp:158 `wsProtocolCore`.
- Incertezas registradas na época: ExtractPacket internals (resolvido em 0C.6-H), papel de Connection.cpp (ainda UNCERTAIN), camada ativa em runtime (parcialmente resolvido em 0C.6-K), contagem de cases do switch(head) (ainda PENDING).

### 0C.6-A-2 — EVIDÊNCIA MÍNIMA [CHAT]
- Script autossuficiente, 3 fetches (135.014 / 21.153 / 4.859 B). **Run 1 verbatim limpo — nenhum STOP/NOTE** (primeiro script da série sem correção).
- Âncoras: sig :76, switch :98, cases iniciais `0x00@100, 0x02@103, 0x03@106, 0x0E@109, PROTOCOL_CODE2@112, PROTOCOL_CODE3@115, 0x18@118, 0x19@121, 0x1B@124, 0x1C@127, 0x1E@130, 0x22@133`; SocketManager :275/:281/:306/:340 + callsite único :882; SMM :132/:134/:171.
- Novidades: constantes simbólicas nos cases do servidor; vizinho `case ProtocolHead::BOTH_POSITION:`@110.

### 0C.6-B — ENUM ProtocolHead (SERVIDOR) [CHAT]
- Run 1: HIT=MENTION_ONLY em Protocol.cpp:1441 (`gSocketManagerModern.PacketSend(aIndex,ProtocolHead::BOTH_POSITION,...)`) — o ramo de includes fazia `break` no primeiro mention e o tree-scan só rodava com `hit is None`.
- Sonda da cadeia (evidência preservada): `SocketManagerModern.h` (1.316 B, token sem def; usa `olc::net::message<ProtocolHead>`) → `SocketConnection.h` (874 B/25 linhas; `CSocketConnection : public olc::net::server_interface<ProtocolHead>`; inclui `SocketManagerEnum.h`) → **definição**.
- Microtestes: (a) MENTION_ONLY vira fallback e o loop continua; (b) tree-scan roda também quando hit≠ENUM_DEF.
- Run 2: **ENUM_DEF `Source Server/GameServer/GameServer/SocketManagerEnum.h:3-22`** — `enum class ProtocolHead : uint16_t`, 13 membros: CLIENT_ACCEPT(0), CLIENT_LIVE_CLIENT(1), SERVER_CONNECT(2), SERVER_DISCONNECT(3), BOTH_CONNECT_LOGIN(4), BOTH_CONNECT_CHARACTER(5), **BOTH_POSITION(6)**, BOTH_MOVE(7), BOTH_ATTACK1(8), BOTH_ATTACK2(9), BOTH_ATTACK3(10), BOTH_CONNECT_JOIN_GAME(11), **BOTH_MESSAGE(12=0x000C)**.
- Cross-check: enum do cliente (`Source Client/source/ProtocolSend.h:7-26`) **idêntico membro a membro** — interoperabilidade por construção.
- O arquivo é o menor do filtro do tree-scan (377 B) — o script do usuário acharia sozinho após os microtestes.

### 0C.6-C — PROTOCOL_CODE2/3 [CHAT]
- Run 1 verbatim limpo (pontos quentes previstos eram reais porém inofensivos: `StdAfx.h` 404 — arquivo real `stdafx.h` minúsculo; `find_definitions` misturou menções em comentários :227/:471/:494).
- Defs: `Protocol.h:8-48` multi-branch `GAMESERVER_LANGUAGE` 0-7: LANG0{1=D3,2=D7,3=DF,4=0x10} · **LANG1{1=D4,2=0x11,3=0x15,4=0xDB}** · LANG2{1D,DC,D6,D7} · LANG3{D9,D7,D0,1D} · LANG4{00,00,00,00} · LANG5{D6,DD,DF,D2} · LANG6{DD,D6,DF,11} · LANG7{D9,15,DC,1D}.
- Build: `stdafx.h:48 #define NEW_PROTOCOL_SYSTEM 1`; `stdafx.h:51 #define GAMESERVER_LANGUAGE 1` (incondicional; `#ifndef` da :50 comentado) → **CODE2=0x11 (PACKET_ATTACK), CODE3=0x15 (PACKET_POSITION)** (+CODE1=0xD4=PACKET_MOVE, CODE4=0xDB).
- `static_assert`s ativos Protocol.h:54-58 (+`#pragma message` de aviso para LANG≠1 :59-60); comentário :52 documenta a intenção ("Client uses: PACKET_MOVE=0xD4, PACKET_ATTACK=0x11, PACKET_POSITION=0x15").
- Limitação registrada: VALUE_PARSE best-effort do script imprimiu 0xD7/0xDF (branch LANG0) — em blocos #if multi-branch, literal ≠ valor de build.
- Usos: Protocol.cpp {83 (filtro de log), 112 (case), 115 (case), 768 (case CODE1), 1423 (send CODE3), 1920 (send CODE1), 2508 (send CODE2)}.
- Incerteza: PROTOCOL_CODE4=0xDB não aparece entre os 156 cases top-level do cliente — destino UNCERTAIN.

### 0C.6-D — FRAMING MODERNO SERVIDOR [CHAT]
- Árvore achou `Source Server/GameServer/GameServer/Time/include/olcPGEX_Network.h` (29.939 B); o fallback embutido do script (`GameServer/Time/include/...` sem duplicação) **não existe** — o tree-scan salvou o run.
- `message_header<T>` :98-103 (`T id{};` :101; `uint32_t size = 0;` :102) = **6 B** com T=ProtocolHead:u16; `message<T>` :109-119 (header + vector body; `size()` retorna só body — mesma quirk do cliente).
- I/O: WriteHeader :458-463 / ReadHeader :535-542 / ReadBody :580 — struct crua via `asio::buffer(&header, sizeof(...))`; **ZERO hton/ntoh no arquivo** → wire little-endian (x86).
- Guard `#if(NEW_PROTOCOL_SYSTEM==1)` @60. Incidental: `//ReadHeader(); //No Encrypt` :378/:404.

### 0C.6-E — VALIDAÇÃO header.size (SERVER RECV) [CHAT]
- Única checagem: `if (m_msgTemporaryIn.header.size > 0)` **:549** → true: `body.resize(header.size)` **:553** + `ReadBody()` :554; false: `AddToIncomingMessageQueue()` :560 (mensagem sem body é ACEITA e processada).
- **Sem clamp/teto**; `MAX_MAIN_PACKET_SIZE` (8192) não consultado; campo u32 controlado pelo cliente → risco de alocação gigante.
- Disconnect APENAS por `error_code` ASIO (:563-566 e equivalente no ReadBody) → `OnClientDisconnect` + `m_socket.close()`. Sem política de disconnect por tamanho.
- Espelha o cliente (0C.4-E). Auditoria: **APROVADO** pelo coordenador (PROBABLE: DoS por alocação — consequência direta).

### 0C.6-F / 0C.6-F-v2 — BRIDGE BOTH_MESSAGE→recv[8024] [CHAT]
- v1 verbatim limpo; v2 (redes largas: memset, len/length, contextos até EOF) sem divergência.
- Evidências: case :132; `static uint8_t recv[8024];` **:134 (static, não stack — contrasta com o cliente)**; cópia :136-138 `for (uint16_t start = 0; start < msg.header.size; start++) std::memcpy(&recv[start], &msg.body[start], 1);` — **SEM nenhum check de tamanho**.
- Contraste no MESMO arquivo: `PacketSend` :59-65 TEM guard (`size > MAX_MAIN_PACKET_SIZE` → log+drop :61-64); `CGCharacterInfoRecv` :126-128 TEM clamp (`copySize = min(body.size(), sizeof(name))`) + memset.
- Regimes de `header.size` na recepção (cadeia causal registrada): ≤8024 normal; 8025-65535 escrita OOB além do buffer static; **>65535 loop infinito (wrap de uint16_t)**; ==0 processa resíduo (buffer static não zerado).
- Pós-cópia: parse clássico :153-164; `gPacketManager.AddData(&recv[count],size)`/`ExtractPacket(DecBuff)` :166 (falha→return); `ProtocolCore(head,DecBuff,size,aIndex,0,0)` :171.

### 0C.6-G (+rerun, +snip) — PARSE/HANDOFF [CHAT]
- Executado 2× com resultado idêntico (+1 dump complementar).
- Achados: (A) **`ExtractPacket` ORIGINAL comentado** :180-203 (switch C1/C2 + `default: return 0` + `XorData((size-1),end)` — rejeição e XOR existiam no código da ponte); (B) switch tipado vizinho :107-131 (BOTH_CONNECT_CHARACTER→CGCharacterListRecv :108; BOTH_POSITION→CGPositionRecv :111; BOTH_MOVE→CGMoveRecv :114; BOTH_ATTACK1→CGAttackRecv :117; BOTH_ATTACK2→CGMultiSkillAttackRecv :120; BOTH_CONNECT_JOIN_GAME→síntese `header.set(0xF3,0x03)` :125→CGCharacterInfoRecv :129); (C) parse :153-164 **sem else** — header ∉{C1,C2,C3,C4} → size=0, `head` não-inicializado (:151), fluxo continua (posteriormente NEUTRALIZADO pelo guard do AddData — ver 0C.6-H); (D) ponte **single-shot** (1 AddData + 1 ExtractPacket + 1 ProtocolCore por BOTH_MESSAGE; `count=0` fixo).
- snip 105-175/175-220: **BOTH_ATTACK3 é o único membro BOTH_* de gameplay sem case tipado** neste switch; case @104 (fora do dump) inferido por padrão como BOTH_CONNECT_LOGIN — **inferência rotulada, não evidência**; `static BYTE DecBuff[MAX_MAIN_PACKET_SIZE]` :150.

### 0C.6-H — PACKETMANAGER [CHAT]
- Objeto: `gPacketManager` (SMM:166); implementação `Source Server/GameServer/GameServer/PacketManager.cpp` (11.001 B) + `.h` (1.521 B).
- Classe (PacketManager.h:32-64): `m_buff[2048]` :60; `m_size` :61; `m_XorFilter[32]` :63; `#if(GAMESERVER_UPDATE>=701)` → Crypto++ `ECB_Mode<DES_XEX3>` :52-54 (**dormente** — build 502 → branch `ENCDEC_DATA` :56-58 ativa); `extern CPacketManager gPacketManager` :66.
- **AddData :530-541**: `if(size <= 0 || size >= 2048) return 0;` → **guard (0,2048)**; memcpy para m_buff[2048] sempre in-bounds.
- **ExtractPacket :542-570**: switch `m_buff[0]` — só **C1/C2**; `default: return 0` (**C3/C4 são DROPPADOS no caminho moderno** — simetria com o cliente, que não decifra C3/C4 embutidos); `if(m_size < (DWORD)size) return 0` (tautológico na ligação atual: m_size=size do AddData); `XorData((size-1),end)` :565; `memcpy(lpBuff, m_buff, size)` (≤2047 → DecBuff[8192] in-bounds).
- **XorData :572-580**: `for(n=start; n>end; n--) m_buff[n] ^= m_buff[n-1] ^ m_XorFilter[n%32];` — cascata reversa, filtro 32 B.
- SimpleModulus completo no arquivo: Encrypt :214, Decrypt :268, EncryptBlock :319, DecryptBlock :369, AddBits :430, GetByteOfBit :474, Shift :487; expansão 8→11 visível :242-244 (`dec=(size+7)/8; size=((dec+(dec*4))*2)+dec`); LoadEncryptionKey/LoadDecryptionKey/LoadKey :121-147.
- Correção de registro: o "head não-inicializado" de 0C.6-G(C) **nunca chega ao ProtocolCore** (AddData(ptr,0) retorna 0 → return).

### 0C.6-I (+pack) — RECEIVE CLÁSSICO [CHAT]
- `DataRecv` SocketManager.cpp:255 — **cadeia de defesas completa**: `size<3 → return 1` :257; `else { gLog "Protocol header error (…Header: %x)"; return 0; }` **:283-287 (default-reject + LOG)**; `size<3 || size>MAX_MAIN_PACKET_SIZE → log + return 0` :289-293 (clamp 3..8192); `size<=lpIoBuffer->size` :295 (frame completa); `while(true)` :269 (multi-frame por recv).
- C3 :299-331: `Decrypt(&DecBuff[1],&lpMsg[count+2],(size-2))+1` :301 → DecSerial=DecBuff[1] :303, head=DecBuff[2] :306 → **C1 sintético** (:305-309) → AddData/ExtractPacket (XOR) :311 → QUEUE_INFO{index,head,buff,size,encrypt=1,serial} :316-326 → semáforo :328-331.
- C4 :335-366: análogo com C2 sintético (HIBYTE/LOBYTE :343-344); **quirk :336 `DecSize = size;` sobrescreve o retorno do Decrypt** (o `+2` computado é descartado) — aparente artefato copy-paste, registrado sem análise.
- Plain C1/C2 :369-380+; **Decrypt/Encrypt (SimpleModulus) usados SÓ no caminho clássico** (recv :301/:335; send :451/:464).
- Send (pack B :430-490): `send[MAX_MAIN_PACKET_SIZE]` static :439; injeção de serial `gSerialCheck[index].GetSendSerial()` :449 (C3)/:462 (C4) com save/restore; Encrypt :451/:464; rebuild :455-456/:468-470; **guard `size>MAX → log+Disconnect`** :474-479 (o mais duro do código-base); `#if(ENCRYPT_STATE==1) EncryptData(send,size)` :482-486; per-socket IoSendContext :488; side-buffer :490-504 (guard `MAX_SIDE_PACKET_SIZE` "Type: 2" → Disconnect).
- Topologia (pack C): `ServerWorkerThread` :821 (GetQueuedCompletionStatus INFINITE :829; erros tolerados :831) → switch IoType :850 (IO_RECV→OnRecv / IO_SEND→OnSend, sob m_critical) → fila+semáforo → **`ServerQueueThread` :866-888 dedicada, THREAD_PRIORITY_HIGHEST :246** → valida OBJECT_RANGE+Connected :880 → **ProtocolCore :882**.
- OnRecv :571-620: `IoSize==0 → Disconnect`; `ENCRYPT_STATE==1 → DecryptData` :590-592; `DataRecv()==0 → Disconnect` :598-603; `wsabuf.len = MAX_MAIN_PACKET_SIZE−size` :607 (buffer de recepção limitado).

### 0C.6-J — ENCRYPT_STATE / EncryptData [CHAT]
- **`ENCRYPT_STATE=0`** @stdafx.h:57-59 (`#ifndef` default; nenhum override visto) → camada de transporte compilada FORA no build. `PROTECT_STATE=1` :54-55.
- Callsites (todos dormentes): SocketManager.cpp :482-486 (send), :588-592 (OnRecv recv — sobre bytes crus pré-parse), Util.cpp :240-244 (`DataSendSocket`, antes de `send()` com retry WSAEWOULDBLOCK :250-255).
- Declarações: HackCheck.h:3-6 — `DecryptData/EncryptData/MHPDecryptData/MHPEncryptData/InitHackCheck`.

### 0C.6-K — PACOTE CAMADA DE TRANSPORTE [CHAT]
- **Achado maior: gateway global de envio `Util.cpp:214-218`** — a `DataSend(aIndex,…)` central: `#if (NEW_PROTOCOL_SYSTEM==1) gSocketManagerModern.PacketSend(aIndex, ProtocolHead::BOTH_MESSAGE, lpMsg, size); return 1; #else return gSocketManager.DataSend(...)` — **todo outbound do game-code viaja como BOTH_MESSAGE neste build**; caminho clássico de envio bypassado (só rodaria com NEW_PROTOCOL_SYSTEM=0). Espelho exato do cliente (0C.3). (Log comentado "PacketOUT" :210-212 delata o papel.)
- Secundários: tier `PROTECT_STATE` 0→"Free"/1→"Premium" (stdafx.h:62-66); side-buffer com guard+Disconnect; OnRecv com disconnect em erro de protocolo (contraste: moderna retorna silencioso); HackCheck.h = 5 declarações.
- **Incerteza levantada (resolvida em 0C.4-I)**: XOR seria simétrico servidor→cliente?

### 0C.6-L — HACKCHECK.CPP (implementação) [CHAT]
- Arquivo minúsculo: 98 linhas / 2.072 B. Globais `EncDecKey1/2`, `MHPEncDecKey1/2`, `MHPCustomerName[32]`.
- Algoritmos (byte-a-byte, inversos exatos): `EncryptData` :25-30 `(lpMsg[n]+Key2)^Key1` (+MHP depois); `DecryptData` :12-23 (MHP antes, se ativa) `(lpMsg[n]^Key1)−Key2`; MHP variants :38-52.
- `InitHackCheck` :54-98: `EncDecKey = Σ(m_CustomerName[n] ^ m_ServerSerial[n%sizeof(serial)])` :58-63; `PROTECT_STATE==0` → **0xE2/0x76** (constantes públicas clássicas do MU) :65-67; `#else` (Premium=build) → **0xEE/0xFF** :69-70; `+= LOBYTE/HIBYTE(EncDecKey)` :75-76; MHP lê `..\Data\Hack\MHPServer.ini` :78-94 (só ativa se keys≠0); `VM_START` :56 / `VM_END` :97.
- Classificação: bind de licença trivialmente contornável, não segurança de dados.

### 0C.6-M (+pack) — ORIGEM DAS CHAVES [CHAT]
- Struct: ServerInfo.h **:49** `char m_CustomerName[32]`; **:63** `char m_ServerSerial[17]`.
- Carga: `CServerInfo::ReadStartupInfo` ServerInfo.cpp **:793** (CustomerName :797; ServerSerial :825 — `GetPrivateProfileString`).
- Caller: **GameServer.cpp:56** `gServerInfo.ReadStartupInfo("GameServerInfo",".\\Data\\GameServerInfo - Common.dat")`.
- Valores REAIS (dupla fonte: workspace local E RAW do repo): **CustomerName = MuOnline; ServerSerial = TbYehR2hFUPBKgZj** (idênticos em GameServer e GameServerCS; RAW: MuServer/GameServer/DATA/ 21.950 B e MuServer/GameServerCS/DATA/ 10.053 B, valores às linhas :5/:15 de cada).
- Descoberta de ambiente: caminho canônico é **`DATA/` maiúsculo** (`Data/` dá 404).
- Incidental: ReadStartupInfo também carrega ServerName/Code/Lock/Port, mutex `WZ_MU_GS_MUTEX_%d` :809, ServerVersion parseado char a char :813-823 (índices [0],[2],[3],[5],[6] — formato "x.x.xxxx"), clamp ServerMaxUserNumber :829, endereços DS/JS/CS :831-839.

### 0C.6-N — MHPServer.INI [CHAT]
- 8 caminhos RAW (DATA/Hack, DATA/HACK, variantes GS/GSCS/root) — todos MISS; `find` no workspace local: nenhum `MHP*` nem dir `Hack/` em todo o MuServer/.
- Consequência (cadeia fechada): `GetPrivateProfileInt` defaults → MHPEncDecKey1/2 = 0 → guard :14/:32 falso → **camada MHP nunca ativa neste deployment**. Cifra de transporte **duplamente morta** (macro 0 + chaves MHP ausentes).

### 0C.6-O (+pack) — VM_START/VM_END [CHAT]
- Tokens definidos em **`ThemidaSDK.h`** (25.002 B), incluído por HackCheck.cpp:4. `VMProtectSDK*.h`: inexistentes (3× MISS) — **não é VMProtect**.
- Macros por compilador: Borland `__emit__` :35-38; GCC `asm(".byte …")` :235-269; MSVC `__asm __emit` (~:430-570). Bytes: `EB 10 57 4C 20 20 [opcode] 00… 57 4C 20 20` — assinatura `57 4C 20 20` = **"WL  " (WinLicense/Themida, Oreans)**; opcodes 0x0C (VM_START) / 0x0D (VM_END).
- `EB 10` = jmp +16 → marcador auto-pulante: **no-op num binário não protegido**; ganha efeito somente se o binário passar pelo protector na esteira de build (não verificável do fonte).
- Irmãos no SDK: CODEREPLACE/ENCODE/CLEAR/UNPROTECTED/CHECK_PROTECTION/CHECK_CODE_INTEGRITY.

### 0C.4-I — CENSO DECODE/CRYPTO NO CLIENTE [CHAT]
- WSclient.cpp (415.884 B): `XorData/XorFilter/AddData/ExtractPacket/CStreamPacketEngine/SimpleModulus/Encrypt/BOTH_MESSAGE` = **0 ocorrências**; `Decrypt` ×3 (`g_SimpleModulusSC.Decrypt` :11686 (C3) / :11692 (C4); log serial :11708); `BuxConvert` ×4.
- **`BuxConvert` :209-215**: `static BYTE bBuxCode[3] = {0xFC,0xCF,0xAB}; Buffer[i] ^= bBuxCode[i%3];` — callsites :474 (Question), :487 (Password), :9357 (ChatMsg) → **ofuscação de campos** (senha/resposta/chat), NÃO decode de frame.
- `ProtocolCompiler` :11649+ consome C1/C2 **CRUS** (:11667-11678, casts PBMSG_HEADER/PWMSG_HEADER) → TranslateProtocol; C3/C4 → SimpleModulus+serial (:11686-11708).
- **Resolução da incerteza 0C.6-K: o XOR de frame é UNIDIRECIONAL (cliente→servidor)** — builder aplica (0C.3), servidor decodifica (0C.6-H); servidor→cliente viaja claro nas duas pontas (PacketSend cru + ProtocolCompiler cru). Consistência, não assimetria.
- Armadilha conhecida confirmada: 1º match do regex TranslateProtocol = protótipo :11646.

--------------------------------------------------------------------------------

## 6. FASE 0A — INVENTÁRIO / ARQUITETURA [FILE: NEW_ERA_PHASE_0A_REPOSITORY_AUDIT.md]

### CONFIRMED (extrato — documento completo é a fonte)
- Repo `wongddd/muonline`, clone íntegro na época (17 commits); HEAD `580472e` "v8: cross-directory texture search"; branch main à frente de origin/master (`b9d07d2`, 8 commits, diffs só docs).
- Origem declarada: privado MU Season 5 (EX502); GameServer família **"Louis Emulator"** (creandomu.com, comunidade BR); cliente das fontes WebZen S5; docs dos autores em chinês; strings PT-BR.
- Histórico: base S8 rebaixada para S5 (`8c68a0e`) + 8 rodadas de fixes v1-v8 (2026-05-26/27): compilar, conectar, logar, selecionar personagem, entrar, texturas.
- Cliente: Win32/VS2019 v142, `Main.sln`; **OpenGL fixed-function/immediate mode** (Winmain.cpp:877-893; `OpenGL3/Shader/` = stub vazio, Shader.cpp 0 linhas — "OpenGL 3" do README é aspiracional); DirectX9 SDK embutido para input (DIRECTINPUT 0x0800) e som; ASIO 1.19.1 standalone; lua 5.1 headers; cryptopp ×2.
- Servidor (todos Win32 GUI VS2019): GameServer (TCP 55901; 251 .cpp; configs Release_EX502/EX502CS), GameServerCS (55919, Castle Siege), ConnectServer (TCP 44405/UDP 55557; ServerList.dat), DataServer (55960; ODBC DSN MuOnline; AllowableIpList.txt), JoinServer (55970; MEMB_INFO; MD5 opcional).
- Topologia confirmada (0A §5.2): CLIENT→CS 44405→lista/IP→CLIENT→GS 55901; GS→DS 55960; GS→JS 55970; GS/JS→CS UDP 55557 heartbeats.
- `NEW_PROTOCOL_SYSTEM` ativo nos DOIS lados (Defined_Global.h cliente / stdafx.h servidor).
- Análise de matching dos autores (~60% cliente↔servidor) presente no repo (docs).
- Working tree LIMPO durante toda a 0A.

### PROBABLE
- [PENDING — nada específico recuperado além do relatório]

### SUPERSEDED
- Nada nesta fase.

--------------------------------------------------------------------------------

## 7. FASE 0B — GAME LOOP / FPS / TIMING [FILE: NEW_ERA_PHASE_0B_GAMELOOP_FPS_AUDIT.md]

### CONFIRMED (extrato — documento completo é a fonte)
- Entrada: `WinMain()` Winmain.cpp:1392; loop principal `while(1)` Winmain.cpp:1758 (PeekMessage → else Scene(g_hDC) → após Scene, sempre: ProtocolCompiler() / ChatRoomSocketList->ProtocolCompile() / [NEW_PROTOCOL_SYSTEM] gProtocolSend.RecvMessage()).
- **Tick ~40 ms (25 Hz)**: literal `40` hardcoded em ZzzScene.cpp:2320 (`for(Remain = TimeRemain; Remain >= 40; Remain -= 40)`) — sem constante nomeada.
- **Limiter ~25 FPS**: ZzzScene.cpp:2516-2524, após `glFlush(); SwapBuffers` — `std::this_thread::sleep_for(milliseconds(40 - DifTimer))` (sleep simples; não busy-wait/VSync); `TimePrior = GetTickCount()` :2452 medido ANTES do render (nota¹ do relatório: DifTimer mede entre inícios consecutivos de frames).
- **Relógios múltiplos** (tabela do relatório): `GetTickCount` (TimePrior; limiter+acumulador; 61 refs) · `timeGetTime` (CalcFPS ZzzAI.cpp:781-812: WorldTime→efeitos/iluminação, difTime→CalcSkillDelay, **DeltaT→código morto**; 84 refs) · `QueryPerformanceCounter` (CTimer Time/Timer.cpp; CInput, CUIMng clamp 200ms; 8 refs) · `std::chrono::sleep_for` (limiter; waits de rede).
- **`MoveSceneFrame`** = relógio de lógica: `++` por tick ZzzScene.cpp:2357; padrão `% N == 0` em CSWaterTerrain.cpp:41 (%40), GMBattleCastle.cpp:733/797/844 (%20/%60/%10), GMHellas.cpp:317+ (%100/%50), CSPetSystem.cpp:570 (%60)…
- Dentro do tick: input-poll, UI, movimento, animação (`PlayAnimation` sem dt), física (cloth ×5 substeps `g_PhysicsManager.Move(0.005f)` ZzzScene.cpp:2353/PhysicsManager.cpp:1207), efeitos, pets, IA mapas, `MoveSceneFrame++`.
- `CalcFPS()` ZzzAI.cpp:781-817: bug de unidade (timeGetTime ms ÷ CLOCKS_PER_SEC) → FPS exibido impreciso; FPS é só display — **não influencia lógica/movimento/render/rede**.
- Rede acoplada ao frame: recepção assíncrona, mas dispatch por frame (latência +0..40 ms a 25 FPS; rajadas processadas no frame seguinte; pacotes não perdidos por frame lento, exceto overflow do buffer 8192).
- Limitações catalogadas L1 (cap 25 FPS — CRÍTICA), L3 (rede por frame — ALTA), etc. (tabela completa no relatório).
- Conclusão de arquitetura (para o NEW-ERA): **clocks separados** são necessários (simulação/render/rede desacoplados) — registrado como requisito, não implementação.

### SUPERSEDED / DISCARDED
- Nada nesta fase.

--------------------------------------------------------------------------------

## 8. FASE 0C — NETWORK / PROTOCOL (cronológico)

### 8.1 — 0C.1 Connection Map [MEM] — CONFIRMED
- Client→ConnectServer TCP **44405** (WSAAsyncSelect clássico; F4:06 serverlist, F4:03 GS addr).
- Client→GameServer **55901** (ASIO moderno `|` CWsctlc clássico).
- GameServerCS **55919**; **SEM socket GS↔GSCS** (vinculação via MapServerInfo.dat + auth via JS).
- GS→JS **55970**; GS→DS **55960** (CConnection heartbeat 1 s); GS→CS **UDP 55557** one-way (0x01).
- JS/DS→ODBC (MEMB_INFO / 74 tabelas). NEW_PROTOCOL cobre **SÓ Cliente↔GS**.
- Incertezas (à época): ChatRoom, JS UDP, CS Protect, GameShop.
- Configs: `Main.info` (-y/-z/-u); `ConnectServer.ini`+`ServerList.dat`; `GameServerInfo - Common.dat`; `JoinServer.ini` (GlobalPassword **"XwefDastoD"**); `MapServerInfo.dat`.

### 8.2 — 0C.2 Client Receive [MEM] — CONFIRMED
- Clássico: FD_READ→`nRecv` (WSctlc.cpp:432, buffer 8192); parse `while(1)` (byte inválido → zera buffer, **SEM resync**) → CPacketQueue; drain 1×/frame se SceneFlag<CHARACTER_SCENE → `ProtocolCompiler` (WSclient.cpp:11649) → C3/C4 decrypt+serial → header sintético → TranslateProtocol.
- ASIO: ReadHeader 6 B → resize **sem teto** → tsqueue → RecvMessage (main-thread); BOTH_MESSAGE → memcpy body → `recv[8024]` **stack SEM bounds-check** → parse clássico (C3/C4 embutidos NÃO decifrados).
- Em jogo, tudo chega via BOTH_MESSAGE; processamento frame-bound (25 FPS).

### 8.3 — 0C.3 Client Send [MEM] — CONFIRMED
- Builder `CStreamPacketEngine` (Init placeholder / End patch / **XOR cascata**) → splitter `SendPacket` (**wsclientinline.h:67-124**, `__forceinline`): sempre SendPacketClassic→DataSend→BOTH_MESSAGE; SceneFlag>=CHARACTER_SCENE mata o ramo clássico.
- Login: serial `g_byPacketSerialSend++` + SimpleModulus → C3|C4 → `sSend` (não-bloqueante, m_SendBuf 8192 + FD_WRITE). Escolha C3/C4: `iSize<256 && !bForceC4 ? C3 : C4` (wsclientinline.h:101-115).
- Moderno: `asio::post` → fila → WriteHeader/WriteBody. **Frame clássico viaja como payload SEM serial/cifra.**

### 8.4 — 0C.4 Packet Framing / ASIO / BOTH_MESSAGE [MEM] — CONFIRMED
- **C1** 3 B `{0xC1,Sz,Head}` máx 255 (payload 252), opcode[2]; **C2** 4 B `{0xC2,SzH,SzL,Head}` máx 65535, opcode[3]; sub-opcode F1/F3/F4 = payload[0].
- **C3** `[0xC3][Sz:1][cifra(Serial,Head,Data)]` — serial+opcode DENTRO da cifra; SimpleModulus 8→11 B/bloco, 4 DWORD (Modulus/Enc/Dec/XOR); chaves: CS=Enc1.dat (envio) / SC=Dec2.dat (recv); máx ~180 B plaintext; reconstrói C1 sintético (WSclient.cpp 11679-11736); serial mismatch → só loga+ressincroniza.
- **C4** `[0xC4][Sz:2][cifra]` ciphertext ≤2048 ≈1488 plaintext; reconstrói C2 sintético.
- **BOTH_MESSAGE = 0x000C** (enum `ProtocolHead : uint16`, cliente ProtocolSend.h:7-26) — envelope ASIO 6 B + frame legado; header.size = body; SEM teto/validation; memcpy recv[8024] = risco stack overflow.
- ASIO `message_header{id:u16, size:u32}` = 6 B (ProtocolAsio.h:32-37); ZERO htonl = little-endian x86; ReadHeader 450-481 → resize :468 → ReadBody 489-511 → AddToIncoming :586-598.
- 0C.4-G/H: ver §5 (fila, error recovery).

### 8.5 — 0C.5 Opcode Dispatch (cliente)
Ver §10 (mapa completo + contagens + SUPERSEDED).

### 8.6 — 0C.6 Server-side (A..O)
Ver §5 (detalhes) e §11 (síntese).

--------------------------------------------------------------------------------

## 9. EVIDÊNCIAS DE PROTOCOLO (consolidado, CONFIRMED @580472e)

| Item | Valor/Evidência |
|---|---|
| C1 | 3 B {0xC1, Sz:1, Head}; máx 255; opcode byte[2] |
| C2 | 4 B {0xC2, SzH, SzL, Head}; máx 65535; opcode byte[3] |
| C3 | [0xC3][Sz:1][SimpleModulus(Serial,Head,Data)]; ~180 B plaintext máx; reconstrução C1 sintética (cliente 11679-11736; servidor :305-309) |
| C4 | [0xC4][Sz:2][cifra]; ciphertext ≤2048; reconstrução C2 sintética (cliente; servidor :342-344) |
| BOTH_MESSAGE | ID **0x000C** (enum membro 12); carrega frame legado como body |
| message_header | `{T id; uint32_t size}` = 6 B com T=u16 — cliente ProtocolAsio.h:32-37; servidor olcPGEX_Network.h:98-103 |
| Endianness | SEM hton/ntoh nos dois lados → wire little-endian (x86) |
| ProtocolHead | enum class : uint16_t, 13 membros, idêntico cliente (ProtocolSend.h:7-26) ↔ servidor (SocketManagerEnum.h:3-22) |
| ProtocolCore (server) | `void ProtocolCore(BYTE head,BYTE* lpMsg,int size,int aIndex,int encrypt,int serial)` — Protocol.cpp:76; switch(head):98; sub-switches lpMsg[3] |
| TranslateProtocol (client) | `BOOL TranslateProtocol(int HeadCode, BYTE* ReceiveBuffer, int Size, BOOL bEncrypted)` — WSclient.cpp:12821; switch(HeadCode):12823 |
| HeadCode | byte 2 (C1/C3) ou byte 3 (C2/C4) — cliente e servidor |
| SubCode | payload[0] (F1/F3/F4 cliente); lpMsg[3] (sub-switches servidor) |
| PROTOCOL_CODE1 | **0xD4** (build LANG=1) = PACKET_MOVE |
| PROTOCOL_CODE2 | **0x11** = PACKET_ATTACK (static_assert Protocol.h:56) |
| PROTOCOL_CODE3 | **0x15** = PACKET_POSITION (static_assert :57) |
| PROTOCOL_CODE4 | **0xDB** — uso UNCERTAIN (não está entre os 156 cases do cliente) |
| PacketManager (server) | CPacketManager; m_buff[2048]; m_XorFilter[32]; gPacketManager global |
| AddData | guard `(0 < size < 2048)`; memcpy p/ m_buff; PacketManager.cpp:530-541 |
| ExtractPacket | C1/C2-only + default-reject; check m_size; XorData; memcpy out; :542-570 |
| XorData | `m_buff[n] ^= m_buff[n-1] ^ m_XorFilter[n%32]` (cascata reversa); :572-580; **unidirecional cliente→servidor** (0C.4-I) |
| BuxConvert (client) | XOR 3 B {0xFC,0xCF,0xAB} — campos Question/Password/ChatMsg; WSclient.cpp:209-215 |
| Cifra transporte (server) | ADD+XOR 2 chaves (licença); **MORTA** (ENCRYPT_STATE=0 + MHP keys=0) |
| SimpleModulus | cliente g_SimpleModulusCS (Enc1/Dec2.dat); servidor PacketManager Encrypt/Decrypt — **só caminho clássico** |
| Serial | cliente g_byPacketSerialSend/Recv; servidor gSerialCheck[index].GetSendSerial() (:449/:462) + QueueInfo.serial |

--------------------------------------------------------------------------------

## 10. OPCODE DISPATCH — CLIENTE (WSclient.cpp @580472e)

### 10.1 Estrutura (CONFIRMED)
- Definição **:12821**; protótipo :11646; chamada :11763; fim :14317.
- `switch(HeadCode)` **:12823**; bloco **12824-14314**; 367 labels totais = 365 cases + 2 defaults.
- **CONTAGEM CANÔNICA: 156 HeadCodes top-level + 1 default (@14312) = 157 labels** (0C.5-C).

### 10.2 SUPERSEDED (preservar a história)
- **SUPERSEDED**: "125 HeadCodes top-level + 240 aninhados" (0C.5-A, heurística de indentação `^\tcase `).
- **CONFIRMED**: 156 top-level (+~240 aninhados).
- **Motivo**: indentação inconsistente (2-3 tabs) e #ifdefs escondiam 28 cases; veredito por profundidade real de chaves (string-aware) em 0C.5-B-audit + parser de depth do 0C.5-C; triplo cross-check 157.
- Outro SUPERSEDED da fase 0C.4: **BOTH_MESSAGE 0x000B → 0x000C** (corrigido em 0C.4-E pela leitura do enum).

### 10.3 Mapa (famílias top-level + handlers dos 28 descobertos)
Famílias [MEM 0C.5-A]: login F1/F3/F4 (subcode; F1=34 sub-cases, login-Value=24, F3=23) · chat 0x00-0x03 · move/combate **0xD4(PACKET_MOVE)/0x15(PACKET_POSITION)/0x11(PACKET_ATTACK)** + 0x18/0x19/0x69/0x1A/0x1E/0x1B/0x07/0xBA/0x4A/0x4B(Monk #ifdef) · viewport 0x12/0x13/0x1F/0x45/0x14/0x25/0x16/0x9C/0x17/0x2D · itens 0x20/0x21/0x23/0x2A/0x26/0x27/0x28 · NPC/trade 0x30/0x32/0x33/0x36 · 0x1C Teleport (FALSE→"Strange packet") · party 0x40-0x43 · guild 0x50-0x56/0x5D/0x60-0x64/0x65/0x66/0xE1/0xE5 · union 0x67/0xE9 · storage 0x81-0x83 · Chaos 0x86/0x87 · eventos 0x90-0x93/0x9A/0x99/0xB2-0xB5 · social 0xC0-C8/0xCA/0xCB · quests/custom 0xA4/0xE6-EA · misc 0x9D/0x9E · sub-dispatch switch/if legítimos: EB/BC/8E/F6/F7/F8/F9/AA/3F/B2/BD/D1/BF.

Os 28 descobertos em 0C.5-B-audit (case@linha → handler):
```
0x2C@13211 ReceiveUseStateItem       0x68@13425 ReceivePreviewPort        0xAF@13734 ReceiveMoveToEventMatchResult2
0x34@13230 ReceiveRepair             0x94@13479 ReceiveEventChipInfomation 0xB1@13745 (sub-if, siege)
0x44@13286 ReceivePartyInfo          0x95@13482 ReceiveEventChip          0xB6@13889 ReceiveGuildMemberLocation
0x46@13289 ReceiveSetAttribute       0x96@13485 ReceiveMutoNumber         0xB7@13895 (sub-if, guild-siege)
0x47@13292 ReceivePartyGetItem       0x9B@13476 ReceiveMatchGameCommand   0xB8@13929 (sub-if, guild-siege)
0x4A@14305 ReceiveStraightAttack*    0x9F@13497 ReceiveEventCount         0xB9@13953 (sub-if, guild-siege)
0x4B@14308 ReceiveDarkside*          0xA0@13500 ReceiveQuestHistory       0xBB@13892 ReceiveGuildNpcLocation
0xA7@13610 ReceivePetCommand        0xD2@14242 (sub-switch, InGameShop*) 0xDE@14220 (if, equipping-item*)
0xA8@13613 ReceivePetAttack         0xEE@13381 (if, CustomArena)         0xEF@13387 (if, CustomArena)
0xA9@13616 ReceivePetInfo           (* = dentro de #ifdef)
```
Cases sem handler direto (parser): `<none>` FALSO em 0x19/0x69/0x1E/0x16/0x9C/0x30/0x36 (corpos `if (!ReceiveX(...))` — handlers: ReceiveMagic/ReceiveMonsterSkill/ReceiveMagicContinue/ReceiveDieExp/ReceiveDieExpLarge/ReceiveTalk/ReceiveTrade, mapeamento 1:1 exato [PENDING]); `switch` como handler nas famílias sub-dispatch.
Comportamento default: `break;` → função **sempre retorna TRUE** (desconhecido ignorado sem log).
Tabela completa 157 rows: existiu em `/tmp/new_era_0C5C_top_level_headcodes.md` e `/tmp/new_era_0C5B_opcode_map.md` — **efêmeros; [PENDING — regeneráveis por re-run dos scripts]**.

### 10.4 Servidor (mensurado em 0C.6-A-2)
Case labels iniciais do switch(head) @98: `0x00@100, 0x02@103, 0x03@106, 0x0E@109, PROTOCOL_CODE2@112(=0x11), PROTOCOL_CODE3@115(=0x15), 0x18@118, 0x19@121, 0x1B@124, 0x1C@127, 0x1E@130, 0x22@133 …` — **contagem total de cases do switch(head) NÃO medida (PENDING)**.

--------------------------------------------------------------------------------

## 11. SERVER DISPATCH (síntese @580472e)

```
CLIENTE ──55901──┬─ [NEW=1] SocketManagerModern.cpp (ASIO acceptor :37)
                 │    ├─ msgs tipadas → CG*Recv (:105-129; BOTH_* → CGPosition/CGMove/CGAttack/
                 │    │   CGMultiSkillAttack/CGCharacterList/CGCharacterInfo[síntese F3/03])
                 │    └─ BOTH_MESSAGE(0x000C) :132 → recv[8024] :134 (SEM guard) → head=recv[2|3]
                 │        → gPacketManager.AddData(guard 0<size<2048)/ExtractPacket(C1/C2-only+XOR)
                 │        → ProtocolCore(head,DecBuff,size,aIndex,0,0) :171
                 └─ [clássico] SocketManager.cpp (WSASocket :141-159; IOCP)
                      └─ OnRecv :571 → DataRecv :255 (defesas :257-295; multi-frame; C3/C4
                         SimpleModulus Decrypt :301/:335 → C1/C2 sintético → AddData/ExtractPacket)
                         → QUEUE_INFO → semáforo → ServerQueueThread :866 (HIGH priority)
                         → ProtocolCore :882 (com encrypt+serial reais)
                                   ▼
              ProtocolCore(head,…) Protocol.cpp:76 → switch(head):98 → handlers (sub-switches lpMsg[3])
```
- Envio: game-code → `DataSend` (Util.cpp) → **gateway :214-218** → NEW=1 → `PacketSend(BOTH_MESSAGE)` (guard >8192 log+drop) → ASIO; caminho clássico (serial+SimpleModulus Encrypt :445-471 + guard+Disconnect :474) **bypassado neste build**.
- Familiares inter-server (cada elo com ProtocolCore próprio, padrão fila+drain `SocketManager.cpp:708` em CS/JS/DS): GS→CS `CSProtocol.cpp:81` · GS→DS `DSProtocol.cpp:58` · GS→JS `JSProtocol.cpp:15` · GS→ExDB `ESProtocol.cpp:89` · cliente→CS `ConnectServerProtocol.cpp:10` · `Connection.cpp:158 wsProtocolCore` (papel UNCERTAIN).
- Criptografia efetiva no build: **transporte MORTA (2×)** · **SimpleModulus ativa só no clássico** (recv clássico; envio clássico bypassado) · **XOR 32 B ativa nos dois caminhos** (sentido c→s apenas).

--------------------------------------------------------------------------------

## 12. RISCOS ARQUITETURAIS (do legado — registro, não correção)

### CONFIRMED RISK
| Risco | Evidência | Impacto potencial | Microteste |
|---|---|---|---|
| BOTH_MESSAGE → recv[8024] sem validação (cliente, stack) | 0C.2/0C.4-E [MEM] | stack overflow | feito (evidência) |
| BOTH_MESSAGE → recv[8024] sem validação (servidor, static) | SMM :136-138 (0C.6-F) | OOB 8025-65535; **loop infinito >65535** (wrap u16); resíduo se 0 | feito |
| header.size u32 sem teto → resize (cliente e servidor) | ProtocolAsio.h; olcPGEX_Network.h :549-553 (0C.4-E/0C.6-E) | alocação gigante/DoS | feito |
| Assimetria defensiva entre caminhos do mesmo servidor | 0C.6-F vs 0C.6-I (clássico tem default-reject+clamp+disconnect; moderno silencioso) | superfície de ataque concentrada na ponte moderna | feito |
| Fila ASIO: release() sem reset = leak; sem retry/reconnect | 0C.4-G/H | vazamento de memória; recuperação manual | feito |
| Wire little-endian sem hton/ntoh (ambos os lados) | 0C.4-E/0C.6-D | portabilidade | feito |
| Buffers static compartilhados em paths de rede (DecBuff, recv) | SMM :134/:150; SM :265/:439/:876 | estado residual entre mensagens/conexões | registrado |

### PROBABLE RISK
| Risco | Evidência | Status |
|---|---|---|
| DoS por alocação gigante (header.size) | consequência direta dos CONFIRMED acima (sem try/catch evidenciado no caminho) | PROBABLE (auditoria 0C.6-E) |
| Casts de confiança `msg.body.data() → PMSG_*` (handlers tipados) | SMM :105-120 (0C.6-G) — body curto lê além do recebido (in-bounds do vector, fora do semântico) | PROBABLE |

### HYPOTHESIS / PENDING
- Topologia de threads do io_context (1 loop infinito trava quanto?) — PENDING.
- `MAX_SIDE_PACKET_SIZE` valor — PENDING.

--------------------------------------------------------------------------------

## 13. DESCOBERTAS SUPERSEDED (história intelectual — NÃO APAGAR)

### 13.1 Contagem de HeadCodes top-level (cliente)
- **Antiga**: 125 top-level (+240 aninhados). **Evidência antiga**: heurística de indentação `^\tcase ` (0C.5-A).
- **Nova**: **156 top-level + default = 157 labels**. **Evidência nova**: script 0C.5-B (157 entries), veredito brace-depth string-aware (28 suspeitos todos depth 1), script 0C.5-C (157), triplo cross-check.
- **Motivo**: indentação inconsistente (2-3 tabs) e #ifdefs escondiam cases; grep por indentação não é prova de escopo C++.

### 13.2 ID do BOTH_MESSAGE
- **Antiga**: 0x000B. **Nova**: **0x000C**. **Motivo**: leitura direta do enum `ProtocolHead` (ProtocolSend.h:7-26; membro 12) em 0C.4-E; reconfirmada no servidor (SocketManagerEnum.h, 0C.6-B).

### 13.3 "Head não-inicializado chega ao ProtocolCore" (ponte moderna)
- **Antiga** (0C.6-G achado C): parse sem else deixaria `head` não-inicializado fluindo. **Nova** (0C.6-H): **neutralizado** — `AddData(ptr, size=0)` retorna 0 → return antes do ProtocolCore. **Motivo**: guard `(0<size<2048)` do PacketManager não era conhecido quando do 0C.6-G.

### 13.4 "Possível assimetria do XOR servidor→cliente"
- **Antiga** (0C.6-K): incerteza — PacketSend não aplica XOR; onde o cliente decodificaria? **Nova** (0C.4-I): XOR de frame é **unidirecional por design** (cliente→servidor); não há decode no cliente porque não há encode no servidor. **Motivo**: censo de tokens no WSclient.cpp (0 XorData/ExtractPacket; BuxConvert é de campo).

### 13.5 (menor) Parser do script 0C.5-B
- VALUE_PARSE/contagens parciais do script tinham artefatos (depth com src[i-] pós-incremento; `<none>` em corpos `if (!…)`). Registrados como limitação de heurística, não como verdade substituída.

--------------------------------------------------------------------------------

## 14. HIPÓTESES AINDA NÃO CONFIRMADAS

| # | Hipótese | Origem | Evidência existente | Falta | Microteste possível | Status |
|---|---|---|---|---|---|---|
| H1 | O filtro do builder (CStreamPacketEngine) é o inverso exato do `m_XorFilter[32]` servidor | 0C.3 + 0C.6-H | XOR cascata nos dois; símbolos distintos | comparação byte-a-byte dos 32 valores + direção da cascata | dump dos 32 bytes + algoritmo do builder (wsclientinline.h/StreamPacketEngine*) | UNCERTAIN |
| H2 | `Connection.cpp`/`wsProtocolCheck` é caminho distinto do SocketManager clássico (dupla camada?) | 0C.6-A | Connection.cpp:158 chama wsProtocolCheck próprio | papel/uso real | microteste sobre Connection.cpp | UNCERTAIN |
| H3 | Qual camada (clássica vs moderna) efetivamente atende 55901 em runtime | 0C.6-A/K | ambas compiladas; NEW=1 no gateway de envio; ambas chamam ProtocolCore | aceitação simultânea? portas separadas? | leitura de StartServer/ListenServer/ports | UNCERTAIN |
| H4 | `PROTOCOL_CODE4`=0xDB tem uso real (cliente ou servidor) | 0C.6-C | definido; ausente dos 156 cases do cliente | grep de uso no servidor/cliente | microteste grep 0xDB/CODE4 | UNCERTAIN |
| H5 | `BOTH_ATTACK3` chega ao servidor via BOTH_MESSAGE/legado | 0C.6-G-snip | membro enum sem case tipado | tráfego real/fonte de envio | análise de uso (deferred) | UNCERTAIN |
| H6 | VM_START/VM_END já foram efetivamente protegidos por Themida em builds distribuídos | 0C.6-O | SDK integrado; marcadores no-op sem protector | binários compilados (fora do fonte) | análise de binário (fora do escopo fonte) | UNCERTAIN |
| H7 | `CalcFPS` bug de unidade afeta exibição apenas | 0B [FILE] | relatório 0B §CalcFPS | — | — | CONFIRMED (display apenas) — mantida por completude |

--------------------------------------------------------------------------------

## 15. PENDÊNCIAS (identificadas, NÃO investigadas — não virar conclusão)

1. **Contagem de cases do `switch(head)` do ProtocolCore servidor** (iniciada em 0C.6-A-2, nunca completada).
2. `PacketManager.cpp` além dos 3 métodos (Init/seeding de `m_XorFilter[32]`; quem define o filtro; conteúdo de LoadKey/Formato Enc1/Dec2 server-side).
3. `Connection.cpp` / `wsProtocolCheck` (H2) e camada ativa em runtime (H3).
4. `MAX_SIDE_PACKET_SIZE` (valor), `gSerialCheck` (janela/ressincronização), `QUEUE_INFO.encrypt` no ramo plain C1/C2.
5. `PROTOCOL_CODE4` (0xDB) — uso real (H4).
6. Deferred clássicos [MEM]: ChatRoom destino · GameShop URLs · caminho real de `MapServerInfo.cpp/.h` · fluxos SERVIDOR restantes (envio CS/JS/DS detalhado) · compressão (mencionada como tema deferido) · criptografia profunda (parcialmente coberta por 0C.6-I..O — o que resta: chaves/arquivos Enc/Dec do SERVIDOR, ver #2).
7. Roadmap 0D-0L inteiro (ver §16).
8. Auditoria formal do coordenador para 0C.6-F..O e 0C.4-I (executados/entregues; livro-mestre visível parava em 0C.6-E).

--------------------------------------------------------------------------------

## 16. ROADMAP GLOBAL [MEM + livro-mestre do coordenador]

| Fase | Tema | Status |
|---|---|---|
| FASE 0 | Pesquisa/Auditoria READ-ONLY | EM ANDAMENTO |
| 0A | Repository/Architecture Inventory | **APROVADO** |
| 0B | Game Loop / FPS / Timing | **APROVADO** |
| 0C | Network/Protocol | EM ANDAMENTO — 0C.1-0C.5 APROVADOS; 0C.6-A..E APROVADOS; 0C.6-F..O + 0C.4-I executados/entregues (auditoria pendente) |
| 0D | Renderer | NÃO INICIADO |
| 0E | Assets/BMD/Animation | NÃO INICIADO |
| 0F | Gameplay/Combat/Skills | NÃO INICIADO |
| 0G | Items/Data/Chaos | NÃO INICIADO |
| 0H | Lua/Scripting | NÃO INICIADO |
| 0I | Server Architecture | NÃO INICIADO |
| 0J | Diagnostics/Observability | NÃO INICIADO |
| 0K | Cross-platform/Mobile | NÃO INICIADO |
| 0L | Legacy Compatibility | NÃO INICIADO |
| — | Licenciamento/reuse boundaries | NÃO INICIADO |

--------------------------------------------------------------------------------

## 17. ESTADO ATUAL (momento da criação do Ledger)

- **FASE 0 — PESQUISA/AUDITORIA · READ-ONLY · Implementação NEW-ERA NÃO INICIADA.**
- A instrução de criação deste Ledger indicava "último microteste conhecido: 0C.6-H; próximo: 0C.6-I". **O histórico real da sessão demonstra evolução posterior** [origem: histórico direto do chat desta sessão]: 0C.6-I, 0C.6-I-pack, 0C.6-J, 0C.6-K, 0C.6-L, 0C.6-M, 0C.6-M-pack, 0C.6-N, 0C.6-O, 0C.6-O-pack e **0C.4-I (último executado)** — todos entregues em chat com evidência e classificação (ver §4/§5).
- Arc 0C.6-I→O (criptografia/transporte/licença do GameServer) fechado **sem incertezas internas**; 0C.4-I resolveu a incerteza de simetria XOR.
- Última aprovação formal visível do coordenador: **0C.6-E** (mensagem de auditoria + livro-mestre consolidado que planejava 0C.6-F como próximo).

--------------------------------------------------------------------------------

## 18. LACUNAS DE RECUPERAÇÃO (obrigatório — não preenchidas por inferência)

- [PENDING] Comandos/scripts originais verbatim das fases 0C.1, 0C.2, 0C.3, 0C.4-A..H, 0C.5-A (pré-compactação do histórico) — não recuperáveis; conclusões/âncoras numéricas preservadas via [MEM] e reconfirmadas indiretamente por runs posteriores (ex.: 0C.4-I reconfirmou âncoras do ProtocolCompiler; 0C.6-* reconfirmaram framing).
- [PENDING] Snippets originais (texto integral de trechos) de 0C.1-0C.5-A — idem.
- [PENDING] Mapeamento 1:1 exato opcode→handler para os 7 cases `<none>` (0x19/0x69/0x1E/0x16/0x9C/0x30/0x36) — lista de handlers conhecida, pareamento exato não recuperado.
- [PENDING] Tabelas completas 157 rows (0C.5-B/0C.5-C) — viviam em `/tmp` (efêmero); regeneráveis por re-run dos scripts do usuário (scripts preservados nesta sessão apenas de forma efêmera).
- [PENDING] `NEW_ERA_MASTER_CHECKPOINT.md` / `NEW_ERA_CURRENT_STATE.md` — não presentes no workspace deste Agent (versão mais recente vista: bloco citado na mensagem de auditoria 0C.6-E, chat).
- [PENDING] Outputs integrais dos runs (arquivos /tmp/*.txt) — efêmeros por natureza; trechos materiais estão transcritos nas entregas de chat e resumidos aqui.
- [NOT RECOVERED] Resultado completo (todas as linhas) do run 1 do 0C.6-A antes do microteste (mantido em /tmp/0C6A_run1.txt na época; conteúdo essencial transcrito no chat).

--------------------------------------------------------------------------------

## 19. INTEGRIDADE DO LEDGER

- Este documento é uma **reconstrução histórica** da trajetória de pesquisa.
- Ele **não substitui** os arquivos-fonte (repo `wongddd/muonline` @ `580472e`).
- Ele **não substitui** evidências RAW (fetches pinados por commit).
- Ele **não autoriza implementação** nenhuma.
- Informações não recuperadas devem **permanecer marcadas** ([PENDING]/[NOT RECOVERED]).
- Conclusões futuras devem ser **adicionadas**, nunca apagar o histórico.
- Conclusões **SUPERSEDED devem permanecer registradas** (§13).
- Proveniência marcada por [FILE]/[MEM]/[CHAT]/[RAW] em todo o documento.

--------------------------------------------------------------------------------

## 20. NEXT ACTION (registrada, NÃO executada)

- Nenhum microteste estava formalmente enfileirado pelo coordenador no momento da criação (o plano visível parava em 0C.6-F, já executado e superado).
- Candidatos naturais apontados pelas evidências (a agendar PELO COORDENADOR, respeitando phase-gating): (1) contagem canônica de cases do `switch(head)` do ProtocolCore servidor (espelho do 0C.5-C); (2) microteste H1 (comparação dos 32 bytes do filtro XOR builder↔servidor); (3) H2/H3 (Connection.cpp + camada ativa em runtime); (4) auditoria formal de 0C.6-F..O/0C.4-I no livro-mestre.
- **Nada disso foi executado nesta operação.**

================================================================================
FIM DO LEDGER — documento vivo; adicionar, nunca apagar.
================================================================================

## 21. REGISTRO DE CONTINUIDADE (governança) — adicionado por instrução do coordenador
- 2026-09-03: Protocolo de assunção COORDENADOR/GUARDIÃO emitido no chat; diagnóstico
  de continuidade produzido SEM execução de pesquisa e SEM alteração de arquivos
  [origem: CHAT — não evidência de código].
- Verificação de arquivo (find recursivo, mesma data): NEW_ERA_CURRENT_STATE.md e
  NEW_ERA_MASTER_CHECKPOINT.md AUSENTES do workspace (confirmado em arquivo pela
  ausência + §0 :26/§18 deste Ledger); nenhum artefato *0C*/*0D*/*0E* adicional.
- Reconstruções dos 2 documentos canônicos geradas EM RESPOSTA (não gravadas),
  baseadas apenas nos 3 arquivos NEW-ERA do workspace.

### 21.1 CHAT-ONLY (NÃO RECUPERÁVEL NO WORKSPACE) — NÃO-CANÔNICO
> Os itens abaixo foram executados/entregues no chat APÓS a criação deste Ledger.
> NÃO há evidência em arquivo no workspace (scripts e outputs viveram em /tmp,
> efêmero e fora do workspace). NÃO tratar como CONFIRMED enquanto o coordenador
> não auditar e a evidência não for persistida. Classificação: [CHAT-ONLY —
> NÃO-CANÔNICO].

- 0C.4-J (+EVID pack) — alegado: filtro XOR builder cliente ≡ decoder servidor
  (32/32 bytes idênticos); legado comentado no PacketManager (downgrade S8→S5);
  resolveria H1 (§14). STATUS: NÃO-CANÔNICO.
- 0D.1-A, -A2, -B, -C, -D (2 envios, 1º truncado — SyntaxError documentado), -D-full,
  -E, -F, -G — alegados: renderer híbrido (immediate mode + GLSL 2.0 via
  wglGetProcAddress em ZzzShader.cpp com fontes embutidas; uso terreno/utils;
  fallback não-fatal; MSAA 4x→2x com recriação de contexto; ferramentas órfãs
  Save/Save2/DestroyBuiltinShaders sem call-sites). STATUS: NÃO-CANÔNICO.
- 0E.1-A..I — alegados: formato .bmd decifrado (magic "BMD", memcpy cru); pipeline
  SMD-texto (SMD.cpp→SMD2BMD→Models[]); espinha dorsal binária AccessModel→Open2
  (793 usos); extensão .bmd dual (modelos × tabelas-texto Data\Local); família
  anti-tamper *MemoryDump com rand()%1024; MAX_MODELS sem definição nos 948
  arquivos de /source. STATUS: NÃO-CANÔNICO.
- Eventos de ambiente (chat-only): truncamento de scripts do usuário ×3; outage E2B.
- AÇÃO REQUERIDA: coordenador audita → classifica → autoriza persistência das
  evidências (ex.: relatórios 0D/0E em /home/user/) → então converter em entradas
  canônicas nos termos do §19 (adicionar, nunca apagar).

### 21.2 Entradas novas com evidência em arquivo
- NENHUMA além deste bloco de governança: o inventário recursivo confirma que os
  únicos arquivos NEW-ERA do workspace já estão indexados (§0 :24-26). Não há
  matéria-prima nova em arquivo para entrar no Ledger.

## 22. 0C.7-A-NET — Censo switch(head) ProtocolCore (GameServer) (COM REDE RESTRITA)
- Data: 2026-09-03 (execução) · 2026-09-04 (consolidação/persistência, por comando do coordenador).
- Rede ultra-restrita: URL ÚNICA `https://raw.githubusercontent.com/wongddd/muonline/580472e/Source%20Server/GameServer/GameServer/Protocol.cpp` (commit `580472e`) — HTTP 200 · **135.014 B** · SHA256 `e8bb4dbb28d1baf23222dbc72762e2ebd55b116dbb8d36e5f017dd744e510f13` (re-download de consolidação byte-idêntico).
- **Resultado**: `void ProtocolCore(BYTE head,BYTE* lpMsg,int size,int aIndex,int encrypt,int serial)` :76-1165; `switch(head)` top-level **:98-1164**; **107 case labels top-level, 107 distintos, 0 duplicados, SEM default** (depth-1 scan 98-1164 = 0 labels); 1 case vazio (0x8A @353); sem fallthrough top-level; 155 labels aninhados em sub-switches `lpMsg[3]` EXCLUÍDOS; 171 linhas #if/#endif (GAMESERVER_UPDATE 202-803) com **nenhum label condicional** (conjunto de labels independe do build; corpos variam). Método: brace-depth string/comment-aware (mesmo da 0C.5-B-audit), re-derivação independente na cópia persistida reproduziu 107/107/155/default=[].
- **Achado (H4 → CONFIRMED)**: `case PROTOCOL_CODE4:` **@:771** → `gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)lpMsg,aIndex,0);` @:772 (CODE4=0xDB no build LANG=1; ausente dos 156 cases do cliente — assimetria legada).
- **ERRATA (SUPERSEDED, sem editar o passado)**: a nota da §5/0C.6-A-2 — "vizinho `case ProtocolHead::BOTH_POSITION:`@110" — está INCORRETA: :110 é o CORPO do case 0x0E (`CGLiveClientRecv(...)`); `ProtocolHead::` não ocorre como case no arquivo (apenas em PacketSend :1441/:1454/:1938/:1951/:3119). A lista da §10.4 (sem @110) permanece correta. Regra §13 respeitada: histórico preservado, correção registrada aqui.
- **Fecha a Pendência §15.1** (contagem canônica do switch(head) do ProtocolCore servidor — iniciada em 0C.6-A-2, nunca completada). PENDÊNCIA DERIVADA (não investigada): censo dos 155 labels de sub-switches `lpMsg[3]` (candidato 0C.7-B).
- Artefatos persistidos (evidência em ARQUIVO — nova fronteira documental):
  - `NEW_ERA_PHASE_0C7A_GS_PROTOCOLCORE_SWITCH_CENSUS.md` (relatório do censo, lista completa 107)
  - `NEW_ERA_EVIDENCE/GS_Protocol.cpp__580472e__sha256_e8bb4dbb.cpp` (cópia da fonte, sha conferido) + `.SHA256.txt`
- Proveniência: [RAW] (fetch pinado) + [CHAT] (runs 0C.7-A / 0C.7-A-CHAT / 0C.7-A-NET e consolidação).

## 23. 0C.7-B — Censo sub-switches `switch(lpMsg[3])` (ProtocolCore GS)
- Data: 2026-09-04 · SEM REDE (fonte: evidência persistida em 0C.7-A-NET — `NEW_ERA_EVIDENCE/GS_Protocol.cpp__580472e__sha256_e8bb4dbb.cpp`, SHA256 `e8bb4dbb28d1baf23222dbc72762e2ebd55b116dbb8d36e5f017dd744e510f13` conferido no início do run; 135.014 B / 5.217 linhas).
- **Resultado**: **26 sub-switches `switch(lpMsg[3])`** dentro do switch(head) :98-1164 (todos a profundidade relativa 1; **nenhum outro predicado de switch na função**; zero aninhamentos ≥3) · **155 labels `case` somados** (= exatamente os 155 aninhados excluídos no 0C.7-A-NET — **divergência ZERO** vs "~155") · 155 distintos por sub-switch (sem duplicatas internas) · **54 sub-opcodes distintos globais** · **21 repetidos em >1 HEAD** (espaço de sub-opcode é local por família; `0x01` ocorre em 21 HEADs) · **1 único `default:`** — HEAD **0xBF** @:656 `LogAdd(LOG_RED,"[0xBF] Unknown packet BF:%d",lpMsg[3]); break;` (LOG; único ponto defensivo de todo o dispatch — o top-level é silencioso).
- Maiores: **0xF3=22** (login/personagens) · 0xB2=16 (Castle Siege) · 0xF8=14 · 0xBF=11; menores: 0xB1/0xEB/0xF7=1. Corpos condicionais (`GAMESERVER_UPDATE`) em 10 sub-switches; labels incondicionais.
- Método: mesmo parser brace-depth string/comment-aware de 0C.7-A-NET, com rastreamento de HEAD pai (último label depth-1) e de profundidade de labels (depth-2 do sub-switch).
- **Fecha a pendência derivada da §22** (censo dos 155 labels). Pendência NOVA derivada (não investigada): pareamento fino sub-opcode↔handler vs cliente (ex.: F3 servidor 22 vs F3 cliente 23 — §10.3 [MEM]; diferença não reconciliada).
- Artefato: `NEW_ERA_PHASE_0C7B_GS_PROTOCOLCORE_LPMSG3_SUBSWITCH_CENSUS.md` (mapa completo HEAD→subcases com linhas + repetições globais + trechos). Proveniência: [FILE-evidência] + [CHAT].

## 24. 0E.1-J — Definição/Origem de MAX_MODELS (client/assets)
- Data: 2026-09-04 · SEM REDE · somente workspace local (comando do coordenador com consolidação automática).
- **Classificação: (4) INCONCLUSIVO NO WORKSPACE (faltam arquivos)** — `MAX_MODELS` tem **ZERO ocorrências** em todo o repo local (uso ou definição; grep -rw, todos os tipos de arquivo), incluindo `Main.vcxproj` (ClCompile Debug :93 / Release :157 / ResourceCompile :180 vazia — **não provém do build do cliente**), `.filters/.user/.sln`, response file `CUsersAdministratortmpcl.rsp` e `_TextureIndex.h` da raiz. Padrões de definição cobertos: #define / enum / const / constexpr / static const / template / -D (**enum foi lacuna do scan chat-only 0E.1-I — aqui coberta, 0 hits locais**).
- Faltam exatamente: raiz de `Source Client/source/` (local: VAZIA — apenas 6 subdirs/345 arquivos; ausência verificada de Winmain.cpp, ZzzBMD.cpp/.h, ZzzOpenData.cpp, ZzzScene.cpp/.h, SMD.cpp) + lado GitHub nunca varrido (raiz de `Source Client/` e `sdk-directx9/`; e re-varredura enum-pattern dos 948 de /source) — tudo exige rede.
- Evidência indireta local: nenhum `Models[` acessível; únicos matches MAX*MODEL* = `GL_MAX_MODELVIEW_STACK_DEPTH` (GL.h :527 / glew.h :580 / glad.h :552 — falso-positivo OpenGL).
- **Achados laterais (fato, evidência em arquivo)**: (a) `Source Client/_TextureIndex.h` na RAIZ local = 823 L / 23.257 B ≠ GitHub `source/_TextureIndex.h` 18.779 B [0E.1-A, CHAT-ONLY] — **divergência workspace↔commit 580472e** (nova evidência do §3-A2); (b) `CUsersAdministratortmpcl.rsp` (132 B, 1º char U+F03A) = artefato `cl` da máquina dos autores (build ad-hoc de `C:\Users\Administrator\tmp\debugger.cpp`) commitado na raiz; (c) define de Release **`LDS_PATCH_GLOBAL_100520`** (Main.vcxproj :157) — flag de patch cliente não catalogada.
- Fechamento parcial: hipóteses (a) macro-define e (b) build-define **refutadas no escopo local**; (c) enum/const em arquivos ausentes e origem fora de /source permanecem ABERTAS → candidato natural 0E.1-J-NET (com rede: varrer raiz de `Source Client/` + enum-rx nos 948).
- Artefato: `NEW_ERA_PHASE_0E1J_MAX_MODELS_DEFINITION_HUNT.md`. Proveniência: [FILE-local] + contexto [CHAT-ONLY §21.1].

## 25. 0E.1-J-NET — MAX_MODELS via GitHub Code Search + raw@580472e (REDE RESTRITA)
- Data: 2026-09-04 · rede ultra-restrita (1 chamada search + raw@580472e SOMENTE para paths do search — regra do comando).
- **Resultado: classificação (4) INCONCLUSIVO mesmo com rede — BLOQUEADO na etapa A**: `GET api.github.com/search/code?q=MAX_MODELS+repo:wongddd/muonline&per_page=25` → **HTTP 401 "Requires authentication"** (Code Search da API exige token; nenhum token fornecido/autorizado; nenhuma credencial não-autorizada usada). 0 paths retornados → **0 downloads** (regra "somente paths do search" cumprida estritamente; limite 25 não atingido).
- Evidência principal: `NEW_ERA_EVIDENCE/MAX_MODELS__search_code_api__response_401.json` (120 B; corpo `{"message":"Requires authentication",…,"status":"401"}`) + `.SHA256.txt`.
- Estado do conhecimento sobre MAX_MODELS inalterado (ver §24): 4 usuários no GitHub @580472e [CHAT-ONLY §21.1]; não é build-define; enum-rx pendente no lado GitHub.
- Caminhos de fechamento (PENDENTES de novo comando; NÃO executados): (1) fornecer GITHUB_TOKEN e re-run; (2) autorizar trees API + raw dos candidatos (raiz `Source Client/`, `sdk-directx9/`, enum-rx nos 948); (3) autorizar download direto dos 4 paths-usuários conhecidos + headers adjacentes (~6-10 arquivos, menor custo).
- Artefato: `NEW_ERA_PHASE_0E1JN_MAX_MODELS_NET_SEARCH_AND_EVIDENCE.md`. Proveniência: [RAW-search-401] + [CHAT].
- Governança: correções neste Ledger somente via append/errata nova — evitar edits in-place (reinforced por comando do coordenador).

## 26. 0E.1-JN2 — MAX_MODELS via Contents API (ref=580472e) + raw@580472e (sem token)
- Data: 2026-09-04 · rede ultra-restrita (1 Contents API + 25 raw@580472e; sem search/code, sem token).
- **Contents API: HTTP 200, 613 itens (607 files + 6 dirs)** de `Source Client/source` @580472e. Seleção determinística (regex nome `max|model|bmd|smd|open|data|zzz|define|global|std|textureindex`, ordem lexical): 63 candidatos → **25 baixados (25/25 HTTP 200, ~1,16 MB), 38 truncados** (incl. ZzzOpenData.cpp/.h, stdafx.h, _define.h — ver §26 limite). Evidências: `NEW_ERA_EVIDENCE/0E1JN2__*__580472e.*` + 25 `.SHA256.txt`.
- **Classificação: (4) AINDA INCONCLUSIVO para definição/valor — com progresso substantivo ([FILE-FACT] @580472e):**
  1. `MAX_MODELS` ocorre UMA única vez no conjunto, como **comentário histórico**: `ZzzBMD.cpp:22 //BMD Models[MAX_MODELS];` — e a definição ATUAL do storage é **ponteiro dinâmico**: `ZzzBMD.cpp:23 BMD *Models;` (+`:24 BMD *ModelsDump;`) com externs ativos `ZzzBMD.h:334-335` (`//extern BMD Models[];` :333 comentado). Evidência: `NEW_ERA_EVIDENCE/0E1JN2__20__ZzzBMD__...__580472e.cpp` sha256 `0083f7fbb062747a589fb7bd47ee127665b384f90c3f0e07a08ff24f053dfa55` e `0E1JN2__21__ZzzBMD__...__580472e.h` sha256 `44d0d6693ebb4a5fc652b1ca5b1f8d0c208d2785a457fc1ce47064dc4016a0d0`.
  2. **Nenhuma definição** (`#define/enum/const/constexpr`) nos 25; ZzzBMD.h define os limites-irmãos `MAX_BONES 200`(:6)/`MAX_MESH 50`(:7)/`MAX_VERTICES 15000`(:8)/`MAX_MONSTER_SOUND 10`(:35) mas NÃO MAX_MODELS; Define.h (91 B) só `MAX_ID_SIZE 10`; Defined_Global.h (940 B) só feature-flags.
  3. **Site de alocação FORA do conjunto** (`new BMD`/`Models =`: 0 hits nos 25) → o limite real é imposto onde `BMD *Models` é alocado. [CHAT-ONLY §21.1] aponta `ZzzOpenData.cpp:110-121` (`new BMD[MAX_MODELS+1024]` + `rand()%1024`) — **não verificado em arquivo** (ZzzOpenData.cpp casou a regex mas caiu na truncagem >25). Usos ativos do ponteiro indexado no conjunto: LoadData.cpp:34/:36/:48 (hub AccessModel :22-43 c/ FatalError p/ Player/Monster/Angel) · SMD.cpp:217-234 (`Models[ID].Version=10` :222) · SMD2BMD.cpp:289/:381 · ZzzAI.cpp:446 · ZzzCharacter.cpp:379+:448+ (`Models[MODEL_PLAYER]`; enum MODEL_PLAYER também fora do conjunto).
- **Faltam exatamente 5-6 arquivos para fechar**: `ZzzOpenData.cpp` · `ZzzOpenData.h` · `stdafx.h` · `_define.h` (+ `Winmain.cpp`, fora-da-regex) → sugerido **0E.1-JN3** (mesma mecânica, lista explícita, ≤6 raw).
- Artefato: `NEW_ERA_PHASE_0E1JN2_MAX_MODELS_CONTENTS_RAW_AUDIT.md`. Proveniência: [RAW-Contents+raw@580472e, evidência persistida] + [CHAT-ONLY §21.1].

## 27. 0E.1-JN3 — Models allocation site (ZzzOpenData) + MAX_MODELS (raw@580472e, rede restrita)
- Data: 2026-09-04 · rede = SOMENTE 7 URLs raw@580472e (lista fechada do comando; sem API/search/HTML/git). **7/7 HTTP 200** (~845 KB), evidências `NEW_ERA_EVIDENCE/0E1JN3__<1..7>__*__580472e` + 7 `.SHA256.txt` (principais: ZzzOpenData.cpp 269.112 B sha256 `26504dd7d9d5428217751b89553237651a0ad315db52703431556b58527ccc61`; Winmain.cpp 45.584 B `8e77ee5c1f0b993946cfa25ebfd534b353aeee9c5e070bc1446f65354c428e5d`; _define.h 18.981 B `c2fa25d74def49ff12b76b5918e3108bb14e99b04a8bf47c77eefb84a3f7204f`).
- **Classificação: (2) MAX_MODELS EXISTE como símbolo ativo — 3 usos EM ARQUIVO (0 comentados) — mas o VALOR não é literal fechado nos 7 (definição em header fora da lista; candidato #1 `_enum.h`, include stdafx.h:85; build-define refutado §24):**
  1. **Site de alocação [FILE-FACT]** `ZzzOpenData.cpp:110-113 void OpenPlayers()`: `:111 ModelsDump = new BMD [MAX_MODELS+1024];` · `:112 Models = ModelsDump + ( rand() % 1024);` (anti-tamper: base deslocada 0..1023) · `:113 ZeroMemory( Models, MAX_MODELS * sizeof ( BMD));` — **MAX_MODELS É o limite real** (classe (3) refutada); confirma [CHAT-ONLY §21.1] e adiciona ZeroMemory :113.
  2. **Teardown [FILE-FACT]** `Winmain.cpp:447-449` `for(int i=MODEL_LOGO;i<MAX_MODELS;i++) Models[i].Release();` + `:459 SAFE_DELETE_ARRAY( ModelsDump );` (irmãs :456-458: CharacterMemoryDump/ItemAttRibuteMemoryDump/RendomMemoryDump — padrão *Dump anti-tamper). Boot chain: `OpenBasicData` ZzzOpenData.cpp**:5394-5396** → `OpenPlayers()`.
  3. **Definição AUSENTE nos 7** (incl. _define.h: 415 #defines, família MODEL_* encadeada por expressões — `MAX_MODEL_MONSTER 400` :110, `MODEL_MONSTER01 = MODEL_SKILL_END+1` :112, `MODEL_MONSTER_END = ...` :113, `MODEL_BODY_NUM 24/20` :116/:118 e `MAX_CLASS 7/6` :379/:381 condicionais a `PBG_ADD_NEWCHAR_MONK` — mas SEM MAX_MODELS/MODEL_PLAYER/MODEL_LOGO) → lar provável `_enum.h` (presente no repo, listing JN2; stdafx.h:85). **Falta 1 arquivo para fechar o valor → sugerido 0E.1-JN4 (1 download: _enum.h).**
- Limitações: lista fechada sem _enum.h/_types.h/_struct.h/Winmain.h; [CHAT §21.1] dizia "4 usuários" — em arquivo são 3 (ZzzOpenData.cpp ×2, Winmain.cpp ×1) + 1 comentário (ZzzBMD.cpp:22, §26); bounds-check explícito de índice de model NÃO existe nos 7 (risco §12); nota metodológica: 1º grep de token quase reportou falso-negativo por glob casando só sidecars — detectado e refeito (resultado §27 é o correto).
- Artefato: `NEW_ERA_PHASE_0E1JN3_MODELS_ALLOCATION_AND_MAX_MODELS_AUDIT.md`. Proveniência: [RAW raw@580472e, evidência persistida] + [CHAT-ONLY §21.1 confirmado em arquivo].
- Governança: Ledger só por append; correções via errata nova.

## 28. 0E.1-JN4 — Valor/definição de MAX_MODELS em _enum.h (raw@580472e, rede restrita)
- Data: 2026-09-04 · rede = 1 URL raw@580472e (lista fechada). **HTTP 200, 56.491 B**, sha256 `7a3eba988f02b19178aee240ad2730615f851def3e83f6c8e0bfb85d3e35ad90` → `NEW_ERA_EVIDENCE/0E1JN4__enum_h__580472e.h`(+.SHA256). **Série 0E.1-J ENCERRADA.**
- **Classificação: (1) MAX_MODELS DEFINIDO em _enum.h com VALOR RESOLVIDO = 10002** (ressalvas abaixo). Definição: `_enum.h:1046` — `MAX_MODELS = (MODEL_MASK_HELM+MAX_ITEM_INDEX),` membro final do 2º enum anônimo da região object (:885-:1047; enum#1 :321-:883).
- **Cadeia (resolvedor textual: macros _define.h + contagem sequencial + #ifdef pela clausura baixada; 612 membros, ZERO símbolos não-resolvidos)**: MAX_ITEM_INDEX=512 (_define.h:342) · MAX_QUEST_ITEM=64 (:352) · MAX_EVENT_ITEM=35 (:353) · MODEL_BODY_NUM=20 (:118, MONK off) · MODEL_ITEM_COMMON_NUM=2 (:126) · MAX_ITEM=8192=16×512 (:339/:344) · MAX_MODEL_MONSTER=400 (:110) · MODEL_ITEM=1086 (implícito :1001; MODEL_PLAYER=1078 :991; MODEL_SKILL_END=564 :882; MODEL_NPC_BEGIN=965 :889) → MODEL_HELM2=9278 (:1020) → MODEL_BODY_HELM=9288 (:1033, #else MONK_ITEM off) → MODEL_EVENT=9388 (:1040) → MODEL_QUEST=9423 (:1041) → MODEL_MASK_HELM=9490 (:1045) → **MAX_MODELS=10002 (:1046)**.
- **Validações**: âncoras duras 6/6 OK (0/130/140/150/160/160 — :324/:403/:408/:415/:422/:424); lattice 16×512 íntegro (MODEL_SWORD=1086 … MODEL_ETC=+7680; MODEL_HELM2=+8192 = logo após o bloco); spot-check linha-a-linha fiel (:424-:448, :985-:1012); **geometria da alocação fecha com §27: `new BMD[10002+1024]=11026`, janela Models[0..10001] em offset ≤1023 → índice máx 11024 ≤ 11025 — encaixe exato (os +1024 são o orçamento exato do anti-tamper)**.
- **Ressalvas**: (a) comentário autoral `:1001 // (515)` é **STALE** — 515 < mínimo aritmético possível (1086 com todos os gates OFF; ≤1150 com todos ON); (b) flags não definidos na clausura baixada (Winmain.h não baixado) só podem ELEVAR: faixa [10002, ~10103] (variante all-ON parcial = 10082); (c) parser assume ramos #else de MAX_CLASS/MODEL_BODY_NUM (correto p/ config evidenciada).
- Correlatos resolvidos: MODEL_LOGO=160 (:424), MODEL_PLAYER=1078 (:991), MODEL_MONSTER_END=965, MODEL_ITEM=1086.
- Artefato: `NEW_ERA_PHASE_0E1JN4_MAX_MODELS_VALUE_FROM_ENUM_H.md`. Proveniência: [RAW raw@580472e + derivação mecânica documentada; evidências JN2/JN3 reutilizadas].
- Governança: Ledger só por append; correções via errata nova.

## 29. 0E.1-K — Censo de indexadores de Models[] e bounds-check (offline)
- Data: 2026-09-04 · SEM REDE · análise textual sobre repo local (1225 arquivos .cpp/.h — **ZERO hits**: camada de modelos ausente §24/§26) + 35 evidências @580472e (JN2/JN3/JN4) · 1309 registros únicos (dedupe arquivo+linha+padrão).
- **INDEXADORES = 1293** (500 `Models[…]` diretos + 793 chamadas `AccessModel` em ZzzOpenData). **CHECK=SIM: 3 · CHECK=NÃO: 1286 · CHECK=INDETERMINADO: 4.**
- **3 achados principais**: (1) **wrapper `CLoadData::AccessModel` (LoadData.cpp:22-43) SEM QUALQUER bounds-check** — `Models[Type]` :34/:36 sem guard — e é o canal de 793 call-sites (constantes em sua quase-totalidade, ~8 com var real: `nIndex`, `c`, `k`, `_nRollIndex`); (2) **~272 sites com índice dinâmico SEM guard**: 173 por campo de objeto (`o->Type`×58+`o->Owner->Type`×61+`o->Type`(Effect)×37+`pObject/pOwner/pSource/pTarget->Type`+`c->Object.Type`×5) + 91 por parâmetro/base+parâmetro (`Type`×43, `MODEL_MONSTER01+Type`×38 ZzzOpenData:3175+, `ID`×7 SMD/SMD2BMD, `_Model_NpcIndex`×2) — incluem **ESCRITAS**: ZzzCharacter.cpp:12948-12952 (`Models[c->Object.Type].Meshs[n]…=true`), SMD.cpp:221 (`strcpy(Models[ID].Name,…)`), ZzzEffect.cpp:6128-6129; origem final de `Type` FORA do escopo (ZzzObject.cpp/wsclient.cpp ausentes) → classe IV = HIPÓTESE; (3) **WINMAIN.CPP:447 É O ÚNICO guard de MAX_MODELS de todo o cliente** (`for(i=MODEL_LOGO;i<MAX_MODELS;i++)` teardown; os outros 2 SIM são loops por constantes do lattice: ZzzOpenData:1847/:2346).
- Design anti-tamper (§27) × ausência de check: 0 índices negativos e 0 `Models±n` no escopo (região à esquerda do ponteiro nunca endereçada); MAS índice ≥10002 sem guard cai na folga não-zeroed `[Models+10002, ModelsDump+11026)` → acesso silencioso; ≥11026+offset → heap corruption — o offset randômico NÃO mitiga OOB positivo.
- Limitações: 35/603 arquivos de /source; 2 loops indeterminados (:2362; :4934-4951); `OpenBMD` = protótipo órfão (0 chamadas no escopo).
- Artefato: `NEW_ERA_PHASE_0E1K_MODELS_INDEXERS_BOUNDS_AUDIT.md`. Próximo sugerido: 0E.1-L (origem de `OBJECT::Type` — rede restrita 2-3 raw: ZzzObject.cpp + wsclient.cpp).
- Governança: correções do Ledger só via append.

## 30. 0E.1-L — Origem e sanitização de OBJECT::Type (vetor para Models[] OOB)
- Data: 2026-09-04 · offline (35 evidências) + rede ultra-restrita (1 Contents API 200/613 itens + 23 raw@580472e, 23/23 OK, 1.288.497 B, truncagem 0; 6 mandatórios inexistentes no dir; 3 já em evidência). Evidências `NEW_ERA_EVIDENCE/0E1L__01..23__*`(+.SHA256); chaves: WSclient.cpp 415.884 B `6538d944126181dfa77d4419bf992ac77151eec5411dc3ab243f2926fec868bf` · ZzzObject.cpp 378.840 B `2974f5bb…` · wsclientinline.h 62.684 B `67a6d535…`.
- **Classificação: (3) OBJECT::Type NÃO recebe bytes de rede diretamente — sempre enums internos** via `CreateCharacterPointer` ZzzCharacter.cpp:11182→`:11219 o->Type = Type` (callers todos `MODEL_*`), **com nuance (2)**: o seletor de rede SEM validação é traduzido por **default-substitution**.
- **Cadeia R [FILE-FACT]**: handlers **0x13** (WSclient:2585/:2597 máscara `0x03`→Type≤1023; dispatch :13106) · **0x1F** (:2714/:2723 16-bit SEM máscara; :13110) · **0x45** (:2382/:2425 idem; :13114) · PreviewPort case2/3 (:11226) → `CreateMonster(Type,…)`; `CreateMonster` (ZzzCharacter:12496-14355) = switch(Type) com casos LITERAIS e **ÚNICO `default:` :13386 = `OpenMonsterModel(0)`+`CreateCharacter(Key,MODEL_MONSTER01,…)` (Type desconhecido → modelo 0 — substituição estrutural, não crash)**; `Setting_Monster` :12396 armazena `c->MonsterIndex = Type` cru :12422 (**nunca usado como índice** — só comparações :8530+) e classifica Kind por faixas :12426-12450.
- **OpenMonsterModel** (ZzzOpenData:2356): header SEM guard `Index=MODEL_MONSTER01+Type` :2360 · `&Models[Index]` :2362 · `AccessModel(Index,…)` :2366 — porém **0 callers variáveis em 58 evidências** (todos literais/`71+randType`; `Models[MODEL_MONSTER01+Type]` :3175-3340 dentro de switch com cases literais → constante por case).
- **Única injeção R em campo indexável**: equipamento — `ChangeCharacterExt` ZzzCharacter:12005 (chamado c/ `Equipment` de pacotes :571/:1941/:2320/:2489/:11217): `Type=Equipment[0]` :12016 · `ExtType` 12-bit :12019-20 · **sentinela `0x0FFF→-1`** :12022 · `c->Weapon[0].Type = MODEL_SWORD+ExtType` :12034 → máx 5180 < 10002 (**in-bounds por bit-width, sem check explícito**); irmãs Wing/Helper/:11741-11807 (`p[EQUIPMENT_*].Type+MODEL_ITEM`) não traçadas linha-a-linha.
- **Ponte K (§29)**: o risco "rede→OOB em Models[]" NÃO tem vetor ativo confirmado no escopo (seletor neutralizado por default-substitution; equipamento in-bounds por construção); permanece a fragilidade estrutural: 0 guards em 1293 indexadores — qualquer path novo que injete Type cru vira OOB silencioso/heap-corruption.
- Atribuições catalogadas: 14 grupos/~45 sites; sanitização EXPLÍCITA vs MAX_MODELS = **0**; estrutural = todos os caminhos R. `struct OBJECT` def fora do escopo regex (_struct.h/_types.h existem no listing) → tipo C exato [PENDING].
- Artefato: `NEW_ERA_PHASE_0E1L_OBJECT_TYPE_ORIGIN_AND_SANITIZATION.md`. Próximo sugerido: 0E.1-M (mapa recebimento de itens → p[EQUIPMENT_*].Type, última origem R indireta).
- Governança: correções do Ledger só via append.

## WORKSPACE HYGIENE PLAN — manifesto e plano gerados (sem deleções)
- 2026-09-04 (comando do coordenador) · motivo: **alerta de budget** — workspace medido em 135.125.516 B (128,87 MiB) / 4.374 arquivos, no teto de snapshot (~128 MB best-effort).
- Gerados: `NEW_ERA_EVIDENCE_MANIFEST.md` (inventário completo: 116 arquivos/2,91 MiB em NEW_ERA_EVIDENCE/, 0 duplicados por SHA256, 114/116 referenciados, TOP-30 do workspace) e `NEW_ERA_WORKSPACE_CLEANUP_PLAN.md` (diagnóstico: driver = muonline/ 125,57 MiB READ-ONLY protegido; recuperável dentro da governança = 0 B; SAFE-DELETE SET = VAZIO).
- **NENHUM arquivo foi removido/movido/compactado; deleção requer comando separado e aprovação explícita.** (Seção fora da numeração §NN por determinação do comando; numeração normal retoma em §31.)

## WORKSPACE HYGIENE APPLIED — prune mínimo de artefatos IDE/build
- 2026-09-04 (comando PARTE 2/2 do coordenador; exceção explícita e pontual ao READ-ONLY de muonline/).
- **Removidos: 31 arquivos, 2.196.585 B (2,09 MiB)** — padrões EXATOS `*.suo` (3: GameServer.suo 750.592, DataServer.suo 94.720, JoinServer.suo 50.176) · `*.aps` (4: DataServer 302.052, JoinServer 301.104, GetMainInfo 167.416, ConnectServer 20.392) · `*.tlog` (24: cryptopp/CTRelease + cryptoppVs2019/CTRelease + dlltest___Win32_Release; lista integral pré-deleção registrada no chat da Parte 1/2). Nenhum diretório removido.
- **Preservados deliberadamente** (fora dos padrões autorizados): `CUsersAdministratortmpcl.rsp` (evidência §24), 5× `.vcxproj.user`, DLLs duplicadas (msvcr100/msvcp100 ×5), `Main.exe.original`, `client_screen.png`.
- Pós: workspace **132.961.951 B (126,80 MiB) / 4.345 arquivos** (muonline/ 129.475.864 B / 4.209). **Nenhum arquivo NEW_ERA_EVIDENCE/ foi alterado.**

## 31. 0E.1-M — Rede→Item/Equip→Model ID mapping (offline, evidências)
- Data: 2026-09-04 · SEM REDE · 58 evidências @580472e (WSclient.cpp/ZzzCharacter.cpp/ZzzObject.cpp/_define.h/_enum.h; hashes no relatório). 40 handlers item/equip localizados; 8 caminhos rede→model-id mapeados (P1-P8).
- **Classificação: (3) INCONCLUSIVO por falta de 1 arquivo — a def de `ConvertItemType`** (6 call-sites em WSclient/ZzzObject; declaração em header não baixado). **Provado in-bounds**: P2 `Weapon[1].Type=MODEL_SWORD+ExtType` (ExtType 12-bit de Equipment[12]&240<<4|Equipment[1], sentinela 0x0FFF, ZzzCharacter:12036-12063) e P3 Wing/Helper (máscaras 1-3 bit: (Equipment[4]>>2)&3, [8]&0x07, [4]&3, [9]&1, [15]&0xE0 → ≤7997). **Condicionais a ConvertItemType ≤13-bit**: P1 `ReceiveChangePlayer` **0x25** (WSclient:1951/:1959+ `MODEL_ITEM+Type`, sentinela 0x1FFF por case) · **P4 `CreateItem` ZzzObject:6110-6145 `o->Type = MODEL_SWORD+Type` SEM sentinela — itens no chão (0x20) alimentam DIRETAMENTE os ~95 indexadores `o->Type` do §29** · P5 `SetCharacterClass` :11741+ (`p[EQUIPMENT_*].Type+MODEL_ITEM`).
- **Evidência estrutural de 13-bit (forte, indireta)**: sentinela 0x1FFF (=8191=16×512−1) em 3 receivers + lattice desenhado `MODEL_HELM2 = MODEL_ITEM+8192` (_enum.h:1020) = bloco de itens exato ⇒ se Confirmado, P1/P4 ≤ 9277 < 10002 ⇒ classificação vira (1).
- **Sanitização EXPLÍCITA (rara!)**: P7 `ReceiveCreateItemViewport` :5644 `if(Key<0||Key>=MAX_ITEMS) Key=0` (clamp de Key de Items[]) · P6 `ReceiveGetItem` :5703 `Result>=MAX_EQUIPMENT_INDEX && Result<MAX_MY_INVENTORY_INDEX` — ambas em arrays de inventário, NÃO em Models[].
- Opcodes: 0x25 change player · 0x20 create item · 0x22 get · 0x23 drop · 0x32/0x33 buy/sell · 0x39 trade; ReceiveEquipment/ModifyItem e PersonalShop em sub-switches internas (head pai não isolado — PENDING menor).
- Artefato: `NEW_ERA_PHASE_0E1M_ITEM_EQUIP_MODEL_MAPPING_AUDIT.md`. Próximo sugerido: 0E.1-N (1-2 raw: def de ConvertItemType → fecha P1/P4/P5).
- Governança: correções do Ledger só via append.

## 32. 0E.1-N — Definição e range de ConvertItemType (rede restrita opcional)
- Data: 2026-09-04 · offline (58 evidências: 5 calls, 0 defs) + 1 Contents API (200/613) + 10 raw@580472e (10/10 OK, ~117 KB, parar-cedo não disparou; regex convert|item|equip|inventory|weapon|shop|trade|drop|pick → 39 candidatos, **29 TRUNCADOS pelo cap**) — evidências `NEW_ERA_EVIDENCE/0E1N__01..10__*`(+.SHA256).
- **Classificação: (3) INCONCLUSIVO — a def de `ConvertItemType` não está em nenhum dos 68 arquivos de evidência** (0 ocorrências do token nos 10 baixados: CSItemOption.cpp/.h, ItemAddOptioninfo, ItemInfo, ItemManager, NewUIInventoryCtrl). Range INDETERMINADO — sem corpo, qualquer bound seria invenção.
- Suspeitos #1 (entre os 29 truncados, registrados no relatório): **ZzzInventory.cpp (333 KB)/.h** · NewUIMyInventory.cpp (74 KB); ou arquivo fora-da-regex.
- Impacto: P1/P4/P5 do §31 permanecem CONDICIONAIS (vetor R→Models[] aberto por 1 elo); evidência estrutural 13-bit (0x1FFF) mantida, porém sem reforço. Risco ao §29 inalterado.
- Fechamento barato: 1 comando ≤10 raw priorizados (ZzzInventory.cpp/.h, NewUIMyInventory.cpp/.h, NewUIItemMng.cpp, PersonalShopTitleImp.cpp, NewUITrade.cpp, NewUINPCShop.cpp, NewUIMixInventory.cpp) — NÃO executado agora.
- Artefato: `NEW_ERA_PHASE_0E1N_CONVERTITEMTYPE_DEFINITION_AND_RANGE.md`. Governança: correções só via append.

## 33. 0E.1-N2 — Encontrar definição/range de ConvertItemType (priorizado; rede restrita)
- Data: 2026-09-04 · offline revalidado + 1 Contents API (200/613) + 10 raw@580472e (10/10 OK, ~399 KB; PRIORIDADE-1: ZzzInventory.cpp/.h baixados #1/#2 — ZzzItem/Inventory.* inexistentes; P2: NewUIItem*/LuckyItem, dedupe dos 10 do 0E.1-N). Evidências `NEW_ERA_EVIDENCE/0E1N2__01..10__*`(+.SHA256); ZzzInventory.cpp 333.218 B.
- **Classificação: (3) INCONCLUSIVO — NOT FOUND**: única ocorrência do token nos 10 = **CALL** (`ZzzInventory.cpp:7646 int Type = ConvertItemType(Item);`). Acumulado: **6 calls / 0 defs / 0 protótipos em 78 arquivos**.
- **Inferência nova (includes comuns dos 3 TUs portadores WSclient/ZzzObject/ZzzInventory)**: protótipo/def deve estar em headers compartilhados ainda ausentes — **`UIManager.h` (incluído pelos 3)** · **`ZzzInfomation.h/.cpp`** (grafia "Infomation" não casa nenhuma regex JN2/N/N2 — lacuna sistemática de seleção por nome!) · ZzzInterface.h · DSPlaySound.h · GOBoid.h · CSQuest.h · GMHellas.h.
- Impacto §31: P1/P4/P5 CONDICIONAIS (range indeterminado; anti-invenção); risco §29 inalterado. Fechamento: 0E.1-N3 com 4-6 raw fechados (UIManager.h, ZzzInfomation.h/.cpp, ZzzInterface.h, DSPlaySound.h) — NÃO executado.
- Artefato: `NEW_ERA_PHASE_0E1N2_CONVERTITEMTYPE_FIND_DEFINITION.md`. Governança: correções só via append.

## 34. 0E.1-N3 — ConvertItemType: varredura em UIManager/ZzzInfomation/ZzzInterface/DSPlaySound (rede restrita)
- Data: 2026-09-04 · offline baseline (6 calls) + 1 Contents API (200/613) + **3/6 raw@580472e com parar-cedo**: UIManager.h (2.439 B) · ZzzInfomation.h (3.778 B) · **ZzzInfomation.cpp (118.917 B, sha256 `d4dd353569254d18931e5961da62b3df35be9f86e7464c12eb6e2cd4493b72c8`)** — ZzzInterface.h/DSPlaySound.h/UIManager.cpp existiam mas NÃO foram baixados (stop). Evidências `NEW_ERA_EVIDENCE/0E1N3__1..3__*`(+.SHA256).
- **Classificação: (1) DEFINIÇÃO ENCONTRADA e RANGE PROVADO — o elo P1/P4/P5 do §31 está FECHADO.**
  1. Protótipo: `ZzzInfomation.h:29 extern int ConvertItemType(BYTE *Item);` · Definição: `ZzzInfomation.cpp:371-377`: `ret = Item[0]+(Item[3]&128)*2+(Item[5]&240)*32;` — empacotamento de 13 bits (Item[0]=bits 0-7; Item[3] bit7=bit 8; Item[5] nibble alto=bits 9-12).
  2. **RANGE PROVADO [0, 8191]** (máx 255+256+7680 = 8191 = 0x1FFF — bate EXATAMENTE com a sentinela 0x1FFF dos receivers §31 e com o lattice: bloco de itens [MODEL_ITEM..MODEL_HELM2−1]=[1086..9277]).
  3. Impacto: **P1 `MODEL_ITEM+Type` e P4 `MODEL_SWORD+Type` ∈ [1086..9277] < 10002 — PROVADO in-bounds** (folga 725); P5 idem. **Vetor rede→Models[] (aberto desde §30) ENCERRADO: todos os caminhos R conhecidos são estruturalmente in-bounds ou neutralizados** (monstros: default-substitution §30; equip: máscaras §31; itens: 13-bit provado aqui). §29 = fragilidade latente (0 guards), SEM vetor ativo de rede.
- Artefato: `NEW_ERA_PHASE_0E1N3_CONVERTITEMTYPE_HEADER_CHAIN_AUDIT.md`. Governança: correções só via append.

## 35. 0C.8-A — Reconciliação F3 (sub-opcodes) cliente↔servidor (offline)
- Data: 2026-09-04 · SEM REDE · fontes: GS_Protocol.cpp (sha e8bb4dbb…) + WSclient.cpp (sha 6538d944…) + WSclient.h (structs). **Fecha a pendência derivada da §23 (F3 22×23).**
- **Contagens: SERVIDOR 22 subcases (:955 → switch(lpMsg[3]) :956-:1034, sem default; handlers CG\*) × CLIENTE 23 subcases (:12942 → subcode via structs PHEADER_DEFAULT_SUBCODE(_WORD) → switch(subcode) :12958-:13038, sem default; handlers Receive\*).** Mapeamento de bytes provado: PBMSG_HEADER 3 B ⇒ SubCode=[3] (C1, = lpMsg[3] do servidor); PWMSG_HEADER 4 B ⇒ SubCode=[4] (C2; correspondência server-side p/ C2: PENDING menor).
- **Reconciliação: COMUNS 7 (00 01 02 03 06 30 52 — pares request↔response com nomes casando, ex.: CGCharacterListRecv↔ReceiveCharacterList; CGMasterSkillRecv↔Receive_Master_LevelGetSkill) · SOMENTE-SERVIDOR 15 (12 15 16 21 31 E6-EA EB EC ED F0 F1 — requests CG\* sem resposta de mesmo subcode; ex.: MoveViewportEnable, NameCheck/Change, SecurityBreach, OffTrade 0xEB, ItemBuyConfirm 0xED) · SOMENTE-CLIENTE 16 (04 05 07 08 10 11 13 14 20 22-25 40 50 51 — pushes S→C: Damage, PK, Inventory, MagicList, Equipment, SummonLife, WT/Soccer, ServerCommand, MasterLevel) · INCERTOS 0.**
- **Classificação: (1) MISMATCH RESOLVIDO — causa: 0xF3 é namespace DUPLEX (cada lado despacha a direção oposta: CG\* = C→S no servidor; Receive\* = S→C no cliente); a divergência 22×23 era coincidência numérica entre conjuntos direcionais distintos, não anomalia.**
- Artefato: `NEW_ERA_PHASE_0C8A_F3_CLIENT_SERVER_SUBOPCODE_RECONCILIATION.md`. Governança: correções só via append.

## 36. 0C.9-A — Head 0xF1: reconciliação + spec mínima cliente↔servidor (offline)
- Data: 2026-09-04 · SEM REDE · série 3 partes (A1 extração server / A2 extração client / A3 reconciliação, reproduzidas deterministicamente em evidência). Fontes: GS_Protocol.cpp (e8bb4dbb…) + WSclient.cpp (6538d944…) + WSclient.h (structs).
- **Contagens: SERVIDOR 3 subcases (case 0xF1 :942 → switch(lpMsg[3]) :943-:953; 0x01 CGConnectAccountRecv(PMSG_CONNECT_ACCOUNT_RECV*), 0x02 CGCloseClientRecv(PMSG_CLOSE_CLIENT_RECV*), 0x03 VAZIO — sem default) × CLIENTE 7 subcases (case 0xF1 :12825 = 1º case de TranslateProtocol :12821 → switch(Data->SubCode) :12828-:12939; 0x00 ReceiveJoinServer · 0x01 login inline + switch(Data->Value) ~20 códigos · 0x02 ReceiveLogOut · 0x12 create-account inline + Value (00/01/02) · 0x03/0x04 ReceiveConfirmPassword(2) · 0x05 ReceiveChangePassword — sem default).**
- **Prova estrutural**: `PHEADER_DEFAULT_SUBCODE {PBMSG_HEADER(3B); SubCode=[3]; Value=[4]}` (WSclient.h:129-134); sem branch C1/C2 (fase de conta = C1).
- **Reconciliação: COMUNS {01,02,03} · SOMENTE-SERVIDOR ∅ · SOMENTE-CLIENTE {00,04,05,12}.** Classificação: **(1) assimetria DIRECIONAL por design + namespace MULTI-PEER/FASE**: lado cliente = fase Connect/JoinServer (join/login/contas/senhas); lado GS = connect-account/close-client in-game; subcodes reutilizados entre peers (0x01 login-result × connect-account; 0x02 logout × close-client — par de ciclo de vida; **0x03 ConfirmPassword(cliente) × case VAZIO(GS)** = peers distintos, GS ignora). Spec mínima MVP registrada no relatório (enums de Value de 0x01/0x12 documentados).
- Artefato: `NEW_ERA_PHASE_0C9A_F1_CLIENT_SERVER_MIN_SPEC.md`. Governança: correções só via append.

## 37. 0C.9-B — Spec operacional 0xF1:0x01 (login result)
- Data: 2026-09-04 · SEM REDE · fontes: evidências GS_Protocol.cpp (e8bb4dbb…) + WSclient.cpp (6538d944…) + WSclient.h + repo READ-ONLY in place (JoinServerProtocol.cpp 531fb3d5…; ConnectServer: 0 ocorrências; GameServer local VAZIO de protocolo — só Time/lua).
- **CADEIA COMPLETA PROVADA: Cliente →(C1 0xF1:0x01, account/password via PacketArgumentDecrypt)→ GS CGConnectAccountRecv (:1960-:2006; version/serial→result 6 :1972/:1978) →GJConnectAccountSend (:2004)→ JS GJConnectAccountRecv (JoinServerProtocol.cpp:76-:254; result 0=senha :123/:154, 1=ok :86/:231, 2=conta inválida/inexistente/DB :88/:111/:140/:170/:178/:192, 3=já conectado :219-:224, 4=cheio :95-:104; +BlockCode :188, sno__numb :186, AccountLevel :200, Lock :206) → GS GCConnectAccountSend (:3127-:3142, header.set(0xF1,0x01) :3132; overload SOCKET :3144-:3155 :3149) → Cliente case 0x01 :12833 → switch(Value) :12835-:12906.** **Correspondência result↔Value 1:1 para 0→0x00, 1→0x01, 2→0x02, 3→0x03, 4→0x04, 6→0x06.**
- **Tabela canônica de Value: 23 labels/21 outcomes (0x20/0x01 sucesso `LogIn=2;CheckHack()`; 0x00 senha; 0x02 ID; 0x03 já-conectado; 0x04 cheio; 0x05 block; 0x06 versão+ErrorReport; 0x07=DEFAULT→FAIL_CONNECT :12866-:12867; 0x08 erro; 0x09 sem pagamento; 0x0a-0x0d tempos; 0x11 <15 anos; 0x40 canal pago; 0xc0/0xd0 point-date; 0xc1/0xd1 point-hour; 0xc2/0xd2 IP inválido).** Sem struct dedicada de login (só Header+SubCode+Value; WSclient.h:129-134).
- **Mini-spec MVP: response `[C1][Size≥5][0xF1][0x01][Value]` (bytes ≥5 ignorados); request ClientVersion[5]+ClientSerial+TickCount+account[10]+password[10] crypted — layout exato [NOT RECOVERED].** Nota: GS @580472e tem stub moderno NEW_PROTOCOL_SYSTEM==1 (`gSocketManagerModern`, ProtocolHead::BOTH_CONNECT_LOGIN :3138).
- **Limitações ([NOT RECOVERED]/HIPÓTESE)**: receiver JG→GS (mapeia BlockCode/sno__numb/AccountLevel→Values 5/0x11/0x40/0x0a-0x0d/0xc0-0xd2) fora do escopo (GameServer local vazio; exigiria rede); defs PMSG_CONNECT_ACCOUNT_*; sender do request no cliente (0 envios de 0xF1 no WSclient.cpp).
- Classificação: **(1) par request/response fechado para MVP** (framing + outcomes + condições provadas + fallback default). Artefato: `NEW_ERA_PHASE_0C9B_F1_01_LOGIN_RESULT_SPEC.md`. Governança: correções só via append.

## 38. 0C.9-C — Head 0xF1:0x00 Join handshake (S→C) + request 0xF1:0x01 (C→S) via SendRequestLogIn (offline)
- Data: 2026-09-04 · SEM REDE · fontes (sha sidecar): WSclient.cpp 6538d944… (415.884 B) + WSclient.h 19d38a3b… (91.012 B) + wsclientinline.h 67a6d535… (62.684 B) + GS_Protocol.cpp e8bb4dbb… (135.014 B).
- **(a) Layout 0xF1:0x00 S→C FECHADO (12 B)**: struct dedicada `PRECEIVE_JOIN_SERVER` (WSclient.h:270-279) `[C1][Size][F1][00][Result][NumberH][NumberL][Version[5]]`; handler `ReceiveJoinServer` (WSclient.cpp:392-460): LogIn!=0 → HeroKey+SendChangeMapServer (:396-:399, map-server move); LogIn==0 → Result 0x01=SUCCESS (:407-:410) / default=MESSAGE_SERVER_LOST (:429-:432); version check `Version[i]-(i+1)!=Data2->Version[i]` (:435-:443). **Emissor = GS** `GCConnectClientSend2` (:3067-:3092, set(0xF1,0x00) :3071, DataSend :3089) / `GCConnectClientSend` (:3094-:3125, set :3099); mapeamento result→Result (:3102), SET_NUMBERHB/LB(aIndex)→NumberH/L (:3104-:3106), m_ServerVersion→Version[5] (:3108-:3116). JS/CS locais: 0 senders (GS-only).
- **(b) Request 0xF1:0x01 C→S ENCONTRADO** — macro `SendRequestLogIn` (wsclientinline.h:243-:269): `Init(0xC1,0xF1)` :249 · `<<0x01` :250 · **BuxConvert** em ID/senha :257-:258 (contraparte do PacketArgumentDecrypt GS :1992/:1996 — H1) · AddData ID/senha :259-:260 · `<<GetTickCount()` :261 · **Version CODIFICADA `V[i]-(i+1)`** :262-:263 (mesma codificação esperada no check :437) · **Serial[16]** :264-:265 (SIZE_PROTOCOLSERIAL=16, H:14) · Send :266. Wire: `[C1][sz][F1][01][ID10][PASS10][Tick4][VerEnc5][Serial16]` ≈49 B [MAX_ID/PASS=10 inferido pelos buffers GS :1990-:1996; defs fora do escopo]. HIPÓTESE menor: m_ServerVersion GS armazena forma codificada (memcmp :1970 vs V-(i+1)).
- **(c) FECHA a lacuna “sender do request no cliente” do §37 (0C.9-B, limitação 3)** — request existe, é macro em wsclientinline.h, layout completo documentado (incl. Serial, invisível no corpo do GS).
- Classificação: **(1) spec 0xF1:0x00 + request 0xF1:0x01 fechados em arquivo**. Artefato: `NEW_ERA_PHASE_0C9C_F1_00_JOIN_AND_F1_01_LOGIN_REQUEST_SPEC.md`. Governança: correções só via append.

## 39. 0C.9-D — GS: decrypt/validação do request 0xF1:0x01 (CGConnectAccountRecv)
- Data: 2026-09-04 · SEM REDE · fontes: GS_Protocol.cpp (e8bb4dbb…) + WSclient.cpp (6538d944…) + wsclientinline.h (67a6d535…).
- **(a) Validção server-side FECHADA**: `CGConnectAccountRecv` :1960-:2006 — kick `Connected!=OBJECT_CONNECTED` :1964 (CloseClient, sem 0xF1); **result 6 = memcmp de ClientVersion :1970-:1974 OU ClientSerial :1976-:1980 → GCConnectAccountSend(aIndex,6)+return — ABORTA ANTES do JS**; guard anti-replay `LoginMessageSend==0` :1982 (1ª msg only); `ClientTickCount=lpMsg->TickCount` :1987; `PacketArgumentDecrypt(account/password, src, 10)` :1992/:1996 (dest[11]); `GJConnectAccountSend` :2004. Results 0-4 seguem só no JS (§37). Struct `PMSG_CONNECT_ACCOUNT_RECV` [NOT RECOVERED]; campos por acesso: ClientVersion[5]/ClientSerial[16]/TickCount/account[10]/password[10] — consistente com wire §38 ≈49 B.
- **(b) DECRYPT PROVADO (cliente)**: `BuxConvert` WSclient.cpp:209-:215 = **XOR cíclico 3 B, chave static {0xFC,0xCF,0xAB}** (`Buffer[i]^=bBuxCode[i%3]`), aplicado em ID/senha antes do AddData (inline:257-:258). `PacketArgumentDecrypt` no GS evidence: **3 chamadas** (:1790 `CGMapServerMoveAuthRecv` size 11; :1992/:1996 login size 10), **def [NOT RECOVERED]** — equivalência PacketArgumentDecrypt≡BuxConvert = HIPÓTESE FORTE (papel/tamanhos/buffers casam; servidor DESFAZ o XOR — dados viajam BuxConvert’ed). **Correção conceitual**: H1 (§21.1, chat-only “XOR 32B”) NÃO vale para credenciais — ofuscador provado = XOR 3B; H1 permanece NÃO-CANÔNICA, nenhum § do Ledger afirmava 32B neste fluxo.
- **(c)** HIPÓTESE §38 mantida (m_ServerVersion em forma codificada V[i]-(i+1); gServerInfo [NOT RECOVERED]). Receiver JG→GS **não** está no GS_Protocol.cpp (0 hits SDHP_*/JG*) — recuperá-lo exigiria novo comando de rede.
- Classificação: **(1) validação GS fechada com evidência**. Artefato: `NEW_ERA_PHASE_0C9D_GS_LOGIN_REQUEST_DECRYPT_VALIDATION.md`. Governança: correções só via append.

## 40. 0C.9-E — PacketArgumentDecrypt: recuperação da definição e equivalência com BuxConvert (offline)
- Data: 2026-09-04 · SEM REDE · escopo: muonline/ (~4,2k arq) + NEW_ERA_EVIDENCE/ (sem sidecars) · padrões: PacketArgumentDecrypt/ArgumentDecrypt/BuxConvert.
- **Conclusão: (3) NOT RECOVERED** — definição de `PacketArgumentDecrypt` NÃO existe no workspace: exatamente **3 ocorrências, todas CALL**, no GS_Protocol.cpp (e8bb4dbb…): :1790 (CGMapServerMoveAuthRecv, size 11), :1992/:1996 (login, size 10); protótipo 0, definição 0.
- **Corroboração INÉDITA da HIPÓTESE §39b**: **2ª definição independente de `BuxConvert` no workspace** — `muonline/Source Tools/Encoder/Encoder/Util.cpp:4-:12` (sha e0424697087e107a7b5e90570626fc4e6b0321d3c1d4b2d023cd1154dacec7f8; protótipo Util.h:3; uso Encoder.cpp:270): `static bBuxCode[3]={0xfc,0xcf,0xab}` + `pbyBuffer[i]^=bBuxCode[i%3]` — **algoritmo e chave IDÊNTICOS byte a byte ao cliente** (WSclient.cpp:209-:215) ⇒ BuxConvert é padrão de codebase (cliente + tool Encoder do server-pack). Equivalência PacketArgumentDecrypt≡BuxConvert permanece HIPÓTESE FORTE (não fato).
- Notas legadas NÃO-CANÔNICAS (repo, provenance duvidosa): `analysis_report.md:168` “BuxConvert(账号密码) → SimpleModulus Encrypt(8B→11B) → TCP”; `fix_log_v3.md:114` “BuxConvert XOR key {0xFC,0xCF,0xAB} 循环”; `fix_log_v3.md:116` “login packet 55 bytes” — **diverge do wire inferido ≈49 B (§38)**; registrado como flag, não evidência.
- **Errata ortográfica** (sem correção in-place, §39 intacto): §39 contém “Validção”.
- MVP: decrypt=BuxConvert é a decisão de menor risco (risco marcado); prova formal exige novo comando de rede (arquivo GS com a def). Artefato: `NEW_ERA_PHASE_0C9E_PACKETARGUMENTDECRYPT_RECOVERY.md`. Governança: correções só via append.

## 41. 0C.9-F — PacketArgumentDecrypt: recuperação via Contents API (GS) + equivalência com BuxConvert
- Data: 2026-09-04 · REDE AUTORIZADA ultra-restrita: 1 Contents API (HTTP 200; 517 entradas/507 códigos em `Source Server/GameServer/GameServer` @580472e) + **1/8 raw downloads (early-stop em FOUND-DEF — 1º da ordem: Util.cpp)**; candidatos: 7 prioridades (todas existiam) + CrywolfUtil.cpp (regex).
- **FOUND-DEFINIÇÃO: `PacketArgumentDecrypt` GS Util.cpp:101-:109** — `void PacketArgumentDecrypt(char* out_buff,char* in_buff,int size)` com `BYTE XorTable[3]={0xFC,0xCF,0xAB}` :103 e `out_buff[n]=in_buff[n]^XorTable[n%3]` :107. Evidência EM ARQUIVO: `NEW_ERA_EVIDENCE/0C9F__Util__path_Source_Server_GameServer_GameServer_Util.cpp__580472e.cpp` (11.245 B, sha256 6271b7befc4a905ec3a1b7a66163502971c79981557b859f546cb08f3d53d395, +.SHA256.txt).
- **EQUIVALÊNCIA COM BuxConvert CONFIRMADA (3 definições independentes, algoritmo+chave idênticos byte a byte)**: cliente WSclient.cpp:209-:215 (static) · tool Encoder/Util.cpp:4-:12 (static) · GS Util.cpp:101-:109 (XorTable local). **Hipóteses §39b/§40 PROMOVIDAS A PROVADO** (via presente, correção de status por append). XOR involutivo: cliente codifica, GS decodifica.
- **MVP login 0xF1:0x01: camada de ofuscação de credenciais 100% provada** (ID/PASS ^{FC,CF,AB} cíclico no wire). Demais lacunas inalteradas ([NOT RECOVERED]: PMSG_CONNECT_ACCOUNT_RECV, receiver JG→GS, gServerInfo).
- Errata menor: sha inicialmente transcrito com erro no relatório 0C9F, corrigido antes desta consolidação (nota no próprio relatório).
- Classificação: **(1) FOUND + EQUIVALÊNCIA CONFIRMADA**. Artefato: `NEW_ERA_PHASE_0C9F_PACKETARGUMENTDECRYPT_NET_RECOVERY.md`. Governança: correções só via append.

## 42. 0C.10-A — Pipeline C3/C4 + SimpleModulus (cliente+GS)
- Data: 2026-09-04 · offline-first + REDE AUTORIZADA (1 Contents HTTP 200 + **4/6 raw**; SimpleModulus.cpp/.h NÃO existem no GS dir; candidatos: PacketManager.cpp/.h + HackPacketCheck.cpp/.h — todos com tokens, persistidos como `0C10A__*` + sidecars).
- **CLIENTE (provado)**: sends **NUNCA** usam Init(0xC3/C4) (193×C1/1×C2/0×C3/0×C4) — C3/C4 é montado pelo wrapper `SendPacket` (inline.h:67-:127): bEncrypt→`g_SimpleModulusCS.Encrypt` :100 + header **C3** :106 (<256B) / **C4** :118, serial `g_byPacketSerialSend++` :94, padding rand :90. Chaves incondicionais no boot (`Data\Enc1.dat`/`Dec2.dat`, Winmain:1459-:1460). RX: C3/C4→`g_SimpleModulusSC.Decrypt` WSclient.cpp:11686/:11692, serial check :11703, falha→`SendHackingChecked(0x06,0)` :11698.
- **GS (provado)**: `case 0xC3/0xC4` GS_Protocol.cpp:670/:673 = **falsos positivos** (namespace friend). Motor real = **`CPacketManager`** (PacketManager.cpp novo, sha 3e82e5d2…): Encrypt :214/Decrypt :268/Blocks :319/:369, aritmética `((Key·d)%Modulus)^Xor` :337/:403, XorFilter[32] :88-:116, chaves em arquivos ENCDEC (ofuscadas :191; análogo Enc1/Dec2), camada Crypto++ DES (DES_XEX3) se GAMESERVER_UPDATE≥701 (.h:9-:13). **Gate compile-time `#if(ENCRYPT_STATE==1) EncryptData`** em DataSendSocket (Util.cpp:240-:244; valor no build [NOT RECOVERED]). **Validação per-packet**: HackPacketCheck script `Index/Value/Encrypt/MaxDelay/MinCount` :71-:79, mismatch de Encrypt → rejeita+log :141-:143.
- **MVP login 0xF1**: request 0x01 `spe.Send(TRUE)` (inline:266) → C3 presumido [INFERÊNCIA — StreamPacketEngine.h fora do escopo]; cliente aceita C1/C3/C4 no RX; response 0x00 via DataSend (crypto do DataSend [NOT RECOVERED]).
- Classificação: **(1) TRANSPORTE C3/C4 ATIVO E NECESSÁRIO, com ressalvas [NOT RECOVERED]: ENCRYPT_STATE@build, dispatch C3/C4 no recv GS (arquivo fora da seleção), semântica exata de spe.Send(TRUE), crypto do DataSend**. Artefato: `NEW_ERA_PHASE_0C10A_C3C4_SIMPLEMODULUS_PIPELINE.md`. Governança: correções só via append.

## 43. 0C.10-B — Fechamento: RECV C3/C4 no GS + ENCRYPT_STATE + prova do C3 no login
- Data: 2026-09-04 · offline (cliente) + REDE AUTORIZADA (1 Contents HTTP 200 + 6/6 raw; early-stop no stdafx.h; 2 sem tokens descartados — SocketManager.h/SocketManagerModern.h não persistidos). Novos: `0C10B__SocketManager…cpp` (21.153 B, sha 8492b66a…), `0C10B__SocketManagerModern…cpp` (4.859 B, 82f0629c…), `0C10B__stdafx…h` (2.590 B, d23c35c3…), `0C10B__GameServer…vcxproj` (30.262 B, 3a3a057d…).
- **CLIENTE (provado, sem inferência de caminho)**: `SendRequestLogIn` → `spe.Send(TRUE)` (inline:266, 1 arg ⇒ bForceC4=FALSE) → `CStreamPacketEngine::Send` (StreamPacketEngine.h:120-:123 — def JÁ em evidência 0E1L__10, sha 797d26d9…) → `SendPacket` (inline.h:100-:112): bEncrypt → SimpleModulusCS.Encrypt → **C3 se cifrado <256 B e !bForceC4** :102/:106 ⇒ **login ≈49 B sai como C3** (premissa única: tamanho 49 B §38, inferido).
- **GS RECV (provado)**: SocketManager.cpp — parse C1/C3 size@1 :271, C2/C4 size@2-3 :277; **C3: gPacketManager.Decrypt :301** + AddData(serial) :311 + ExtractPacket :311; C4 :335-:346; plain :371; **entrega: ProtocolCore(head,buff,size,index,encrypt,serial) :882** (worker de queue). **S→C DataSend :417-:470**: C3/C4 do caller → re-encripta com serial por conexão (gSerialCheck :449/:462 — par do g_byPacketSerialRecv cliente WSclient.cpp:11703); C1 segue plain.
- **ENCRYPT_STATE FECHADO = 0**: stdafx.h:58-59 `#ifndef ENCRYPT_STATE / #define ENCRYPT_STATE 0`; NÃO sobrescrito no GameServer.vcxproj ⇒ gates OFF (DecryptData recv :590-:594; EncryptData DataSendSocket Util.cpp:240-:244; DataSend :482) — transport-whitening desativado; **cripto ativa = CPacketManager (C3/C4)**.
- **BÔNUS estrutural (refina §38)**: `XorData` do stream do cliente (StreamPacketEngine.h:66-:80): **XOR encadeado** `buf[i]^=buf[i-1]^filter[i%32]` por campo (AddData bXor=TRUE :82-:94), tabela 32 B **32/32 bytes idêntica** ao `m_XorFilter` do CPacketManager (PacketManager.cpp:85-:116: E7 6D 3A 89 … E8 56). Ordem física do login request: BuxConvert(ID/PASS) → XOR-encadeado → SimpleModulus → C3. Layout lógico §38 inalterado.
- Classificação: **(1) PIPELINE FECHADO + ENCRYPT_STATE FECHADO — ressalvas §42 ENCERRADAS**. Artefato: `NEW_ERA_PHASE_0C10B_GS_RECV_C3C4_AND_ENCRYPT_STATE.md`. Governança: correções só via append.

## 44. 0C.11-A — Spec final MVP Login (0xF1) consolidada
- Data: 2026-09-04 · SEM REDE · consolida 0C.9-A..F + 0C.10-A/B (§36-§43) no documento NORMATIVO `NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md` (11 fontes primárias com sha256 completo: C1-C5 cliente + G1-G5 GS + J1 JoinServer; código sempre primário).
- **Conteúdo normativo**: (a) fluxo 3-pacotes 0xF1:00→0xF1:01(C3)→0xF1:01(Value) com layouts byte-level (12 B handshake / 49 B request [INFERIDO 10/10] / result 5 B); (b) stack TX na ordem exata: BuxConvert(XOR3 {FC,CF,AB}) → XorData encadeado (tabela 32 B ≡ m_XorFilter GS) → serial → SimpleModulus (((Key·d)%Modulus)^Xor) → C3 (<256 B); (c) ENCRYPT_STATE=0 (whitening OFF); (d) validações GS/JS com linhas (result 6 aborta pré-JS; anti-replay; JS 0-4 com GlobalPassword flagado como NÃO-reproduzir); (e) tabela canônica de Values com emissão neste build marcada (núcleo {0,1,2,3,4,6}+0x20+default 0x07; resto compat); (f) Implementation Notes (reproduzir/stub/abertos).
- Classificação: **(1) SPEC NORMATIVA PRONTA — base para a FASE 1 de implementação do MVP de login**. Artefato: `NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md`. Governança: correções só via append.

## 45. FASE 1 INICIADA — 1.0-A scaffold MVP login criado
- Data: 2026-09-04 · SEM REDE · primeiro código NEW-ERA (implementação ISOLADA em `NEW_ERA_IMPLEMENTATION/mvp_login/`; repo legado e NEW_ERA_EVIDENCE/ intactos). Base normativa: `NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md` (sha e583db6b…, §44).
- **Arquivos criados (7)**: `README.md` (objetivo+spec+backlog) · `login_spec_trace.md` (checklist 3 pacotes + 6 camadas TX com offsets da spec) · `packet_headers.h` (structs C1/C2/C3/C4 + HeaderSubValue + F1_00_JoinServer 12 B + helpers) · `crypto_buxconvert.h` (**implementado** — XOR3 {FC,CF,AB}, §41) · `crypto_xordata32.h` (algoritmo **implementado** §43; tabela 32 B = placeholder ZERADO + bytes canônicos citados em comentário; ativação na 1.0-B) · `crypto_cpacketmanager_stub.h` (interface Encrypt/Decrypt/LoadKeys — stub puro, TODOs 1.0-C+) · `mvp_login_client.cpp` (skeleton build_F1_00_receive_join_parser / build_F1_01_login_request_packet / parse_F1_01_login_result — vazios, sem sockets).
- Governança da FASE 1: implementação isolada; anti-invenção via TODO(spec §…); MASTER_CHECKPOINT não alterado neste passo (fora do escopo do comando 1.0-A).

## 46. FASE 1 — 1.0-B XOR32 + builder do C1 F1:01 (plain) implementados
- Data: 2026-09-04 · SEM REDE · edições somente em `NEW_ERA_IMPLEMENTATION/mvp_login/` + docs NEW-ERA. Evidências intactas.
- **Tabela XOR32 preenchida por EXTRAÇÃO PROGRAMÁTICA** (não digitada): primária `0E1L__10__StreamPacketEngine…h` (byXorFilter :68-74, sha 797d26d9…) → `{E7 6D 3A 89 BC B2 9F 73 23 A8 FE B6 49 5D 39 5D 8A CB 63 8D EA 7D 2B 5F C3 B1 E9 83 29 51 E8 56}`; cross-check `0C10A__PacketManager…cpp` (m_XorFilter :85-116): **MATCH 32/32**. `crypto_xordata32.h`: kXorFilter32 constexpr + proveniência no comentário; variante não-fiel (seed 0) REMOVIDA; fronteira documentada por evidência (header C1 [0..2] sem XOR; primeiro XORed = [3], predecessor = head [2]; aplicação por campo ≡ contígua).
- **`BuildC1_F1_01_LoginRequestPlain` implementada** (49 B, spec §4.2): C1/49/F1/01 + ID[10]/PASS[10] (zero-pad + BuxConvert em cópias locais) + Tick LE + VerEnc[i]=rawVer[i]-(i+1) + Serial[16] + **XorData32 em [3..49)**; `static_assert(49)`; `build_F1_01_login_request_packet` agora encadeia o builder (serial_byte=0 → TODO 1.0-C). **OBSERVAÇÃO de fidelidade registrada no código**: no legado `spe.Send` não chama `End()` (StreamPacketEngine.h :120-123 vs :47-64) — byte de size interno possivelmente vestigial dentro do C3; NEW-ERA grava 49 conforme spec §4.2 (tamanho real viaja no envelope).
- Syntax-check: `g++ -std=c++17 -fsyntax-only` **OK** (nenhum binário gerado/executado). **Sem SimpleModulus/C3 ainda — próximo passo 1.0-C** (cipher por bloco + chaves ENCDEC, ou stub com test-vectors). Artefatos: `crypto_xordata32.h`, `mvp_login_client.cpp`, `login_spec_trace.md` (checklist camada 3 marcada).

## 47. FASE 1 — 1.0-C: builder C3 F1:01 + implementação mínima SimpleModulus (CPacketManager)
- Data: 2026-09-04 · SEM REDE · novo `crypto_cpacketmanager_min.h` (class PacketCryptoSM) + `BuildC3_F1_01_LoginRequestEncrypted` em `mvp_login_client.cpp`. Syntax-check **OK**. Evidências intactas (0C10A PacketManager.cpp sha 3e82e5d2…; wsclientinline.h 67a6d535…).
- **PacketCryptoSM (caminho CLÁSSICO 8B→11B, fiel por linha)**: Encrypt :236-:263 (blocos 8→11; outSize=((dec+4dec)*2)+dec); EncryptBlock :327-:364 (4×WORD LE; `Enc=((Xor^w^EncValue)*Key)%Modulus` :337; chain `Enc[n]=(Enc[n]^Xor[n])^(WORD)Enc[n+1]` :343; AddBits 16+2 bits ×4 = 72 bits; checksum 0xF8^src[0..7] :354-:359; tail `(0x3D^size^CS, CS)` :361-:362 + 16 bits = 88 bits = 11 B); AddBits :430-:470 reescrito equivalente (bitstream MSB-first, OR — evidência :438-:465); chaves por injeção SetKeys (ENCDEC_DATA uint32[4]×3, PacketManager.h:25-30). **NÃO implementado (TODO)**: caminho ≥701 (DES ProcessData :216-:233 — build flag [NOT RECOVERED]); Decrypt/DecryptBlock (:268/:369, passo RX).
- **Wrap C3 (evidência inline.h:88-:126)**: serial **substitui** byte[1] (:92-:94); cifra **[1..49)** = serial+head+sub+payload (48 B → 66 B ct); byte[0]=0xC1 **descartado**; envelope `[0xC3][68][ct…]`. Serial incremental estático local. static_asserts: plain=49, 48→66, 68<256 (permanece C3).
- **HIPÓTESE marcada (TODO 1.0-D)**: padding do último bloco parcial — EncryptBlock lê 8 bytes incondicionalmente (:356-:359); builder usa zero-pad no último bloco (48 B = 6 blocos exatos ⇒ no login NÃO há bloco parcial; hipótese só afeta tamanhos não múltiplos de 8).
- **TODO 1.0-D**: chaves reais Enc1/Dec2 (arquivos não presentes no workspace) + validação contra PacketManager; parse S→C F1:00.

## 48. FASE 1 — 1.0-D: chaves Enc1/Dec2 localizadas/copiadas + loader implementado + builder C3 preparado para chaves reais
- Data: 2026-09-04 · SEM REDE · **chaves ENCONTRADAS no workspace**: `muonline/Client/Data/Enc1.dat` (54 B, sha ee73a59b…) e `Dec2.dat` (54 B, d19e950c…); irmãos do servidor `MuServer/Data/Hack/Enc2.dat` (9cf86fdc…) / `Dec1.dat` (5364ab7d…). Copiadas (isoladas): `NEW_ERA_IMPLEMENTATION/mvp_login/keys/{Enc1,Dec2}.dat` + `KEYS_MANIFEST.md` (origem/destino/bytes/sha).
- **Formato EXTRAÍDO da evidência (sem invenção)**: LoadKey PacketManager.cpp :147-:212 — header `uint16 LE=4370 (0x1122)` + `uint32 LE=54` (:129/:142/:168), 3 tabelas `uint32 LE [4]` (Modulus :187, Key :194, Xor :201) cada dword XOR `m_SaveLoadXor={0x3F08A79B,0xE25CC287,0x93D27AB9,0x20DEA7BF}` (ctor :37-:40). **VALIDADO offline nos 4 arquivos reais** (parser python: header/size OK ×4).
- **⭐ DESCOBERTA (prova matemática do pareamento + da estrutura do cipher)**: dentro de cada par cliente↔servidor Modulus e Xor são IDÊNTICOS e **Key é o INVERSO MODULAR**: `Dec1.Key[i]=Enc1.Key[i]⁻¹ mod Enc1.Modulus[i]` (0x5BC1⁻¹ mod 0x1F44F = 0x7B38 ✓) e `Enc2.Key[i]=Dec2.Key[i]⁻¹ mod Dec2.Modulus[i]` (0x4673⁻¹ mod 0x11E6E = 0x3371 ✓) — confirma Enc1(cliente TX)↔Dec1(GS RX) e Enc2(GS TX)↔Dec2(cliente RX) E valida indiretamente a fórmula modular implementada na 1.0-C.
- **Implementação**: `crypto_sm_keyfile_loader.h` (EncDecKeys + LoadEncDecKeysFromFile fiel :147-:212); `PacketCryptoSM::LoadKeysFromFile` (+flag loaded_; Encrypt retorna -2 sem chaves, a menos que NEWERA_ALLOW_DUMMY_KEYS — default OFF); `TryLoadLoginKeys` (path keys/Enc1.dat) e `BuildC3_…` agora exige chaves reais e reporta erro claro (std::string* err). Syntax-check **OK**.
- Limitações: Decrypt/DecryptBlock (RX) e caminho ≥701 DES seguem TODO (1.0-E). Chaves do servidor NÃO copiadas (só manifestadas).

## 49. FASE 1 — 1.0-E: RX implementado (C3 parse + Decrypt + serial + parse F1:00/F1:01)
- Data: 2026-09-04 · SEM REDE · edições em `crypto_cpacketmanager_min.h` (Decrypt/DecryptBlock) + `mvp_login_client.cpp` (pipeline RX) + `login_spec_trace.md` (RX marcado). Syntax-check **OK** (nenhum binário executado).
- **DECRYPT fiel por linha (PacketManager.cpp 0C10A)**: `Decrypt` :288-:314 (blocos 11→8; **quirk preservado**: `if(result<0)` checa o acumulado, não TempResult :302 — comentado no código); `DecryptBlock` :377-:425 (leitura via AddBits 16+2 bits ×4 :386-:392; unchain n=2..0 :394-:397; `w=(((Key·Dec)%Mod)^Xor)^value` :401-:405 com value=(WORD)DecBuffer[n]; tail :407-:425: size=b0^b1^0x3D, checksum=0xF8^out[0..7], mismatch→-1). `GetByteOfBit` :474 e `Shift` :487 lidos — AddBits-equivalente da 1.0-C cobre ambos os papéis (leitura/escrita).
- **⭐ EVIDÊNCIA RX COMPLETA (SocketManager.cpp 0C10B :285-:340 + PacketManager.cpp :530-:583)**: remontagem C1 do plain (`DecSerial=DecBuff[1]` :303; `DecBuff[0]=0xC1` :308; `DecBuff[1]=DecSize=plain+1` :309 — **size=49 reconstruído, validando a observação 1.0-B**); `AddData` :530-:540 é só cópia; **`ExtractPacket` :542-:570 chama `XorData(size-1,2)` :565 → unXor REVERSO `buf[n]^=buf[n-1]^Filter[n%32]`, n=size-1..3 (:579-:581)** — exato inverso do TX; Queue com encrypt+serial :324-:326 → ProtocolCore :882.
- **Implementação RX**: `TryLoadReceiveKeys` (Dec2.dat, type=1 — loader ampliado p/ enc|dec); `UnXorStreamLikeEvidence`; `ParseC3Packet` (serial DENTRO do ct — evidência :303/byDec[2] :11703); serial esperado estático com **resync não-fatal** (evidência :11703-:11708) e incremento pós-sucesso (HIPÓTESE menor, par do TX :94); `DecodeAndParseMvpPacket` (C1/C3 → decrypt → remontagem fiel → unXor → F1:00 com regra `Version[i]-(i+1)` / F1:01 Value / outros heads: "não suportado" sem falhar). C4 e caminho ≥701 DES: **TODO**.
- Próximo (1.0-F): test vectors offline — roundtrip Encrypt→Decrypt (chaves Enc1/Dec2 reais já em keys/) + golden bytes do login.

## 50. FASE 1 — 1.0-F: golden test vectors gerados (TX/RX login MVP)
- Data: 2026-09-04 · SEM REDE/sockets/RNG · `NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors/`: `generate_vectors.cpp` + `compile_and_run.sh` + `genvec` + `README.md` + golden `login_vectors.json` + `login_vectors.md`. Compilação e execução LOCAIS do gerador autorizadas pelo comando 1.0-F (FASE 1). **GENVEC OK — todos os asserts passaram** (exit 0).
- **Casos**: T1 login request (plain 49 B campo-a-campo; C3 68 B `[C3][44]…`; **roundtrip cripto com Dec1 DERIVADO por inverso modular de Enc1 — relação §48 exercitada em código**); R1 F1:00 simulado com Xor+Encrypt (C3 24 B → RX completo: decrypt+serial+unXor+parse result=01/heroKey=0x1234/versionOk); R2 F1:01 estilo GS REAL (**Encrypt SEM Xor** — DataSend :443-:470 ⇒ novo parâmetro `streamXored=false` no DecodeAndParseMvpPacket; nuança evidenciada: o XorData é artefato do BUILDER do cliente AddData :82-:94, não camada de transporte do servidor).
- **⭐ BUG REAL PEGO PELOS VETORES (e corrigido)**: `DecryptBlock` usava o retorno de `AddBits` (=`targetPos+size`, :469) para avançar a posição da FONTE — o evidence avança `BitPos += 16 / += 2` EXPLICITAMENTE (:388-:391); com o retorno, o bitPos saltava 18→24 e corrompia slots 1-3. Fix em `crypto_cpacketmanager_min.h` + validação dupla (modelo python 4/4 OK antes do rebuild). Lição registrada: retorno de AddBits só avança posição de TARGET.
- Ajustes de suporte: `main()` de mvp_login_client.cpp sob guard `NEWERA_MVP_NO_MAIN`; accessor `DebugKeys()` p/ derivação de inversos. Determinismo: serialPkt=0x01 (1ª chamada, static init 0), serials RX 01/02 fixos.

## 51. FASE 1 — 1.0-G: loopback local (127.0.0.1) validando golden vectors do MVP login
- Data: 2026-09-05 · SEM REDE EXTERNA (bind/connect SOMENTE 127.0.0.1 hardcode, porta efêmera; sem args de host/porta) · execução local autorizada (FASE 1). **RUN OK — exit 0** com o fluxo completo: server enviou R1 → client decodificou (F1:00 result=01 heroKey=0x1234 versionOk serial=1) → client enviou T1 (builder REAL, Enc1) → **server matched T1_expected byte-a-byte (memcmp)** → server enviou R2 → client decodificou **F1:01 Value=1 (login OK)**.
- **Artefatos** (`NEW_ERA_IMPLEMENTATION/mvp_login/loopback/`): `embedded_vectors.h` (gerado programaticamente do golden JSON 1.0-F — sha do json registrado no topo: 9d5e66ff…; arrays kR1 24 B/kR2 13 B/kT1_expected 68 B) · `loopback_harness.cpp` (2 threads POSIX: server stub bind 127.0.0.1:0+promise de porta; client MVP com pipeline real — DecodeAndParseMvpPacket c/ streamXored true(R1)/false(R2) e BuildC3_F1_01_LoginRequestEncrypted; recvall/sendall; diff no 1º offset em mismatch) · `compile_and_run.sh` (compila TU único — harness inclui mvp_login_client.cpp c/ NEWERA_MVP_NO_MAIN; imprime OK só se exit 0) · binário `loopback_harness`.
- Retomada: comando executado após indisponibilidade temporária do sandbox (tentativas 2026-09-04 com saída perdida; nenhum arquivo tocado nelas — verificado na retomada: loopback/ inexistente, golden íntegro 9d5e66ff…).
- Classificação: **(1) prova end-to-end local completa — TX e RX do MVP de login validados com socket real contra golden vectors determinísticos.**

## 52. FASE 1 — 1.1-A: Spec MVP F3 comuns (00/01/02/03/06/30/52) gerada
- Data: 2026-09-05 · SEM REDE · `NEW_ERA_PROTOCOL_MVP_F3_COMMON_SPEC.md` (normativa do próximo MVP pós-login; fontes G1/C1/H com sha). Execução anterior deste comando foi INTERROMPIDA pelo usuário antes de qualquer escrita — verificado nesta retomada (spec inexistente, §52 ausente, Ledger 130.402 B 446f397e…).
- **Extração fresca**: servidor (:955-:1034): 0x00 CGCharacterListRecv(aIndex) **sem cast/payload** :958 · 0x01 CGCharacterCreateRecv :961 · 0x02 CGCharacterDeleteRecv :964 · 0x03 CGCharacterInfoRecv :967 · 0x06 CGLevelUpPointRecv :970 · 0x30 CGOptionDataRecv :991 · 0x52 gMasterSkillTree.CGMasterSkillRecv :999/:1001 — casts PMSG_*_RECV com defs [NOT RECOVERED]. Cliente (:12942-:13038): 0x00 ReceiveCharacterList :12960 (def :523) · 0x01 ReceiveCreateCharacter :12963 (:622) · 0x02 ReceiveDeleteCharacter :12966 (:672) · 0x03 ReceiveJoinMapServer :12970 (:871, FALSE em falha) · 0x06 ReceiveAddPoint :12988 (:6200) · 0x30 ReceiveOption :13023 (:9388) · 0x52 Receive_Master_LevelGetSkill :13035 (:7387).
- **Structs (WSclient.h, TODAS encontradas)**: PHEADER_DEFAULT_CHARACTER_LIST :137-:146 (7 B) · PRECEIVE_CREATE_CHARACTER :376-:386 (19 B, MAX_ID=10 inf.) · PHEADER_DEFAULT_SUBCODE :129-:134 (5 B, §38) · PRECEIVE_ADD_POINT :883-:890 (11 B) · PRECEIVE_OPTION :1205-:1215 (34 B, pack(1)) · PMSG_ANS_MASTERLEVEL_SKILL :2860-:2868 (15 B). Faltantes: PMSG_*_RECV server [NOT RECOVERED] + payload VARIÁVEL da char-list (corpo de ReceiveCharacterList :523 — extrair na 1.1-B).
- **Escolha 1.1-B: subcode 0x00 CHARACTER LIST** (1º pacote pós-login; request vazio = zero lacunas C→S; response 7 B + slots a extrair). Direcionalidade documentada (comuns = mesmo subcode em direções opostas, não mesmo pacote — §35).

## 53. WORKSPACE HYGIENE — removidos binários re-geráveis do MVP
- Data: 2026-09-05 · SEM REDE · comando IMPL-BIN-PRUNE (higiene de budget; NÃO é mudança de fronteira técnica — CURRENT/MASTER intencionalmente não alterados).
- **Removidos**: `NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors/genvec` (248.352 B, ELF x86-64, sha 4270e84f2adee302… = §50) · `NEW_ERA_IMPLEMENTATION/mvp_login/loopback/loopback_harness` (512.048 B, ELF x86-64, sha d7abec33ef09def4… = §51) → **760.400 B liberados**. Nenhum .o/.obj/.exe/a.out existia no MVP (pré-checagem find vazia). Re-geráveis por `test_vectors/compile_and_run.sh` e `loopback/compile_and_run.sh` (TU+fontes preservados).
- Pós: workspace 133.888.443 B / 4.444 arquivos; IMPL 25.192 B / 21 arquivos; **NEW_ERA_EVIDENCE/ e muonline/ INALTERADOS** (manifest sha256 agregado pré=pós `b421fde52625dad6…`); golden vectors (.json/.md), fontes (.cpp/.h) e scripts (.sh) presentes.
- Nota: binários re-geráveis; fontes+scripts+golden preservados; sem rede.
- ERRATA §53 (mesma data, append-only): valores corretos de NEW_ERA_IMPLEMENTATION/ pós-prune = **63.759 B / 20 arquivos** (o texto acima registrou "25.192 B / 21 arquivos" por erro de transcrição do autor; todos os demais números do §53 conferem: workspace 133.888.443 B / 4.444 arquivos; liberados 760.400 B; manifest EVIDENCE+muonline pré=pós).

## 54. FASE 1 — 1.1-B: F3:0x00 CharList (builder request + parser response) implementado (sem golden ainda)
- Data: 2026-09-05 · SEM REDE · SEM BINÁRIO (apenas `g++ -fsyntax-only` — OK). Único arquivo de código modificado: `NEW_ERA_IMPLEMENTATION/mvp_login/mvp_login_client.cpp` (21.186 B, sha 81771b0d62d3936a…) — bloco 1.1-B append dentro de `newera::mvp`.
- **Evidência server-side (request SEM payload)**: GS_Protocol.cpp :955-:960 — `case 0xF3: switch(lpMsg[3]) { case 0x00: CGCharacterListRecv(aIndex); }` (lpMsg NÃO é lido).
- **Evidência client-side (layout variável)**: WSclient.cpp :523-:600 ReceiveCharacterList — cast :527; Offset=sizeof(hdr) :529; **count = Data->Value** :532/:537; slots PRECEIVE_CHARACTER_LIST :539/:574; campos usados por slot: Index/Class(:541)/Level(:565)/CtlCode(:566)/ID(:568-:569, +NUL em [10])/Equipment(:571)/byGuildStatus(:573); switch(Index) 0..4 com **default:return** :545-:561 (parser espelha: Index>4 => erro).
- **Tamanhos PROVADOS por defines (upgrade vs 1.1-A)**: MAX_ID_SIZE=10 (0E1JN2 Define.h:4; 0E1JN3 define.h:305 — antes [INFERIDO]) · EQUIPMENT_LENGTH=17 (WSclient.h:71) ⇒ **slot=33 B** (1+10+2+1+1+17+1) e response header=7 B; C1(N chars)=7+33N.
- **Builder C→S**: `BuildC1_F3_00_RequestCharListPlain()` = [C1][04][F3][00] + XorData32 em [3..4) ⇒ subcode 0x00→**0x7A** (0x00^0xF3^Filter[3]=0x89); `BuildC3_F3_00_RequestCharListEncrypted(err)` = serial(1)+[F3][7A] → Encrypt Enc1 (bloco parcial 3 B zero-padded, HIPÓTESE 1.0-D a validar na 1.1-C) ⇒ **C3 de 13 B** (2+11). Serial: static próprio (evidência = 1 contador/conexão, wsclientinline :94 — NOTA no código).
- **Parser S→C**: `ParseC1_F3_00_CharacterListPlain(c1,out,err)` — valida C1/F3/00; header {sub,maxClass,moveCount,count}; loop com bounds-check (NOVO, legado não tem); structs F3CharListHeader/F3CharListEntry semânticas (sem overlay ⇒ pack(1) N/A); Equipment como offset (17 B). Response tratado acima com **streamXored=false** (GS TX sem Xor — DataSend :443-:470, como R2).
- **TODO 1.1-C**: golden vectors F3:00 (request C3 13 B determinístico + response C1 parseável) + loopback stub (server manda char-list, client parseia) — **remover binário ao final** (budget).

## 55. INFRA — tentativa de push GitHub (repo público NEW-ERA-KNOWLEDGE) — FALHA 403; legado PRESERVADO
- Data: 2026-09-05 · rede autorizada pontualmente pelo comandante (github.com, push/fetch) — reversão explícita das regras OFFLINE para este comando · `.git` inicializado NO workspace (identidade local new-era-agent; origin SEM token; **PAT usado apenas na URL do push, nunca gravado**).
- **Executado**: stage /tmp/newera_stage (51 arquivos NEW-ERA, 497.887 B) → fetch OK (repo existe, **vazio**: sem origin/main|master → branch main nova) → restore → .gitignore (muonline/, NEW_ERA_EVIDENCE/, keys/*.dat, binários) → **commit local 50 arquivos, auditoria limpa: 0 .dat / 0 muonline / 0 EVIDENCE versionados** (só KEYS_MANIFEST.md em keys/) → **PUSH 403**: "Permission to heldergaraujo2/NEW-ERA-KNOWLEDGE.git denied to heldergaraujo2" (token autentica como heldergaraujo2 mas SEM escrita — PAT fine-grained sem "Contents: Read and write" neste repo).
- **Consequências**: `rm -rf muonline/` NÃO executado (condicionado a push OK — legado intacto, READ-ONLY preservado); NEW_ERA_EVIDENCE/ intacto; commit permanece LOCAL (pronto para re-push). Correção depende do OWNER: editar o PAT (Repository access incluir NEW-ERA-KNOWLEDGE + Contents: Read/Write) e reordenar o push.
- Nota ambiente: `.git/config` não persiste entre turnos (política do sandbox) — em re-execução futura, reconfigurar user.name/email/origin (objetos/commit persistem).

## 56. WORKSPACE THIN MODE — pushed to GitHub via deploy key + pruned muonline/ after push OK
- repo: heldergaraujo2/NEW-ERA-KNOWLEDGE · branch: main · data/hora local: 2026-09-05 ~13:3x BRT (UTC ~16:3x) · rede: somente github.com (ssh-keyscan + push SSH; sem PAT).
- Push via deploy key ED25519 (fingerprint SHA256:5LIYYXfFfChaC27sMy6/YQxjEH+AskpsMwBAX/B5Ws4) — commits: 406a3e6 (sync 50 arquivos) + sync (deploy key) c/ §55. Auditoria pré-push: 0 *.dat / 0 muonline/ / 0 NEW_ERA_EVIDENCE/ versionados; .ssh/ e uploads/ NÃO versionados; remote SSH sem credenciais.
- **muonline/ removido localmente para budget; não era parte versionada** (workspace 134.240.420 B → 4.764.556 B, −96,4%). NEW_ERA_EVIDENCE/ mantido (intacto). Falha 403 do PAT documentada no §55 — resolvida via deploy key com write access.
