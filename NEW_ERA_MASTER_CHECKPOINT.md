# NEW-ERA — MASTER CHECKPOINT
> RECONSTRUÇÃO GERADA EM RESPOSTA (não é o original do coordenador — original AUSENTE).
> Somente evidência EM ARQUIVO do workspace. Sem rede, sem execução, sem escrita.

## 1. Fontes Usadas
| Arquivo | Tam. | Linhas | Uso |
|---|---|---|---|
| NEW_ERA_RESEARCH_LEDGER.md | 65.962 B | 594 | Fonte primária (§0-§20; âncoras: §4 :92-133, §5 :137-365, §8 :331-364, §9 :366-395, §10 :397-433, §11 :435-456, §12 :458-481, §13 :483-502, §14 :504-515, §15 :518-529, §16 :531-549, §17 :552-559, §18 :561-571, §20 :586-592) |
| NEW_ERA_PHASE_0A_REPOSITORY_AUDIT.md | 49.701 B | 591 | Relatório 0A (clone íntegro na época; 17 commits; inventário/arquitetura) |
| NEW_ERA_PHASE_0B_GAMELOOP_FPS_AUDIT.md | 27.919 B | 318 | Relatório 0B (game loop/FPS/timing do cliente) |
| NEW_ERA_PHASE_0C7A_GS_PROTOCOLCORE_SWITCH_CENSUS.md | — | — | Relatório 0C.7-A-NET (censo 107 cases; 2026-09-04) |
| NEW_ERA_EVIDENCE/GS_Protocol.cpp__580472e__sha256_e8bb4dbb.cpp | 135.014 B | 5.217 | Fonte do censo (sha256 e8bb4dbb… conferido) + .SHA256.txt |
Método: leitura integral por chunks + grep de âncoras de seção. Zero execução de código.
Ausências verificadas por find recursivo: CURRENT_STATE/MASTER_CHECKPOINT = 0 hits;
nenhum arquivo *0C*/*0D*/*0E* além dos indexados acima.

## 2. Snapshot Canônico Derivado (só evidência em arquivo)
- Objeto: MU Season 5 (EX502), base S8 rebaixada p/ S5, GameServer "Louis Emulator",
  cliente WebZen S5 [LEDGER §6 :~291-296].
- Commit canônico único de evidência: 580472e (main) [LEDGER §0].
- Topologia (CONFIRMED) [LEDGER §8.1 :~333-344]: Client→CS 44405 → Client→GS 55901;
  GSCS 55919 (SEM socket GS↔GSCS); GS→DS 55960; GS→JS 55970; GS/JS→CS UDP 55557;
  JS/DS→ODBC; NEW_PROTOCOL cobre SÓ Cliente↔GS.
- Protocolo consolidado (CONFIRMED) [LEDGER §9 :366-395]: C1 3B/C2 4B; C3/C4
  SimpleModulus (serial+opcode na cifra); BOTH_MESSAGE=0x000C; message_header 6 B
  little-endian sem hton; enum ProtocolHead 13 membros idêntico nos dois lados;
  PROTOCOL_CODE1/2/3=0xD4/0x11/0x15 (LANG=1), CODE4=0xDB uso UNCERTAIN.
- Dispatch (CONFIRMED) [§10-§11]: cliente TranslateProtocol :12821/switch :12823 —
  156 HeadCodes top-level + default (contagem canônica, triple cross-check);
  servidor ProtocolCore :76/switch :98 — **censo 0C.7-A-NET: 107 cases top-level,
  107 distintos, SEM default** (switch :98-1164; Pendência §15.1 fechada; H4 confirmado);
  **censo 0C.7-B: +26 sub-switches `lpMsg[3]` com 155 sub-opcodes (54 distintos globais,
  21 reutilizados entre HEADs, default único em 0xBF @656 com LOG)** — dispatch servidor
  mapeado por completo (262 labels totais no ProtocolCore);
  gateway global de envio Util.cpp:214-218 → todo outbound = BOTH_MESSAGE.
- Cripto efetiva no build (CONFIRMED) [§11 :~453-455; §5 0C.6-J/N/L]: transporte
  MORTA 2× (ENCRYPT_STATE=0 + MHPServer.ini inexistente); SimpleModulus só caminho
  clássico; XOR 32 B ativa c→s; BuxConvert campo-level; Themida "WL  " no-op sem
  protector; chaves repo CustomerName=MuOnline / ServerSerial=TbYehR2hFUPBKgZj.
