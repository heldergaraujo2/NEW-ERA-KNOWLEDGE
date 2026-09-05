# NEW-ERA — CURRENT STATE
> RECONSTRUÇÃO GERADA EM RESPOSTA (não é o original do coordenador — original AUSENTE do workspace).
> Base: exclusivamente arquivos do workspace (ver Fontes). Nada foi executado ou alterado.
> Data da geração: 2026-09-03 · Proveniência: [LEDGER] NEW_ERA_RESEARCH_LEDGER.md (65.962 B, 594 L)
> **Atualização 2026-09-04**: 0C.7-A-NET consolidado com evidência em arquivo (Ledger §22; relatório + fonte persistida) — ver §3/§4.

## 1. Estado Atual
- **Fase 1 — IMPLEMENTAÇÃO · em andamento (iniciada em 1.0-A, 2026-09-04)**.
  Código novo ISOLADO em `NEW_ERA_IMPLEMENTATION/` (repo legado e evidências
  intactos). Fase 0 (§0-§44) PRESERVADA como base normativa.
  [LEDGER §0 :11-13; §2.1 :~36-40]
- Repositório de pesquisa: wongddd/muonline @ commit canônico 580472e (branch main).
  Nenhum outro commit usado como fonte. [LEDGER §0 :14-16]
- Regras ativas: READ-ONLY absoluto; microteste único por mensagem; evidência antes de
  conclusão; EVIDÊNCIA/CONCLUSÃO/HIPÓTESE separados; nada de New-Era no repo; sem
  fork/clone/ZIP/backup adicionais; relatórios fora do repo. [LEDGER §2 :~31-58]
- Workspace DEGRADADO (muonline/ sem .git; fontes parciais) → GitHub raw = fonte de
  verdade; /tmp é efêmero (fora do workspace). [LEDGER §3 A1-A5 :~24-30]

## 2. Último Microteste Formalmente Aprovado
- **0C.6-E** (Validação header.size no recv do servidor) — CONFIRMED/APROVADO.
- Citação: [LEDGER §4 :133] "Status de aprovação: 0A/0B/0C.1–0C.5 e 0C.6-A..E =
  **APROVADOS** (livro-mestre do coordenador, mensagem de auditoria 0C.6-E no chat
  [CHAT])." · Detalhe em [LEDGER §5 :210-216] · Linha da tabela: §4 :113.

## 3. Fronteira Real do Workspace (evidência em ARQUIVO)
- **Último microteste REGISTRADO em arquivo: 0C.4-I** (Censo decode/crypto no cliente)
  — [LEDGER §4 :~131-132, status "EXECUTADO/ENTREGUE (chat)"] + §5 :280-286
  (BuxConvert 0xFC/0xCF/0xAB campo-level; XOR de frame unidirecional c→s;
  Resolve a incerteza levantada em 0C.6-K).
- Nuance obrigatória: a evidência ORIGINAL do 0C.4-I é [CHAT], mas o REGISTRO existe
  em arquivo (Ledger). É a fronteira documental, não uma aprovação.
- **0C.7-A-NET (2026-09-03 execução / 2026-09-04 consolidação)**: censo COMPLETO do
  switch(head) do ProtocolCore GS — 107 cases top-level (107 distintos, sem
  default), switch :98-1164, função :76-1165; H4 confirmado (case PROTOCOL_CODE4 @:771 →
  CGMultiSkillAttackRecv); ERRATA da nota BOTH_POSITION@110. Evidência EM ARQUIVO:
  [FILE] `NEW_ERA_PHASE_0C7A_GS_PROTOCOLCORE_SWITCH_CENSUS.md` + [FILE]
  `NEW_ERA_EVIDENCE/GS_Protocol.cpp__580472e__sha256_e8bb4dbb.cpp` (sha256 conferido) +
  [LEDGER §22]. Fecha a pendência §15.1.
- **0C.7-B (2026-09-04)**: censo dos sub-switches
  `switch(lpMsg[3])` do mesmo ProtocolCore — **26 sub-switches · 155 labels
  (divergência ZERO vs 0C.7-A-NET) · 54 sub-opcodes globais distintos · 21 repetidos
  entre HEADs · 1 único default (HEAD 0xBF @:656, com LOG)**. Sem rede (fonte =
  evidência persistida). Evidência EM ARQUIVO: [FILE]
  `NEW_ERA_PHASE_0C7B_GS_PROTOCOLCORE_LPMSG3_SUBSWITCH_CENSUS.md` + [LEDGER §23].
  Fecha a pendência derivada da §22.