- Game loop (CONFIRMED) [§7]: tick 40 ms/25 Hz ZzzScene.cpp:2320; limiter 25 FPS
  :2516-2524; 4 relógios; DeltaT código morto; rede frame-bound; CalcFPS display-only.
- SUPERSEDED preservados [§13]: 125→156 HeadCodes; BOTH_MESSAGE 0x000B→0x000C;
  head não-inicializado neutralizado; assimetria XOR → unidirecional por design.

## 3. Tabela de Microtestes (fonte: LEDGER §4 :97-133)
| ID | Status | Fonte | Observação |
|---|---|---|---|
| 0A | APROVADO | Ledger §4 + relatório [FILE] | inventário/arquitetura |
| 0B | APROVADO | Ledger §4 + relatório [FILE] | tick 25 Hz; limiter 25 FPS |
| 0C.1 | APROVADO | Ledger §4/§8.1 [MEM] | connection map |
| 0C.2 / 0C.3 / 0C.4-A..H | APROVADO | Ledger §4/§5/§8 [MEM] | recv/send/framing/ASIO |
| 0C.5-A/-B/-B-audit/-C | APROVADO/ENTREGUE | Ledger §4/§5/§10 | 156 top-level canônico |
| 0C.6-A/-A-2/-B/-C/-D/-E | APROVADO | Ledger §4/§5 | última aprovação formal = 0C.6-E (:133) |
| 0C.6-F/-F-v2/-G/-G-snip/-H/-GH-pack | EXECUTADO/ENTREGUE | Ledger §4/§5 | auditoria pendente |
| 0C.6-I/-I-pack/-J/-K/-L/-M/-M-pack/-N/-O/-O-pack | EXECUTADO/ENTREGUE | Ledger §4/§5 | auditoria pendente |
| 0C.4-I (+aux dump SMM) | EXECUTADO/ENTREGUE | Ledger §4 :~131-132/§5 :280 | fronteira em arquivo anterior |
| 0C.7-A (local) / 0C.7-A-CHAT (forense) | EXECUTADO/ENTREGUE | chat (sem arquivo) | pré-requisitos do NET; alvo GS ausente do workspace local |
| **0C.7-A-NET (+consolidação)** | **EXECUTADO/ENTREGUE — evidência EM ARQUIVO** | Ledger §22 + [FILE] census report + NEW_ERA_EVIDENCE/…cpp | 107 cases top-level, sem default; fecha §15.1; H4 confirmado; ERRATA BOTH_POSITION@110 |
| **0C.7-B** | **EXECUTADO/ENTREGUE — evidência EM ARQUIVO** | Ledger §23 + [FILE] relatório 0C7B | 26 sub-switches lpMsg[3] · 155 labels (=0C.7-A-NET, divergência zero) · 54 distintos globais · 21 repetidos · 1 default (0xBF @656 com LOG); fecha pendência §22 |
| 0E.1-J | EXECUTADO/ENTREGUE — evidência EM ARQUIVO | Ledger §24 + [FILE] relatório 0E1J | classificação (4) inconclusivo: 0 hits locais de MAX_MODELS; NÃO é build-define (vcxproj); raiz de source/ ausente do workspace |
| 0E.1-J-NET | EXECUTADO — BLOQUEADO (401), evidência EM ARQUIVO | Ledger §25 + [FILE] relatório 0E1JN + evidence 401.json | Code Search API exige auth → 0 paths/0 downloads; classificação (4) mantida; 3 caminhos de fechamento registrados |
| 0E.1-JN2 | EXECUTADO/ENTREGUE — evidência EM ARQUIVO | Ledger §26 + [FILE] relatório 0E1JN2 + 25 evidências raw | classe (4) c/ progresso: MAX_MODELS só como comentário (ZzzBMD.cpp:22); storage atual = ponteiro `BMD *Models` (:23/:334); alocação fora do conjunto; faltam 5-6 arquivos (JN3) |
| 0E.1-JN3 | EXECUTADO/ENTREGUE — evidência EM ARQUIVO | Ledger §27 + [FILE] relatório 0E1JN3 + 7 evidências raw | classe (2): MAX_MODELS ativo (ZzzOpenData.cpp:111/:113; Winmain.cpp:447) — É o limite real (`new BMD[MAX_MODELS+1024]` + rand()%1024); valor em header fora dos 7 (cand. _enum.h) → JN4 |
| **0E.1-JN4** | **EXECUTADO/ENTREGUE — classe (1), evidência EM ARQUIVO** | Ledger §28 + [FILE] relatório 0E1JN4 + _enum.h raw | **MAX_MODELS = 10002** (_enum.h:1046; cadeia resolvida, 612 membros, âncoras 6/6; `new BMD[11026]` geometria exata; (515)=stale; faixa c/ flags desconhecidos [10002,~10103]) — SÉRIE 0E.1-J ENCERRADA |
| 0E.1-K | EXECUTADO/ENTREGUE — evidência EM ARQUIVO | Ledger §29 + [FILE] relatório 0E1K | 1293 indexadores; CHECK SIM 3/NÃO 1286/INDET 4; wrapper AccessModel sem guard (×793 sites); ~272 sites dinâmicos sem guard (incl. escritas); Winmain:447 único guard MAX_MODELS |
| **0E.1-L** | **EXECUTADO/ENTREGUE — classe (3)+nuance(2), evidência EM ARQUIVO** | Ledger §30 + [FILE] relatório 0E1L + 23 evidências raw | o->Type = enums internos (:11219); rede 16-bit sem validação neutralizada por default-substitution (:13386); equipamento in-bounds por bit-width (≤5180); vetor ativo rede→OOB NÃO confirmado; fragilidade 0-guards permanece |
| 0E.1-M | EXECUTADO/ENTREGUE — classe (3), evidência EM ARQUIVO | Ledger §31 + [FILE] relatório 0E1M | 8 caminhos mapeados; P2/P3 provados in-bounds (máscaras/sentinelas); P1/P4/P5 pendentes de `ConvertItemType` (def fora do escopo); CreateItem:6145 sem sentinela é o elo crítico; sanitização explícita só em inventário |
| 0E.1-N | EXECUTADO — classe (3), evidência EM ARQUIVO | Ledger §32 + [FILE] relatório 0E1N + 10 evidências raw | def AUSENTE dos 68 arquivos; 39 candidatos/29 truncados (cap 10 lexical); suspeito #1 ZzzInventory.cpp; P1/P4/P5 seguem condicionais |
| 0E.1-N2 | EXECUTADO — classe (3), evidência EM ARQUIVO | Ledger §33 + [FILE] relatório 0E1N2 + 10 evidências raw | NOT FOUND: 6 calls/0 defs em 78 arquivos; alvo afunilado p/ headers compartilhados ausentes (UIManager.h; ZzzInfomation.h/.cpp — grafia fora das regex) |
| **0E.1-N3** | **EXECUTADO/ENTREGUE — classe (1), evidência EM ARQUIVO** | Ledger §34 + [FILE] relatório 0E1N3 + 3 evidências raw | **ConvertItemType def ZzzInfomation.cpp:371-377; range [0,8191] provado (13-bit, máx=0x1FFF)**; P1/P4/P5 in-bounds [1086..9277]<10002; **vetor rede→Models[] ENCERRADO** |
| **0C.8-A** | **EXECUTADO/ENTREGUE — classe (1), evidência EM ARQUIVO** | Ledger §35 + [FILE] relatório 0C8A | F3 duplex: server 22 (CG\* C→S) × client 23 (Receive\* S→C); comuns 7 · só-srv 15 · só-cli 16; 22×23 = coincidência numérica; pendência §23 ENCERRADA |
| **0C.9-A** | **EXECUTADO/ENTREGUE — classe (1), evidência EM ARQUIVO** | Ledger §36 + [FILE] relatório 0C9A (spec mínima) | 0xF1: server 3 (connect/close/vazio) × client 7 (join/login/logout/create/pwds); comuns {01,02,03}; assimetria direcional + multi-peer/fase; spec MVP pronta |
| **0C.9-B** | **EXECUTADO/ENTREGUE — classe (1), evidência EM ARQUIVO** | Ledger §37 + [FILE] relatório 0C9B (spec operacional) | Login 0xF1:0x01 cadeia completa C→GS→JS→GS→C; result↔Value 1:1 (0/1/2/3/4/6); 23 Value labels c/ default; JS:76-:254; GS senders :3132/:3149 |
| **0C.9-C** | **EXECUTADO/ENTREGUE — classe (1), evidência EM ARQUIVO** | Ledger §38 + [FILE] relatório 0C9C (spec 0x00 + request 0x01) | 0xF1:0x00 S→C 12 B fechado (struct+emissor GS :3071/:3099); request 0xF1:0x01 = macro SendRequestLogIn (BuxConvert+Tick+VerEnc+Serial16); fecha lacuna 3 do §37 |
| **0C.9-D** | **EXECUTADO/ENTREGUE — classe (1), evidência EM ARQUIVO** | Ledger §39 + [FILE] relatório 0C9D | GS: result 6 = version/serial, ABORTA pré-JS (:1970-:1980); anti-replay :1982; BuxConvert provado XOR 3B {FC,CF,AB} (WSclient.cpp:209-:215); PacketArgumentDecrypt def [NOT RECOVERED] |
| **0C.9-E** | **EXECUTADO/ENTREGUE — conclusão (3) NOT RECOVERED, evidência EM ARQUIVO** | Ledger §40 + [FILE] relatório 0C9E | PacketArgumentDecrypt: 3 CALLs, def 0 no workspace; BuxConvert 2ª def Encoder/Util.cpp:4-:12 (chave idêntica) ⇒ padrão codebase; equivalência segue HIPÓTESE FORTE |
| **0C.9-F** | **EXECUTADO/ENTREGUE — conclusão (1) FOUND+CONFIRMADO, evidência EM ARQUIVO** | Ledger §41 + [FILE] relatório 0C9F + evidência 0C9F__Util (sha 6271b7be…) | REDE 1 Contents+1/8 raw early-stop; PacketArgumentDecrypt GS Util.cpp:101-:109 = XOR 3B {FC,CF,AB} ≡ BuxConvert (3 defs idênticas); §39b/§40 PROMOVIDOS A PROVADO |
| **0C.10-A** | **EXECUTADO/ENTREGUE — classe (1) c/ ressalvas, evidência EM ARQUIVO** | Ledger §42 + [FILE] relatório 0C10A + 4 evidências 0C10A__* | C3/C4 ativo: cliente SendPacket→C3/C4+serial (inline:67-127), RX decrypt :11679-:11710; GS CPacketManager (modulo+DES), ENCRYPT_STATE gate Util:240-244, HackPacketCheck valida Encrypt per-packet; GS_Protocol:670/673 = falsos positivos |
| **0C.10-B** | **EXECUTADO/ENTREGUE — classe (1), evidência EM ARQUIVO** | Ledger §43 + [FILE] relatório 0C10B + 4 evidências 0C10B__* | spe.Send(TRUE)→C3 PROVADO; GS RECV SocketManager.cpp:271-371→ProtocolCore :882; DataSend serial :449/:462; **ENCRYPT_STATE=0** (stdafx.h:59); XorData 32B ≡ m_XorFilter GS (bônus) |
| **0C.11-A** | **EXECUTADO/ENTREGUE — SPEC NORMATIVA EM ARQUIVO** | Ledger §44 + [FILE] `NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md` | MVP login 0xF1 completo: fluxo 3-pacotes, layouts byte-level, stack 3-camadas de ofuscação, Values com emissão marcada, Implementation Notes |
| **1.0-A** (FASE 1) | **EXECUTADO — scaffold implementação** | Ledger §45 + [DIR] `NEW_ERA_IMPLEMENTATION/mvp_login/` (7 arq) | Scaffold MVP login (headers/BuxConvert/XorData32-placeholder/stub CPacketManager/skeleton); implementação scaffolding |
| **1.0-B** (FASE 1) | **EXECUTADO — implementação scaffolding** | Ledger §46 + [FILE] `crypto_xordata32.h`/`mvp_login_client.cpp` | kXorFilter32 REAL (extração da evidência, cross-check 32/32) + BuildC1_F1_01_LoginRequestPlain 49 B + static_assert; syntax OK; sem C3 ainda |
| **1.0-C** (FASE 1) | **EXECUTADO — implementação** | Ledger §47 + [FILE] `crypto_cpacketmanager_min.h`/`mvp_login_client.cpp` | PacketCryptoSM clássico 8→11 B fiel (Encrypt :236-:263/Block :327-:364/AddBits :430) + BuildC3 (serial@byte1, cifra 48→66 B, envelope C3 68 B); TODOs: ≥701 DES, Decrypt RX, chaves; syntax OK |
| **1.0-D** (FASE 1) | **EXECUTADO — implementação** | Ledger §48 + [FILE] `crypto_sm_keyfile_loader.h`/`keys/` (Enc1+Dec2+MANIFEST) | Loader fiel LoadKey :147-:212 (4370/54, XOR m_SaveLoadXor) validado ×4 arquivos reais; pareamento PROVADO por inverso modular (§48); builder C3 exige chaves reais; syntax OK |
| **1.0-E** (FASE 1) | **EXECUTADO — implementação** | Ledger §49 + [FILE] `crypto_cpacketmanager_min.h`/`mvp_login_client.cpp` | RX: Decrypt/DecryptBlock fiéis (:288-:425, quirk); remontagem C1+unXor reverso (SocketManager :301-:326, XorData :579-:581); serial resync não-fatal; F1:00 (version rule) + F1:01 (Value); TODO C4/DES; syntax OK |
| **1.0-F** (FASE 1) | **EXECUTADO — test vectors PASSANDO** | Ledger §50 + [DIR] `test_vectors/` (gen+golden) | GENVEC exit 0: T1 roundtrip c/ Dec1-derivado (inv. modular §48), R1 F1:00 sim, R2 GS-real sem Xor (flag streamXored); **bug DecryptBlock (AddBits return vs BitPos+=) pego e corrigido** |
| **1.0-G** (FASE 1) | **EXECUTADO — loopback OK (exit 0)** | Ledger §51 + [DIR] `loopback/` (embedded+harness+script+binário) | Socket real 127.0.0.1 only: R1→decode→T1(builder real)→**memcmp match golden**→R2→Value=1; end-to-end local do MVP login |
| **1.1-A** (FASE 1) | **EXECUTADO — SPEC EM ARQUIVO** | Ledger §52 + [FILE] `NEW_ERA_PROTOCOL_MVP_F3_COMMON_SPEC.md` | F3 comuns 00/01/02/03/06/30/52: handlers/casts 2 lados c/ linhas; 6 structs response no WSclient.h; server PMSG_* [NOT RECOVERED]; **próximo: 0x00 CharList** |
| **1.1-B** (FASE 1) | **EXECUTADO — IMPLEMENTADO (syntax-only OK)** | Ledger §54 + mvp_login_client.cpp (bloco 1.1-B) | Builder C→S F3:00 ([C1][04][F3][00]+Xor→0x7A; C3 13 B c/ bloco parcial) + parser S→C (header 7 B + slots 33 B **provados** por defines MAX_ID=10/EQUIP=17; Index>4 aborta :560) |
| **1.1-C** (FASE 1) | **EXECUTADO — GOLDEN+LOOPBACK OK (exit 0×2)** | Ledger §57 + [DIR] `test_vectors_f3_00/` + [DIR] `loopback_f3_00/` | REQ C3 13 B golden (builder real) + RESP C3 57 B (stub parseado count=1 "Hero1"/345); server memcmp match; **bloco parcial 7 B validado**; bug RX `<5`→`<4` corrigido; binários pruned |
| **1.1-D** (FASE 1) | **EXECUTADO — SPEC+IMPL+GOLDEN+LOOPBACK OK (exit 0×2)** | Ledger §58 + [FILE] `NEW_ERA_PROTOCOL_MVP_F3_02_DELETE_SPEC.md` + [DIR] `test_vectors_f3_02/` + [DIR] `loopback_f3_02/` | F3:0x02 Delete: REQ C1 25 B (ID[10]+Resident[10] :389-:395)→C3 35 B (3 blocos cheios); RESP 5 B result 1=SUCCESS (:675-:693)→C3 13 B; golden+loopback match; binários pruned |
| **1.1-E** (FASE 1) | **EXECUTADO — SPEC+IMPL+GOLDEN+LOOPBACK OK (exit 0×2)** | Ledger §60 + [FILE] `NEW_ERA_PROTOCOL_MVP_F3_30_OPTION_SPEC.md` + [DIR] `test_vectors_f3_30/` + [DIR] `loopback_f3_30/` | F3:0x30 Option: REQ/RESP 34 B espelhados (option[30] :1597-:1603); HotKey swap :9398 (**bug próprio do parser pego pelos asserts**); C3 57 B c/ parcial 1 B; **descobertas: Send()=FALSE ⇒ wire C1 plain; byLanguage; Resident[20]** |
| **1.1-F** (FASE 1) | **EXECUTADO — SPEC+IMPL+GOLDEN+LOOPBACK OK (exit 0×2)** | Ledger §61 + [FILE] `NEW_ERA_PROTOCOL_MVP_F3_52_MASTERSKILL_SPEC.md` + [DIR] `test_vectors_f3_52/` + [DIR] `loopback_f3_52/` | F3:0x52: REQ **wire-real C1 plain 8 B** (int32 :2286-:2292; 1º golden wire-format) + RESP 15 B (:2860-:2868)→C3 24 B; **fix core unXor p/ C1 direto (regressão 3× verde)** |
| **1.2-A1** (FASE 1) | **EXECUTADO — WIRE-REAL FIX OK (exit 0×2)** | Ledger §62 + [FILE] `NEW_ERA_PROTOCOL_MVP_F3_00_CHARLIST_SPEC.md` + [DIR] `test_vectors_f3_00/`+`loopback_f3_00/` (regen C1) | F3:00 wire = **C1 5 B c/ byLanguage** (:288-:296 Send()=FALSE); golden/loopback C1↔C1 sem crypto; builders C3 DEPRECATED |
| **1.2-A2** (FASE 1) | **EXECUTADO — WIRE-REAL FIX OK (exit 0×2)** | Ledger §63 + [FILE] `NEW_ERA_PROTOCOL_MVP_F3_02_DELETE_WIREREAL_SPEC.md` + [DIR] `test_vectors_f3_02/`+`loopback_f3_02/` (regen C1) | F3:02 wire = **C1 34 B c/ Resident[20]** (:310-:320; struct dizia 10 — divergência documentada); golden/loopback C1↔C1 sem crypto; builders antigos DEPRECATED |
| **1.2-A3** (FASE 1) | **EXECUTADO — WIRE-REAL FIX OK (exit 0×2)** | Ledger §64 + spec F3:30 §6 WIRE-REAL + [DIR] `test_vectors_f3_30/`+`loopback_f3_30/` (regen C1) | F3:30 wire = **C1 34 B option[30]** (:1597-:1603); golden/loopback C1↔C1 sem crypto; **migração wire F3 completa (00/02/30/52)** |
| (pós-Ledger, alegados) 0C.4-J, 0D.1-A..G, 0E.1-A..I | NÃO-CANÔNICO (chat-only) | — | ver append; 0 arquivos no workspace |
| Contagem switch(head) servidor | PENDENTE | Ledger §15.1/§10.4 | candidato natural |
| H1 | UNCERTAIN (em arquivo) | Ledger §14 :508 | alegação de resolução é chat-only |
| **H4** | **CONFIRMED (0C.7-A-NET)** | Ledger §22 + census report :771-773 | `case PROTOCOL_CODE4:` existe no servidor → CGMultiSkillAttackRecv (cliente não o tem) |
| H2, H3, H5, H6 | UNCERTAIN | Ledger §14 :509-513 | — |
| Fases 0D-0L (exceto alegado 0D/0E chat-only) | NÃO INICIADO | Ledger §16 :531-549 | — |