- **0E.1-J (2026-09-04)**: caça à definição de
  `MAX_MODELS` (workspace local, sem rede) — **classificação (4) INCONCLUSIVO**:
  0 ocorrências do token em TODO o repo local (incl. Main.vcxproj → **não é
  build-define**; enum-rx coberto). Faltam: raiz de `Source Client/source/`
  (local VAZIA) + lado GitHub nunca varrido. Achados laterais: divergência
  workspace↔commit (`_TextureIndex.h` raiz 23.257 B ≠ GitHub source/ 18.779 B),
  `.rsp` descartado dos autores, define Release `LDS_PATCH_GLOBAL_100520`.
  Evidência EM ARQUIVO: [FILE] `NEW_ERA_PHASE_0E1J_MAX_MODELS_DEFINITION_HUNT.md` +
  [LEDGER §24].
- **0E.1-J-NET (2026-09-04)**: fechamento via rede
  restrita **BLOQUEADO na etapa A** — GitHub Code Search API → **HTTP 401 "Requires
  authentication"** (0 paths → 0 downloads; regra "somente paths do search" cumprida).
  Classificação (4) mantida. Evidência EM ARQUIVO: [FILE]
  `NEW_ERA_EVIDENCE/MAX_MODELS__search_code_api__response_401.json`(+.SHA256) +
  [FILE] `NEW_ERA_PHASE_0E1JN_MAX_MODELS_NET_SEARCH_AND_EVIDENCE.md` + [LEDGER §25].
- **0E.1-JN2 (2026-09-04)**: Contents API 200 (613
  itens de `Source Client/source` @580472e) + 25 raw baixados (63 candidatos
  regex, 38 truncados). **Classe (4) com progresso substantivo [FILE-FACT]**:
  `MAX_MODELS` só existe como COMENTÁRIO (`ZzzBMD.cpp:22 //BMD Models[MAX_MODELS];`);
  storage atual de modelos = **ponteiro dinâmico `BMD *Models`** (ZzzBMD.cpp:23,
  extern ZzzBMD.h:334); nenhuma definição nos 25; site de alocação (`new BMD[...]`)
  FORA do conjunto (provável ZzzOpenData.cpp [§21.1 chat-only], truncado).
  Evidência EM ARQUIVO: [FILE] `NEW_ERA_PHASE_0E1JN2_MAX_MODELS_CONTENTS_RAW_AUDIT.md`
  + 25 evidências `NEW_ERA_EVIDENCE/0E1JN2__*` (+.SHA256) + [LEDGER §26].
- **0E.1-JN3 (2026-09-04)**: 7/7 raw@580472e (lista
  fechada). **Classe (2): MAX_MODELS EXISTE (3 usos ativos) mas valor fora dos 7**.
  [FILE-FACT] alocação `ZzzOpenData.cpp:110-113 OpenPlayers()`: `new BMD
  [MAX_MODELS+1024]` (:111) + `Models = ModelsDump + rand()%1024` (:112,
  anti-tamper) + `ZeroMemory(Models, MAX_MODELS*sizeof(BMD))` (:113) — MAX_MODELS
  É o limite real; teardown `Winmain.cpp:447-449/:459`; boot `:5394-5396`.
  Definição ausente nos 7 (incl. _define.h) → candidato #1 `_enum.h`
  (stdafx.h:85). Evidência EM ARQUIVO: [FILE]
  `NEW_ERA_PHASE_0E1JN3_MODELS_ALLOCATION_AND_MAX_MODELS_AUDIT.md` + 7 evidências
  `NEW_ERA_EVIDENCE/0E1JN3__*` (+.SHA256) + [LEDGER §27].
- **0E.1-JN4 (2026-09-04)**: 1 raw@580472e (_enum.h,
  56.491 B). **Classe (1): MAX_MODELS = 10002 RESOLVIDO** (_enum.h:1046
  `MAX_MODELS = (MODEL_MASK_HELM+MAX_ITEM_INDEX)`; cadeia mecânica completa,
  612 membros, 0 símbolos faltantes; âncoras 6/6; geometria da alocação fecha
  com §27: `new BMD[11026]`, janela 10002 + offset ≤1023). Ressalvas: comentário
  autoral :1001 "// (515)" STALE (515 < mínimo aritmético 1086); flags em
  Winmain.h (não baixado) só elevam → faixa [10002, ~10103]. **Série 0E.1-J
  ENCERRADA** (macro/build refutados §24; ponteiro §26; alocação §27; valor §28).
  Evidência EM ARQUIVO: [FILE]
  `NEW_ERA_PHASE_0E1JN4_MAX_MODELS_VALUE_FROM_ENUM_H.md` +
  `NEW_ERA_EVIDENCE/0E1JN4__enum_h__580472e.h`(+.SHA256) + [LEDGER §28].
- **0E.1-K (2026-09-04)**: censo offline (repo
  local = 0 hits; 35 evidências @580472e). **1293 indexadores de Models[]**
  (500 diretos + 793 via AccessModel) · **CHECK=SIM 3 / NÃO 1286 / INDET 4** ·
  wrapper AccessModel SEM guard (LoadData.cpp:22-43) · ~272 sites dinâmicos sem
  guard (173 campo-de-objeto `o->Type` + 91 param/base+param + ~8 var real),
  incluindo ESCRITAS (ZzzCharacter:12948-52; SMD:221) · Winmain:447 = único
  guard MAX_MODELS do cliente · OOB ≥10002 cai na folga não-zeroed (silencioso).
  Evidência EM ARQUIVO: [FILE]
  `NEW_ERA_PHASE_0E1K_MODELS_INDEXERS_BOUNDS_AUDIT.md` + [LEDGER §29].
- **0E.1-L (2026-09-04)**: origem de OBJECT::Type
  fechada (offline + 1 Contents API 200/613 + 23 raw@580472e). **Classe (3) c/
  nuance (2)**: o->Type = sempre enums internos (ZzzCharacter:11219); seletor
  de rede 16-bit SEM validação (0x13 máscara ≤1023; 0x1F/0x45/Preview puro)
  neutralizado por **default-substitution** (`default: OpenMonsterModel(0)`
  ZzzCharacter:13386); MonsterIndex cru :12422 só comparado; única injeção R
  indexável = equipamento (Weapon.Type=MODEL_SWORD+ExtType 12-bit+sentinela
  0x0FFF, máx 5180<10002 — in-bounds por construção); OpenMonsterModel sem
  guard (:2360-2366) mas 0 callers variáveis. Vetor ativo rede→OOB Models[]
  NÃO confirmado; fragilidade 0-guards permanece. Evidência EM ARQUIVO: [FILE]
  `NEW_ERA_PHASE_0E1L_OBJECT_TYPE_ORIGIN_AND_SANITIZATION.md` + 23 evidências
  `NEW_ERA_EVIDENCE/0E1L__*` (+.SHA256) + [LEDGER §30].
- **0E.1-M (2026-09-04)**: mapa rede→item/equip→
  model id (offline, 58 evidências; 40 handlers; 8 caminhos P1-P8). **Classe
  (3): falta EXATAMENTE a def de `ConvertItemType`**. Provado in-bounds: P2
  (ExtType 12-bit+sentinela 0x0FFF, ≤5180) e P3 (máscaras 1-3 bit, ≤7997).
  Condicionais: P1 `MODEL_ITEM+Type` (0x25, sentinela 0x1FFF) e **P4
  `CreateItem:6145 o->Type=MODEL_SWORD+Type` SEM sentinela (itens no chão →
  ~95 indexadores o->Type do §29)** — in-bounds SE ConvertItemType ≤13 bit
  (evidência estrutural forte: 0x1FFF=8191=16×512−1; lattice MODEL_HELM2=
  MODEL_ITEM+8192). Sanitização EXPLÍCITA só em arrays de inventário (0x20
  Key clamp; 0x22 Result range) — nunca em Models[]. Evidência EM ARQUIVO:
  [FILE] `NEW_ERA_PHASE_0E1M_ITEM_EQUIP_MODEL_MAPPING_AUDIT.md` + [LEDGER §31].
- **0E.1-N (2026-09-04)**: caça à def de
  `ConvertItemType` — offline (5 calls/0 defs) + Contents API 200/613 +
  10 raw (10/10 OK; 39 candidatos, **29 truncados**; 0 ocorrências nos 10).
  **Classe (3): def AUSENTE dos 68 arquivos de evidência**; range
  INDETERMINADO; P1/P4/P5 do §31 permanecem CONDICIONAIS. Suspeitos #1:
  ZzzInventory.cpp (333 KB)/.h · NewUIMyInventory.cpp. Evidência EM ARQUIVO:
  [FILE] `NEW_ERA_PHASE_0E1N_CONVERTITEMTYPE_DEFINITION_AND_RANGE.md` +
  10 evidências `NEW_ERA_EVIDENCE/0E1N__*` (+.SHA256) + [LEDGER §32].
- **0E.1-N2 (2026-09-04)**: PRIORIDADE-1
  (ZzzInventory.cpp/.h baixados; ZzzItem/Inventory.*) + P2 → 10/10 raw;
  **NOT FOUND — 6 calls/0 defs/0 protótipos em 78 arquivos** (nova call:
  ZzzInventory.cpp:7646). Inferência: def/protótipo em headers compartilhados
  ausentes — UIManager.h (nos 3 TUs), ZzzInfomation.h/.cpp (grafia fora de
  todas as regex), ZzzInterface.h, DSPlaySound.h. Classe (3). Evidência EM
  ARQUIVO: [FILE] `NEW_ERA_PHASE_0E1N2_CONVERTITEMTYPE_FIND_DEFINITION.md` +
  10 evidências `NEW_ERA_EVIDENCE/0E1N2__*` (+.SHA256) + [LEDGER §33].