## 4. Principais Descobertas CONFIRMED (evidência em arquivo)
Ver §2 (snapshot) — íntegro nos relatórios 0A/0B e seções §5-§13 do Ledger.
Destaques: gateway BOTH_MESSAGE global; cripto de transporte duplamente morta;
XOR 32 B unidirecional; 156 HeadCodes; riscos §12 (bounds-check ausente BOTH_MESSAGE
cliente/servidor, header.size u32 sem teto, little-endiano, buffers static, leak fila ASIO).
- (0E.1-J, evidência local em arquivo) `MAX_MODELS` não é definido pelo build do
  cliente (Main.vcxproj Debug/Release sem o símbolo) nem existe em nenhum arquivo
  local; nova evidência de divergência workspace↔commit 580472e
  (`_TextureIndex.h`: raiz local 23.257 B ≠ `source/` no GitHub 18.779 B).
- (0E.1-JN2, evidência em arquivo @580472e) o array estático `BMD Models[MAX_MODELS]`
  tornou-se HISTÓRICO (comentado, ZzzBMD.cpp:22): o storage atual de modelos é
  ponteiro dinâmico `BMD *Models` (ZzzBMD.cpp:23; extern ZzzBMD.h:334) — o limite
  real é imposto no site de alocação (`new BMD[...]`), fora do conjunto JN2
  (provável ZzzOpenData.cpp [§21.1 chat-only]). Limites-irmãos em ZzzBMD.h:
  MAX_BONES 200 / MAX_MESH 50 / MAX_VERTICES 15000 / MAX_MONSTER_SOUND 10.