- **0E.1-N3 (2026-09-04)**: **Classe (1) —
  DEFINIÇÃO ENCONTRADA e RANGE PROVADO [0, 8191]**: `ConvertItemType` def em
  `ZzzInfomation.cpp:371-377` (`Item[0]+(Item[3]&128)*2+(Item[5]&240)*32` =
  13-bit; máx 8191=0x1FFF), protótipo `ZzzInfomation.h:29`. **P1/P4/P5 do §31
  PROVADOS in-bounds** ([1086..9277] < 10002, folga 725) e **vetor
  rede→Models[] ENCERRADO** (§30 default-substitution + §31 máscaras + 13-bit
  provado). §29 = fragilidade latente, sem vetor ativo. Evidência EM ARQUIVO:
  [FILE] `NEW_ERA_PHASE_0E1N3_CONVERTITEMTYPE_HEADER_CHAIN_AUDIT.md` +
  3 evidências `NEW_ERA_EVIDENCE/0E1N3__*` (+.SHA256) + [LEDGER §34].
- **0C.8-A (2026-09-04)**: reconciliação F3
  cliente↔servidor (offline). **Classe (1): mismatch RESOLVIDO** — 0xF3 é
  namespace DUPLEX: servidor 22 subcases (CG\* = requests C→S, :955-:1034) ×
  cliente 23 (Receive\* = S→C, :12942-:13038; SubCode=[3] C1/[4] C2 provado);
  COMUNS 7 (pares request↔response) · só-servidor 15 · só-cliente 16;
  22×23 = coincidência numérica direcional. Pendência §23 ENCERRADA.
  Evidência EM ARQUIVO: [FILE]
  `NEW_ERA_PHASE_0C8A_F3_CLIENT_SERVER_SUBOPCODE_RECONCILIATION.md` + [LEDGER §35].
- **0C.9-A (2026-09-04)**: head **0xF1**
  reconciliado (série A1/A2/A3; classe (1)): servidor **3** subcases
  (:942-:953; connect-account/close-client/vazio) × cliente **7**
  (:12825-:12939; join/login/logout/create-account/passwords); comuns
  {01,02,03} · só-cliente {00,04,05,12}; **assimetria direcional +
  multi-peer/fase** (cliente = Connect/JoinServer; GS = in-game; 0x03
  vazio no GS ≠ ConfirmPassword no cliente — peers distintos). **Spec
  mínima MVP do 0xF1 registrada** (incl. enums de Value do login 0x01 e
  create-account 0x12). Evidência EM ARQUIVO: [FILE]
  `NEW_ERA_PHASE_0C9A_F1_CLIENT_SERVER_MIN_SPEC.md` + [LEDGER §36].
- **0C.9-B (2026-09-04)**: **spec operacional
  0xF1:0x01 (login result) fechada** — cadeia completa provada Cliente→GS
  (CGConnectAccountRecv :1960; versão/serial→6)→JS (GJConnectAccountRecv
  :76-:254; 0/1/2/3/4)→GS (GCConnectAccountSend :3132 set 0xF1,0x01)→Cliente
  (:12833; 23 Value labels/21 outcomes, **default→FAIL_CONNECT :12867**);
  result↔Value 1:1 (0/1/2/3/4/6); ConnectServer fora do fluxo; GameServer
  local VAZIO (só Time/lua — GS existe só na evidência @580472e). [NOT
  RECOVERED]: receiver JG→GS, defs PMSG_CONNECT_ACCOUNT_*, sender do
  request no cliente. Evidência EM ARQUIVO: [FILE]
  `NEW_ERA_PHASE_0C9B_F1_01_LOGIN_RESULT_SPEC.md` + [LEDGER §37].
- **0C.9-C (2026-09-04)**: **handshake
  0xF1:0x00 + request 0xF1:0x01 fechados em spec** — S→C 12 B
  `PRECEIVE_JOIN_SERVER` (H:270-279; Result/HeroKey/Version[5]; handler
  :392-:460 c/ path map-server move :396-:399; emissor GS
  `GCConnectClientSend`/`2` :3071/:3099, JS/CS locais 0 senders) e C→S
  macro `SendRequestLogIn` (wsclientinline.h:243-:269: BuxConvert ID/senha,
  TickCount, Version codificada `V[i]-(i+1)`, Serial[16]; ≈49 B). **Fecha a
  lacuna 3 do §37** (sender do request). Evidência EM ARQUIVO: [FILE]
  `NEW_ERA_PHASE_0C9C_F1_00_JOIN_AND_F1_01_LOGIN_REQUEST_SPEC.md` + [LEDGER §38].