- (0E.1-JN3, evidência em arquivo @580472e) **alocação de Models confirmada**:
  `OpenPlayers()` ZzzOpenData.cpp:110-113 — `new BMD[MAX_MODELS+1024]` com base
  deslocada `rand()%1024` (anti-tamper, padrão *Dump: ModelsDump,
  CharacterMemoryDump, ItemAttRibuteMemoryDump, RendomMemoryDump) e
  `ZeroMemory(Models, MAX_MODELS*sizeof(BMD))`; MAX_MODELS é o limite REAL do
  cliente (classe (3) refutada) e permanece definido fora dos 7 arquivos
  (candidato `_enum.h`; família MODEL_* encadeada/condicional em _define.h:
  MAX_CLASS 7/6 e MODEL_BODY_NUM 24/20 via PBG_ADD_NEWCHAR_MONK). Sem
  bounds-check de índice de model nos 7 (risco §12).
- (0E.1-JN4, evidência em arquivo @580472e) **MAX_MODELS = 10002** — definido em
  `_enum.h:1046` como `(MODEL_MASK_HELM+MAX_ITEM_INDEX)`, resolvido por cadeia
  mecânica completa (MAX_ITEM_INDEX 512; MAX_ITEM 8192=16×512; MODEL_ITEM 1086;
  MODEL_PLAYER 1078; MODEL_LOGO 160). Alocação real: `new BMD[11026]`, janela
  útil 10002 com offset anti-tamper ≤1023 — encaixe exato (folga 1 slot).
  Comentário autoral "// (515)" (:1001) provavelmente STALE (515 < mínimo
  aritmético). Série 0E.1-J encerrada: J→JN(401)→JN2(ponteiro)→JN3(alocação)→JN4(valor).
- (0E.1-K, evidência em arquivo @580472e) **bounds-check de Models[] é
  praticamente inexistente no cliente**: 1293 indexadores, apenas 3 guards
  (Winmain:447 `i<MAX_MODELS` — o único com MAX_MODELS — + 2 loops de teardown
  por constantes); wrapper AccessModel (LoadData.cpp:22-43) sem guard some por
  793 call-sites; ~272 sites com índice dinâmico sem guard, incluindo ESCRITAS
  (ZzzCharacter:12948-52 `Models[c->Object.Type].Meshs[n]…`; SMD:221
  `strcpy(Models[ID].Name,…)`); OOB positivo ≥10002 cai na folga anti-tamper
  não-zeroed (acesso silencioso) e ≥11026+offset corrompe heap. Origem de
  `Type` (rede?) fora do escopo — vetor IV permanece HIPÓTESE (0E.1-L).
- (0E.1-L, evidência em arquivo @580472e) **resposta à hipótese IV**: `OBJECT::Type`
  NÃO recebe bytes de rede — sempre enums (ZzzCharacter:11219); o seletor de rede
  16-bit sem validação (WSclient 0x13/0x1F/0x45/Preview) é neutralizado por
  **default-substitution** no switch de CreateMonster (`default: OpenMonsterModel(0)`,
  ZzzCharacter:13386 — Type desconhecido → modelo 0); equipamento é a única injeção
  R indexável e é in-bounds por construção (ExtType 12-bit + sentinela 0x0FFF →
  Weapon.Type ≤ 5180 < 10002). Consequência para o §29: o risco é de FRAGILIDADE
  (0 guards em 1293 indexadores), não de vetor ativo confirmado.