- **0C.9-D (2026-09-04)**: **validação
  server-side do request 0xF1:0x01 fechada** — `CGConnectAccountRecv`
  :1960-:2006: kick :1964; **result 6 = version(:1970) OU serial(:1976)
  mismatch, aborta ANTES do JS**; anti-replay `LoginMessageSend==0` :1982;
  `PacketArgumentDecrypt` ×3 chamadas (:1790 map-move/:1992/:1996 login),
  def [NOT RECOVERED]; **BuxConvert provado = XOR 3B {FC,CF,AB}**
  (WSclient.cpp:209-:215) — equivalência = HIPÓTESE FORTE; H1 “32B” não se
  aplica a credenciais. Struct `PMSG_CONNECT_ACCOUNT_RECV` [NOT RECOVERED].
  Evidência EM ARQUIVO: [FILE]
  `NEW_ERA_PHASE_0C9D_GS_LOGIN_REQUEST_DECRYPT_VALIDATION.md` + [LEDGER §39].
- **0C.9-E (2026-09-04)**: recuperação da def
  de `PacketArgumentDecrypt` — **conclusão (3) NOT RECOVERED** (3 CALLs
  GS:1790/:1992/:1996; proto/def 0 no workspace). **Corroboração inédita**:
  2ª definição de `BuxConvert` no Encoder (`Source
  Tools/Encoder/Encoder/Util.cpp:4-:12`, sha e0424697…) com **algoritmo+chave
  idênticos** ao cliente ⇒ padrão de codebase; equivalência permanece
  HIPÓTESE FORTE. Notas legadas não-canônicas: SimpleModulus (8B→11B) e
  “login 55 B” (diverge do ≈49 B §38 — flag). Errata ortográfica §39
  (“Validção”) registrada no §40. Evidência EM ARQUIVO: [FILE]
  `NEW_ERA_PHASE_0C9E_PACKETARGUMENTDECRYPT_RECOVERY.md` + [LEDGER §40].
- **0C.9-F (2026-09-04)**: **FOUND-DEF via rede
  autorizada (1 Contents + 1/8 raw, early-stop)** — `PacketArgumentDecrypt`
  **GS Util.cpp:101-:109**: XOR 3B `{0xFC,0xCF,0xAB}` (`XorTable[n%3]`);
  **EQUIVALÊNCIA COM BuxConvert CONFIRMADA** (3 defs independentes
  idênticas: cliente :209-:215 · Encoder :4-:12 · GS :101-:109) —
  hipóteses §39b/§40 PROMOVIDAS A PROVADO (§41). Evidência EM ARQUIVO:
  [FILE] `NEW_ERA_EVIDENCE/0C9F__Util…cpp` (11.245 B, sha 6271b7be…) +
  [.SHA256] + [FILE]
  `NEW_ERA_PHASE_0C9F_PACKETARGUMENTDECRYPT_NET_RECOVERY.md` + [LEDGER §41].
  **Cadeia de login 0xF1 100% provada incluindo ofuscação de credenciais.**
- **0C.10-A (2026-09-04)**: **pipeline C3/C4 +
  SimpleModulus provado, classe (1) com ressalvas** — cliente: C3/C4 montado
  por `SendPacket` (bEncrypt→SimpleModulusCS+serial; inline.h:67-:127),
  chaves Enc1/Dec2 no boot (Winmain:1459-:1460), RX decrypt+serial
  (WSclient.cpp:11679-:11710); GS: motor `CPacketManager` (modulo+DES≥701),
  gate `ENCRYPT_STATE==1` (Util.cpp:240-:244), validação per-packet
  (HackPacketCheck :141-:143); `case 0xC3/C4` GS_Protocol:670/:673 =
  falsos positivos (friend). Ressalvas [NOT RECOVERED]: ENCRYPT_STATE@build,
  dispatch C3/C4 no recv GS, `spe.Send(TRUE)`, crypto do DataSend. Rede
  usada: 1 Contents + 4/6 raw (PacketManager.cpp/.h, HackPacketCheck.cpp/.h
  = `0C10A__*`). Evidência EM ARQUIVO: [FILE] `0C10A__*` + [.SHA256] +
  [FILE] `NEW_ERA_PHASE_0C10A_C3C4_SIMPLEMODULUS_PIPELINE.md` + [LEDGER §42].