- (0E.1-N3, evidência em arquivo @580472e) **vetor rede→Models[] ENCERRADO com
  prova aritmética**: `ConvertItemType` (ZzzInfomation.cpp:371-377) retorna
  [0, 8191] (13-bit: Item[0] + bit7 de Item[3]<<8 + nibble alto de Item[5]<<9;
  máx = 0x1FFF, idem sentinela dos receivers) ⇒ `MODEL_ITEM/MODEL_SWORD+Type`
  ∈ [1086..9277] < MAX_MODELS=10002 (folga 725; lattice fecha em MODEL_HELM2=
  9278). Todos os caminhos R (monstros/equip/itens) são in-bounds estruturais
  ou neutralizados por default-substitution.
- (0C.8-A, evidência em arquivo) **F3 reconciliado**: o head 0xF3 é namespace
  duplex — servidor despacha requests do cliente (22 CG\*Recv, lpMsg[3],
  Protocol.cpp:955-:1034) e cliente despacha respostas/pushes (23 Receive\*,
  SubCode via struct, WSclient:12942-:13038); 7 pares compartilham subcode;
  a diferença 22×23 não é anomalia (conjuntos direcionais distintos).
- (0C.9-A, evidência em arquivo) **0xF1 reconciliado com spec mínima**: server
  3 subcodes (connect-account 0x01 / close-client 0x02 / 0x03 vazio) × client
  7 (join 0x00 / login 0x01 com enum Value de ~20 códigos / logout 0x02 /
  create-account 0x12 / passwords 0x03-0x05); namespace multi-peer (cliente =
  fase Connect/JoinServer; GS = in-game) — subcodes reutilizados entre peers
  com semânticas distintas (0x03 ConfirmPassword × vazio). Primeira spec MVP
  NEW-ERA de protocolo emitida (login result codes documentados).