- **0C.10-B (2026-09-04)**: **ressalvas §42
  ENCERRADAS, classe (1)** — cliente: `spe.Send(TRUE)`→C3 PROVADO
  (StreamPacketEngine.h:120-:123, def já em evidência; C3 se <256 B
  inline:102/:106); GS: RECV completo (SocketManager.cpp:271-:371, Decrypt
  :301, entrega **ProtocolCore :882** com encrypt+serial), DataSend S→C
  re-encripta C3/C4 com serial por conexão (:449/:462); **ENCRYPT_STATE=0**
  (stdafx.h:58-59; não sobrescrito no vcxproj) — whitening OFF, cripto
  ativa = CPacketManager. **Bônus**: XOR encadeado de stream (XorData
  :66-:80) com tabela 32/32 ≡ m_XorFilter GS — ordem física do login:
  BuxConvert→XOR-encadeado→SimpleModulus→C3. Rede: 1 Contents + 6/6 raw
  (`0C10B__*`). Evidência EM ARQUIVO: [FILE] `0C10B__*` + [.SHA256] +
  [FILE] `NEW_ERA_PHASE_0C10B_GS_RECV_C3C4_AND_ENCRYPT_STATE.md` + [LEDGER §43].
- **0C.11-A (2026-09-04) — FRONTEIRA DOCUMENTAL ATUAL**: **SPEC NORMATIVA
  DO MVP DE LOGIN PRONTA** — `NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md`
  (consolida §36-§43; 11 fontes com sha completo; fluxo 3-pacotes com
  layouts byte-level; stack BuxConvert→XorData→serial→SimpleModulus→C3;
  ENCRYPT_STATE=0; validações GS/JS; tabela de Values com emissão marcada;
  Implementation Notes reproduzir/stub/abertos). Primeiro documento
  NORMATIVO NEW-ERA voltado à implementação (FASE 1). Evidência EM
  ARQUIVO: [FILE] `NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md` + [LEDGER §44].
- **1.0-A (2026-09-04)**: **FASE 1 INICIADA** —
  scaffold do MVP de login criado em `NEW_ERA_IMPLEMENTATION/mvp_login/`
  (7 arquivos: README, login_spec_trace, packet_headers.h,
  crypto_buxconvert.h [implementado §41], crypto_xordata32.h [algoritmo
  §43; tabela placeholder], crypto_cpacketmanager_stub.h [stub],
  mvp_login_client.cpp [skeleton sem sockets]). SEM REDE; legado/evidências
  intactos. [LEDGER §45].
- **1.0-B (2026-09-04)**: **XOR32 real +
  builder C1 F1:01 (plain) implementados** — tabela kXorFilter32 extraída
  programaticamente da evidência `0E1L__10__StreamPacketEngine…h` (sha
  797d26d9…), cross-check GS `0C10A__PacketManager…cpp` **32/32 MATCH**;
  `BuildC1_F1_01_LoginRequestPlain` (49 B: BuxConvert ID/PASS em cópias,
  Tick LE, VerEnc[i]=v[i]-(i+1), Serial[16], XorData32 em [3..49),
  static_assert 49) encadeada em build_F1_01_login_request_packet;
  observação de fidelidade (spe.Send não chama End()) registrada no
  código+§46. Syntax-check **OK**. [LEDGER §46].
- **1.0-C (2026-09-04)**: **Wrap C3 implementado** —
  `crypto_cpacketmanager_min.h` (PacketCryptoSM: Encrypt/EncryptBlock/AddBits
  fiéis ao caminho clássico 8→11 B do PacketManager.cpp :236-:364/:430-:470;
  chaves injetáveis) + `BuildC3_F1_01_LoginRequestEncrypted` (serial substitui
  byte[1]; cifra [1..49)=48 B→66 B; 0xC1 descartado; envelope [C3][68][ct];
  static_asserts 49/66/<256). TODOs: caminho ≥701 DES, Decrypt (RX), chaves
  Enc1/Dec2 (1.0-D), padding de bloco parcial (hipótese zero-pad — login tem
  6 blocos exatos, não afetado). Syntax-check **OK**. [LEDGER §47].
- **1.0-D (2026-09-04)**: **chaves reais no
  lugar** — Enc1.dat/Dec2.dat encontrados no repo (54 B cada), copiados
  para `keys/` + KEYS_MANIFEST (também manifestados Enc2/Dec1 do servidor);
  loader fiel `crypto_sm_keyfile_loader.h` (LoadKey :147-:212: header
  4370/54 + 3 tabelas XOR m_SaveLoadXor) **validado contra os 4 arquivos
  reais**; **PROVA do pareamento**: Dec1.Key=Enc1.Key⁻¹ mod Modulus (e
  Enc2↔Dec2) — valida fórmula modular da 1.0-C; PacketCryptoSM ganha
  LoadKeysFromFile+loaded_ (Encrypt=-2 sem chaves; NEWERA_ALLOW_DUMMY_KEYS
  OFF); TryLoadLoginKeys + BuildC3 com erro claro. Syntax-check **OK**.
  [LEDGER §48].