- (0C.9-B, evidência em arquivo) **fluxo de login totalmente provado
  ponta-a-ponta**: request 0xF1:0x01 (conta/senha cifradas) → GS valida
  versão/serial (result 6) → JoinServer decide (0 senha · 1 ok · 2 conta · 3
  já-conectado · 4 cheio) → GS envia `[C1][sz][F1][01][Value]` → cliente
  (23 labels; default = FAIL_CONNECT). Result do JS = Value do cliente,
  byte a byte, nos códigos núcleo. ConnectServer fora do fluxo; GameServer
  do repo local está vazio (fonte canônica = evidência @580472e).
- (0C.9-C, evidência em arquivo) **spec 0xF1:0x00 e request 0xF1:0x01
  fechados em arquivo**: handshake S→C de 12 B com struct dedicada
  (Result/HeroKey/Version[5]) emendado ao emissor GS (GCConnectClientSend*
  espelham a struct campo a campo); e o request de login revelado como
  macro `SendRequestLogIn` (wsclientinline.h:243) — ID/senha via BuxConvert,
  TickCount, Version ofuscada por (i+1) e Serial[16] no fim do pacote
  (≈49 B). Trinca de login do 0xF1 documentada ponta a ponta.
- (0C.9-D, evidência em arquivo) **provado com chave**: o ofuscador de
  credenciais do login é `BuxConvert` = XOR cíclico de 3 bytes com chave
  estática {0xFC,0xCF,0xAB} (WSclient.cpp:209-:215) — o server-side
  (`PacketArgumentDecrypt`, 3 chamadas mapeadas) o desfaz [equivalência
  HIPÓTESE FORTE, def não recuperada]; e result 6 (versão/serial) é
  decidido no GS ANTES do JoinServer, com guard anti-replay
  (LoginMessageSend). H1 “XOR 32B” não se aplica a credenciais.