- **1.0-E (2026-09-04)**: **RX completo** —
  Decrypt/DecryptBlock fiéis (:288-:314/:377-:425, quirk result<0
  preservado); remontagem C1 do plain + unXor REVERSO provados
  (SocketManager :301-:326; ExtractPacket/XorData :565/:579-:581 — size=49
  reconstruído valida obs. 1.0-B); `ParseC3Packet` (serial dentro do ct),
  serial com resync não-fatal, `DecodeAndParseMvpPacket` (C1/C3→F1:00 c/
  version rule / F1:01 Value / outros = não suportado sem falhar);
  `TryLoadReceiveKeys` (Dec2). TODO: C4, ≥701 DES, test-vectors (1.0-F).
  Syntax-check **OK**. [LEDGER §49].
- **1.0-F (2026-09-04)**: **golden test vectors
  gerados e PASSANDO** (GENVEC exit 0; determinístico, sem rede/sockets) —
  T1 (plain49/C3-68/roundtrip com Dec1 DERIVADO por inverso modular),
  R1 (F1:00 sim c/ Xor → RX completo), R2 (F1:01 estilo GS real SEM Xor →
  flag `streamXored`). **BUG REAL corrigido**: DecryptBlock avançava bitPos
  pelo retorno de AddBits (targetPos+size :469) em vez do `BitPos +=`
  explícito (:388-:391) — pego pelos vetores. Artefatos em
  `NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors/`. [LEDGER §50].
- **1.0-G (2026-09-05)**: **loopback local
  OK (exit 0)** — socket REAL somente em 127.0.0.1 (porta efêmera,
  hardcode): server stub enviou R1 → client decodificou (pipeline real,
  Dec2) → client enviou T1 (builder real, Enc1) → **server matched
  T1_expected byte-a-byte** → R2 → client decodificou **Value=1**.
  Artefatos: `loopback/{embedded_vectors.h (do golden 9d5e66ff…),
  loopback_harness.cpp, compile_and_run.sh, binário}`. MVP de login
  provado end-to-end local com regressão golden. [LEDGER §51].
- **1.1-A (2026-09-05)**: **spec normativa do
  MVP pós-login (0xF3 comuns 00/01/02/03/06/30/52) gerada** —
  `NEW_ERA_PROTOCOL_MVP_F3_COMMON_SPEC.md`: handlers/casts dos dois lados
  com linhas (server :955-:1034 CG*Recv; client :12942-:13038 Receive*);
  6 structs de response TODAS no WSclient.h (7/19/5/11/34/15 B); server
  PMSG_*_RECV [NOT RECOVERED]; direcionalidade duplex documentada.
  **Escolha 1.1-B: 0x00 CHARACTER LIST** (1º pacote pós-login; request
  vazio; response 7 B + slots variáveis a extrair). [LEDGER §52].
- **1.1-B (2026-09-05)**: **F3:0x00 CharList
  implementado (builder + parser), syntax-only OK** — em `mvp_login_client.cpp`
  (bloco 1.1-B): `BuildC1_F3_00_RequestCharListPlain` ([C1][04][F3][00] +
  Xor32 no subcode → 0x7A) · `BuildC3_F3_00_RequestCharListEncrypted` (Enc1,
  bloco parcial 3 B → **C3 13 B**) · `ParseC1_F3_00_CharacterListPlain`
  (header 7 B {sub,maxClass,moveCount,count}; slots de **33 B PROVADOS** —
  MAX_ID_SIZE=10/EQUIPMENT_LENGTH=17 defines; campos
  Index/ID[11]/Level(W)/CtlCode/Class/Equip[17]offset/GuildStatus;
  bounds-check; Index>4 aborta como :560). [LEDGER §54].
- **1.1-C (2026-09-05)**: **golden+loopback
  F3:0x00 OK (localhost, ambos exit 0)** — golden `test_vectors_f3_00/`
  (REQ C3 13 B; RESP C3 57 B stub 1 slot "Hero1"/345 GS-style; bloco parcial
  7 B exercitado/validado); loopback `loopback_f3_00/` (server matched REQ
  golden; client parse count=1/name/level). Core: +`outPlainC1` param,
  guard RX `<4` (bug latente: C1 de 4 B rejeitado), guard F1:01<5.
  **Binários removidos pós-sucesso** (workspace ~4,6 MB). [LEDGER §57].