- (0C.9-F, evidência em arquivo) **equivalência provada com chave pública**:
  `PacketArgumentDecrypt` (GS Util.cpp:101-:109, recuperado via rede
  autorizada com early-stop no 1º download) = XOR cíclico 3 B
  `{0xFC,0xCF,0xAB}` — idêntico byte a byte às duas defs de `BuxConvert`
  (cliente WSclient.cpp:209-:215 e tool Encoder/Util.cpp:4-:12). Três
  definições independentes, uma única transform: a camada de ofuscação de
  credenciais do login está 100% especificada e implementável sem risco.
- (0C.10-A, evidência em arquivo) **transporte C3/C4 é load-bearing**:
  cliente carrega chaves SimpleModulus (Enc1.dat/Dec2.dat) incondicionalmente
  no boot, mantém serial TX/RX por pacote e reporta hacking em falha de
  decrypt; GS tem motor de módulo completo (CPacketManager; +DES XEX3 em
  builds ≥701), gate compile-time ENCRYPT_STATE e validação per-packet de
  criptografia (HackPacketCheck). C3/C4 não é opcional no protocolo — é
  parte do envelope que o MVP precisa falar.
- (0C.10-B, evidência em arquivo) **ponta a ponta do C3 fechado com
  números**: login request sai como C3 (decisão por tamanho no SendPacket,
  <256 B); GS recebe em SocketManager.cpp, decripta via CPacketManager e
  entrega ao ProtocolCore com flag encrypt + serial; S→C re-encripta C3/C4
  com serial por conexão. **ENCRYPT_STATE=0** (stdafx.h) — a camada extra
  de whitening está DESLIGADA neste build; a cripto real é a de módulo do
  CPacketManager. E o stack de ofuscação do login tem 3 camadas provadas:
  BuxConvert (XOR 3B) → XOR encadeado de stream (tabela 32 B idêntica nos
  dois lados) → SimpleModulus (C3).
- (0C.11-A) **Spec MVP login pronta**: `NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md`
  transforma oito microtestes (§36-§43) em um único documento normativo
  implementável — fluxo 0xF1 ponta a ponta com layouts byte-level, stack
  de cripto na ordem exata, tabela de Values com emissão marcada por build
  e notas claras de reproduzir/stub/abertos. FASE 0 produziu sua primeira
  deliverable de implementação.

## 5. Riscos e Pendências
- Do legado: tabela completa em [LEDGER §12 :458-481] (CONFIRMED + PROBABLE +
  HYPOTHESIS/PENDING).
- De processo: §15 itens 1-8 (pendências técnicas + auditoria formal);
  §18 (lacunas de recuperação); §19 (regras de integridade do documento vivo).