- **1.1-D (2026-09-05)**: **F3:0x02 Delete
  completo (spec+impl+golden+loopback, exit 0×2)** — spec
  `NEW_ERA_PROTOCOL_MVP_F3_02_DELETE_SPEC.md` (request C1 25 B
  ID[10]+Resident[10] :389-:395; response 5 B result 1/0/3/2 :675-:693);
  builder C3 35 B (3 blocos cheios) + parser result; golden
  `test_vectors_f3_02/` + loopback `loopback_f3_02/` (server memcmp match;
  client result=1 SUCCESS). **Binários removidos** (workspace ~4,6 MB).
  [LEDGER §58].
- **1.1-E (2026-09-05) — FRONTEIRA DOCUMENTAL ATUAL**: **F3:0x30 Option
  completo (spec+impl+golden+loopback, exit 0×2)** — spec
  `NEW_ERA_PROTOCOL_MVP_F3_30_OPTION_SPEC.md` (request C1 34 B c/
  option[30] :1597-:1603; response PRECEIVE_OPTION 34 B c/ HotKey swap
  :9398); golden `test_vectors_f3_30/` + loopback `loopback_f3_30/`
  (hotKey0=0x1234/9 vazias/gameOption/qwer=100; REQ C3 57 B match; parcial
  de 1 B exercitado). ⭐Descobertas §60: **Send() default=FALSE ⇒ requests
  F3 reais vão C1 PLAIN** (C3 golden = convenção de teste); charlist real
  leva +byLanguage; delete real usa Resident[20]; F3:52 req = int32.
  Binários removidos. [LEDGER §60].
- **INFRA-1 (2026-09-05)**: infraestrutura upstream adicionada —
  `UPSTREAM_PIN.md` (pin wongddd/muonline@580472e; política raw+sha256, sem
  clone) · `UPSTREAM_INDEX.json` (18.372 entries, tree completa não-truncada,
  metadados) · `scripts/fetch_raw_and_verify.sh` (fetch raw@commit + verificação
  sha256). Repo NEW-ERA-KNOWLEDGE sincronizado via deploy key. [LEDGER §59].
- Microtestes POSTERIORES ao Ledger (alegados: 0C.4-J e séries 0D.1-A..G / 0E.1-A..I):
  **N/A — chat-only, não recuperáveis no workspace** (inventário: nenhum arquivo
  *0D*/*0E* existe; /tmp está fora do workspace e não qualifica como fonte).

## 4. Próximo Microteste Sugerido (suportado por pendências em arquivo)
1. **1.1-F — F3:0x52 MasterSkill** (request = int32 SkillNum :2286-:2292;
  response PMSG_ANS_MASTERLEVEL_SKILL 15 B :2860-:2868 — ambos já mapeados
  §52/§60). Baixo custo. Obs.: correção de formatos wire-real (C1 plain +
  byLanguage + Resident[20]) fica a comando explícito, se desejado.
2. **PacketManager: seeding de m_XorFilter[32] / LoadKey / Enc1-Dec2 server-side** —
   [LEDGER §15 item 2 :~521].
3. **H2 (Connection.cpp/wsProtocolCheck) e H3 (camada ativa em runtime na 55901)** —
   [LEDGER §14 :509-510 UNCERTAIN; §15 item 3].
4. H1 (filtro XOR builder↔servidor 32 B) permanece **UNCERTAIN em arquivo**
   [LEDGER §14 :508] — existe alegação chat-only de resolução (ver §6 abaixo),
   NÃO-CANÔNICA até auditoria do coordenador.
5. **Auditoria formal de 0C.6-F..O + 0C.4-I** (governança) — [LEDGER §15 item 8].

## 5. Pendências e Lacunas (fechamento de governança)
- Auditoria/classificação formais pendentes para 0C.6-F..O, packs e 0C.4-I
  [LEDGER §4 :133; §15 item 8; §17 :~553-556].
- CURRENT_STATE/MASTER_CHECKPOINT originais: [NOT RECOVERED] no workspace
  [LEDGER §0 :26; §18 :~566-567].
- Comandos verbatim 0C.1-0C.5-A e tabelas 157 rows: [PENDING — efêmeros]
  [LEDGER §18 :~563-568].
- Lacuna documental estrutural: tudo que ocorreu APÓS a criação do Ledger
  (2026-09-02) não tem registro em arquivo — ver seção CHAT-ONLY do bloco de append.

## 6. Fontes
1. NEW_ERA_RESEARCH_LEDGER.md (65.962 B / 594 L) — leitura integral.
2. NEW_ERA_PHASE_0A_REPOSITORY_AUDIT.md (49.701 B / 591 L) — integridade verificada
   (indexada no Ledger §6 :~291-301).
3. NEW_ERA_PHASE_0B_GAMELOOP_FPS_AUDIT.md (27.919 B / 318 L) — idem (§7 :~303-327).
