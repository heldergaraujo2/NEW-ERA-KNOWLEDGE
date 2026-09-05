# NEW-ERA SERVERS — PHASE 0B: CLIENT GAME LOOP / FPS / TIMING — DEEP AUDIT
### Auditoria somente-leitura • Base: `wongddd/muonline` @ `580472e` (main) • Data: 2026-09-01

> Todos os caminhos relativos a `Source Client/source/`. Linhas confirmadas por leitura direta do código.
> Repositório permaneceu **intacto** (`git status --short` vazio antes e depois desta fase).

---

## 1. PONTO DE ENTRADA — Fluxo real desde WinMain

`WinMain()` — `Winmain.cpp:1392`

```
WinMain (Winmain.cpp:1392)
├─ new CProtect; CProtect::ReadMainFile("Main.info")   → aborta (MessageBox+ExitProcess) se ausente
├─ leaf::AttachExceptionHandler(ExceptionCallback)      → crash handler global
├─ OpenMainExe()                                        → validações do próprio EXE
├─ g_SimpleModulusCS.LoadEncryptionKey("Data\\Enc1.dat") / LoadDecryptionKey("Data\\Dec2.dat")
├─ OpenInitFile()                                       → config.ini (aborta se falhar)
├─ new CMultiLanguage
├─ EnumDisplaySettings/ChangeDisplaySettings            → fullscreen 16/24/32bpp
├─ StartWindow() → g_hWnd                               → janela Win32
├─ CreateOpenglWindow()                                 → ChoosePixelFormat + wglCreateContext (Winmain.cpp:877-893)
│   └─ InitBuiltinShaders()                             → se falhar: "fixed-function only" (Shader.cpp é stub vazio)
├─ CreateFont×4 (tamanho por resolução)
├─ CInput::Instance().Create(g_hWnd,...)                → input
├─ g_pNewUISystem->Create()                             → NewUI
├─ wzAudioCreate() / InitDirectSound()                  → áudio (música/efeitos)
├─ SetTimer(g_hWnd, HACK_TIMER, 20*1000)                → timer anti-hack 20 s (WM_TIMER)
├─ alocações de pools: CharactersClient[MAX_CHARACTERS_CLIENT+1+128], ItemAttribute[MAX_ITEM+1024], SkillAttribute[MAX_SKILLS], GateAttribute
├─ new CChatRoomSocketList / CUIManager / CUIMapName / CTimer  → g_pTimer (QPC)
├─ BuffSystem/MapProcess/PetProcess/ CUIMng::Create()
└─ (/test: força 127.0.0.1:44405, carga adiantada, SceneFlag=LOG_IN_SCENE)

while(1)  ← LOOP PRINCIPAL (Winmain.cpp:1758)
├─ PeekMessage(PM_NOREMOVE) → GetMessage/Translate/Dispatch
│   └─ WndProc (Winmain.cpp:~500-860): WM_MOUSEMOVE/LBUTTONDOWN/... → globals MouseX/Y/botões
│       WM_CHAR/WM_SYSKEYDOWN → input de texto;  WM_TIMER (HACK 20s);
│       WM_ASYNCSELECTMSG (597): FD_READ → SocketClient.nRecv(); FD_CLOSE → disconnect
└─ else (sem mensagens) → Scene(g_hDC)
    depois do Scene, sempre: ProtocolCompiler() / ChatRoomSocketList->ProtocolCompile()
    / [NEW_PROTOCOL_SYSTEM] gProtocolSend.RecvMessage()
```

## 2. GAME LOOP REAL (funções reais, com linha)

```
WinMain (Winmain.cpp:1758, while(1))
 ↓ (sem mensagens pendentes)
Scene(HDC)                          [ZzzScene.cpp:2828]  — switch(SceneFlag)
 ↓ LOG_IN/CHARACTER/MAIN
MainScene(HDC)                      [ZzzScene.cpp:2314]
 ├─ CalcFPS()                       [ZzzAI.cpp:781]  — WorldTime=timeGetTime(); difTime(ms); FPS; DeltaT(morto)
 │   └─ gSkillManager.CalcSkillDelay(difTime)        [SkillManager.cpp:155] — cooldowns em ms REAIS
 ├─ LOOP DE LÓGICA (passo fixo 40 ms):
 │   for(Remain = TimeRemain; Remain >= 40; Remain -= 40)   [ZzzScene.cpp:2320]
 │   ├─ g_pNewKeyInput->ScanAsyncKeyState()          — poll de teclado (25 Hz)
 │   ├─ [só login/char] CInput::Update() + CUIMng::Update(Δ QPC, clamp 200 ms)  [ZzzScene.cpp:2324-2331]
 │   ├─ NewMoveLogInScene / NewMoveCharacterScene / MoveMainScene
 │   │    └─ MoveMainScene                            [ZzzScene.cpp:1892]
 │   │        ├─ g_pPartyManager->Update(); g_pNewUISystem->Update(); MoveInterface()
 │   │        ├─ MoveObjects; MoveItems; MoveLeaves; MoveBoids/Fishs/Bugs; MoveChat
 │   │        ├─ MoveHero                             [ZzzInterface.cpp:7157] — input→ação/path
 │   │        ├─ MoveCharactersClient                 [ZzzCharacter.cpp:6317]
 │   │        │    └─ MoveCharacterClient (:6292) → MoveCharacter (:3836)
 │   │        │         ├─ AttackTime++ / Freeze-=0.03f / Dead++ (contagens POR TICK)
 │   │        │         └─ AnimationCharacter (:3489) → CharacterAnimation (:2280)
 │   │        │              └─ BMD::PlayAnimation   [ZzzBMD.cpp:408]: AnimationFrame += PlaySpeed
 │   │        ├─ UpdatePets; MovePoints; MovePlanes; MoveEffects; MoveJoints; MoveParticles
 │   │        └─ g_Direction.CheckDirection()
 │   ├─ g_PhysicsManager.Move(0.005f) ×5              [ZzzScene.cpp:2353; PhysicsManager.cpp:1207]
 │   ├─ MoveNotices; ChatTime--; MacroTime--
 │   ├─ WaterTextureNumber=(+1)%32                    — animação de água por tick
 │   └─ MoveSceneFrame++                              — RELÓGIO DE LÓGICA (ver §4)
 ├─ (por frame, fora do loop de tick):
 │   Bitmaps.Manage(); Set3DSoundPosition(); glClearColor/glClear
 │   TimePrior = GetTickCount()                       [ZzzScene.cpp:2452]
 │   RenderMainScene()                                [ZzzScene.cpp:2067]
 │     ├─ MoveMainCamera()  ← ÚNICO sistema atualizado POR FRAME (câmera livre)
 │     ├─ BeginOpengl; CreateFrustrum; CreateScreenVector
 │     └─ mundo/objetos/personagens/efeitos/UI (immediate mode)
 │   g_PhysicsManager.Render(); overlay "FPS : %.1f"  [ZzzScene.cpp:2484-2496]
 │   glFlush(); SwapBuffers(hDC)                      [ZzzScene.cpp:2511-2514]
 ├─ FPS LIMITER:  DifTimer = TimePrior - LastTimeCurrent;
 │   if (DifTimer < 40) { sleep_for(40-DifTimer); TimePrior += Δ; DifTimer = 40; }   [2516-2524]
 └─ TimeRemain = DifTimer + Remain                    — ACUMULADOR de tick p/ próximo frame [2819]
 ↓ (de volta ao while principal)
ProtocolCompiler(...)               [WSclient.cpp:11649]  — drena fila de rede clássica
g_pChatRoomSocketList->ProtocolCompile()
gProtocolSend.RecvMessage()         — drena fila ASIO (NEW_PROTOCOL_SYSTEM)
 → repete
```

**Frequência do loop:** 1 iteração = 1 frame ≥ 40 ms (limiter) → **25 FPS máx**; mensagens Windows bombeadas a cada iteração (e o ramo `else` só roda quando *não* há mensagens — janela arrastada = pausa no jogo).

## 3. TIMING — inventário de mecanismos

| Mecanismo | Onde | Variável | Unidade | Frequência | Consumidores |
|---|---|---|---|---|---|
| `GetTickCount` | ZzzScene.cpp:2312/2452/2516 | `TimePrior` (global) | ms | 1×/frame | Limiter 40 ms + acumulador `TimeRemain` (61 refs no cliente) |
| `timeGetTime` | ZzzAI.cpp:781-812 (CalcFPS) | `WorldTime`, `FPS`, `difTime`, `DeltaT` | ms | 1×/frame | `WorldTime`→efeitos/iluminação (`sinf(WorldTime*k)` em ZzzScene.cpp:1150+); `difTime`→`CalcSkillDelay` (ms reais); `DeltaT`→**ninguém (código morto)**; 84 refs |
| `QueryPerformanceCounter` | Time/Timer.cpp (CTimer) | `m_pcTimerStart` | s (double) | por chamada | `CInput` (dAbsTime), `CUIMng::Update` (login/char, clamp 200 ms) — 8 refs |
| `std::chrono::sleep_for` | ZzzScene.cpp:2520 | — | ms | pós-swap | **FPS limiter** (precisão de sleep ~ms → jitter) |
| `sleep_for`/chrono | ProtocolSend/ProtocolAsio | — | ms | reconexão | waits de rede |
| `Sleep()` | DSplaysound.cpp:395 (10 ms, loader), GameShop/ShopList.cpp (100 ms, polling) | — | ms | — | carregamento/IO |
| `SetTimer`/`WM_TIMER` | Winmain.cpp:563 (HACK_TIMER 20 s; WINDOWMINIMIZED_TIMER 1 s) + 15 refs UI | — | s | evento | anti-hack, minimize, UI |
| `MoveSceneFrame` | ZzzScene.cpp:2357 (`++` por tick) | global int | ticks (40 ms) | 25 Hz | `MoveSceneFrame % N == 0` — CSWaterTerrain.cpp:41 (%40), GMBattleCastle.cpp:733/797/844 (%20/%60/%10), GMHellas.cpp:317+ (%100/%50), CSPetSystem.cpp:570 (%60)… |
| `WaterTextureNumber` | ZzzScene.cpp:2355 | global int | ticks %32 | 25 Hz | água (ZzzBMD.cpp:956, GMHellas, GMBattleCastle) |
| Contadores por tick | ZzzCharacter.cpp:3866+ (`AttackTime++`, `Freeze-=0.03f`, `Dead++` ≥15) | campos CHARACTER | ticks | 25 Hz | velocidade de ataque, morte |
| `CQueueTimer`-like | — (cliente não usa Timer Queue; isso é GameServer) | — | — | — | — |

## 4. OS 40 ms — classificação: **(B) FIXED TIMESTEP PARCIAL (quase global)**

- **Definição:** literal `40` em ZzzScene.cpp:2320 (tick) e 2518-2523 (limiter). Nenhuma constante nomeada — o "40" está **hardcoded em 2 lugares**.
- **Dentro do tick de 40 ms (25 Hz):** input-poll (`ScanAsyncKeyState`), UI de jogo (`g_pNewUISystem->Update`, `MoveInterface`), **movimento** (`MoveHero`/`MoveCharactersClient`/`MoveCharacter`), **animação** (`AnimationCharacter`→`PlayAnimation`), **física** (cloth ×5 substeps), **efeitos/partículas/joints/planos**, pets, IA de mapas (`MoveBoids` etc.), chat visual, `MoveSceneFrame`/`WaterTextureNumber`/`ChatTime`/`MacroTime`, ataques (`AttackTime`).
- **Fora do tick (por frame):** render completo, `MoveMainCamera` (câmera), `Bitmaps.Manage()`, `Set3DSoundPosition()`, overlay de FPS, `glFlush`+`SwapBuffers`, limiter.
- **Tempo real (independente de tick E frame):** `CalcSkillDelay(difTime_ms)` (cooldowns), `CTimer`/QPC (UI login/char), `WorldTime` (efeitos cênicos), Windows timers, rede (eventos por mensagem).
- **Acumulador:** `TimeRemain` carrega sobra/ms não consumidos entre frames; frames lentos executam múltiplos ticks (`DifTimer=100` → 2 ticks + sobra 20). **O render NÃO depende do tick para rodar; o tick é que é quantizado em 40 ms.**

## 5. FPS — CalcFPS e consumidores

`CalcFPS()` [ZzzAI.cpp:781-817]: `frame++` por chamada; `FPS = frame/((t-start)/CLOCKS_PER_SEC)` — **bug de unidade** (`timeGetTime` ms ÷ `CLOCKS_PER_SEC`): o divisor fica ~1000× menor... compensado parcialmente pelos resets de janela de 2 s; o número exibido é **impreciso**. `WorldTime = timeGetTime()` (ms absolutos). `DeltaT` calculado e **jamais consumido** (grep: só ZzzAI.cpp — código morto). Exibição: overlay `"FPS : %.1f"` em MainScene [ZzzScene.cpp:2484-2496] — bloco duplicado (um incondicional + um `#if _DEBUG`). **FPS não influencia lógica, movimento, animação, render ou rede** — é só display. Único contador de FPS do cliente.

## 6. FPS LIMITER (o coração da limitação)

- **Implementação:** inline em `MainScene`, ZzzScene.cpp:2516-2524, imediatamente após `glFlush(); SwapBuffers(hDC);`.
- **Mecanismo:** `std::this_thread::sleep_for(milliseconds(40 - DifTimer))` — **sleep simples** (não busy-wait, não timer de alta resolução, não VSync). `DifTimer = TimePrior - LastTimeCurrent` (Δ entre `GetTickCount` de frames consecutivos, medidos **antes** do render... ver nota¹).
- **Alvo:** 40 ms/frame = **25 FPS máx** (sem nome de constante; `#if LDS_UNFIXED_FIXEDFRAME_FORDEBUG` na região indica que os autores sabiam que é um "fixed frame" forçado).
- **Escopo:** trava **o frame inteiro** — como render, tick de lógica e drenagem de rede acontecem no mesmo `while(1)`, tudo fica preso ao mesmo teto.
- **Fluxo real:** `Render → glFlush → SwapBuffers → sleep(40-Δ) → TimePrior+=Δ → (fim de MainScene) → ProtocolCompiler/RecvMessage → while(1) → Scene → ...`

¹ **Detalhe sutil:** `TimePrior = GetTickCount()` acontece **antes** do render (linha 2452) e `LastTimeCurrent = TimePrior` guarda o frame anterior → `DifTimer` mede o tempo **entre inícios consecutivos de frame** (inclui render do frame anterior + rede + mensagens), não o tempo do render atual.

## 7. VSYNC — **AUSEENTE**

- `wglSwapIntervalEXT` / `SwapInterval` / `WGL_EXT_swap_control`: **0 ocorrências** no fonte do cliente (GLEW está no diretório mas `glewInit` não é chamado; `wglGetProcAddress` só para `wglChoosePixelFormatARB` — FSAA, ZzzOpenglUtil.cpp:778).
- Sem VSync em código; comportamento depende do driver (default: desligado → tearing acima do limiter não ocorre pois o limiter é mais lento que qualquer refresh).
- Limiter + VSync poderiam coexistir se o driver forçasse VSync (efetivo = max(40 ms, 1/refresh)). Hoje: **somente o limiter atua**.

## 8. RENDER LOOP — acoplamento

`Scene → MainScene → RenderMainScene` [ZzzScene.cpp:2067]: `MoveMainCamera` → `BeginOpengl` → `CreateFrustrum` → `CreateScreenVector` → terreno/atributos → objetos → personagens → efeitos → UI (immediate mode, display lists) → (volta a MainScene) → `g_PhysicsManager.Render()` → overlay → `glFlush` → `SwapBuffers`.
**Veredito: parcialmente desacoplado** — o *passe* de render roda 1×/frame independentemente de quantos ticks executaram (câmera é por frame), mas **tudo que é atualizado** (posições, bones, efeitos, UI de jogo) só muda no tick de 40 ms → a 25 Hz. Render é immediate mode (sem batch/VBO) → custo de CPU por frame alto; `glFlush` força sincronização CPU→GPU por frame.

## 9. MOVIMENTO (fluxo real)

```
WndProc WM_MOUSEMOVE/LBUTTONDOWN (Winmain.cpp:728/742) → globals MouseX/MouseY/MouseLButton*  [eventos, imediato]
ScanAsyncKeyState (tick 25 Hz) → estado de teclas async
 ↓ (tick 40 ms)
MoveMainScene → MoveHero [ZzzInterface.cpp:7157]
 ├─ guarda-conds (dead/stun/loading/direction…)
 ├─ lê MouseX/Y/botões + buffs → decide ação (mover/atacar/skill)
 ├─ pathfinding: ZzzPath.h (FindPath; m_iNumPath; MAX_COUNT_PATH; tiles BYTE x/y)
 │   — consumo do path em ZzzAI.cpp:755-760 (PathNum = path->GetPath(); x=GetPathX(); y=GetPathY())
 ↓
MoveCharacter [ZzzCharacter.cpp:3836]  (POR TICK)
 ├─ position/âncora de modelo, CalcStopTime, HeroAttributeCalc, PushingCharacter
 ├─ AttackTime++ → g_iLimitAttackTime define fim do ataque (velocidade de ataque QUANTIZADA em ticks)
 └─ (posição autoritativa: servidor valida; pacotes de move/position chegam pela rede)
 ↓
MoveCharacterVisual [ZzzCharacter.cpp:~6540]  (POR TICK; OBB, safezone, cabeça)
 ↓ (frame)
RenderMainScene → desenha na posição corrente  ← SEM interpolação entre ticks
```
- **Sem delta time no movimento**; avanço é por tick/tile. `SetAttackSpeed()` [ZzzCharacter.cpp:779-793]: `PlaySpeed = 0.25f + AttackSpeed*0.004f` — **velocidade de ataque é codificada como velocidade de animação por tick**.
- **Depende de FPS?** Não (25 Hz sempre). **Interpolação/extrapolação?** Nenhuma — é a causa do "steppy feel" original.

## 10. ANIMAÇÃO — demonstração

Cadeia por tick: `MoveCharacter (:3836) → AnimationCharacter (:3489) → CharacterAnimation (:2280) → BMD::PlayAnimation [ZzzBMD.cpp:408]`:
```
*AnimationFrame += Speed;            // Speed = PlaySpeed do .bmd (constante por ação)
```
`PlaySpeed` vem do BMD (`b->Actions[b->CurrentAction].PlaySpeed`), ajustado por buffs/attack speed [ZzzCharacter.cpp:2300-2347]. `PriorAnimationFrame` só muda quando o inteiro do frame muda (transição de pose).
**Resposta à pergunta "25→144 FPS, o que acontece com a animação?":** **Nada** — `AnimationFrame` só avança dentro do tick de 40 ms, que continua a 25 Hz (o acumulador `TimeRemain` garante isso a qualquer FPS). A animação permanece **correta em velocidade, porém limitada a 25 passos/s** (sem interpolação de bones). A recíproca é a armadilha: **elevar a frequência do TICK para 144 Hz aceleraria animação, movimento, ataques e efeitos 5,76×** — tudo é por-tick.

## 11. FÍSICA — `Move(0.005f)`

- Chamada: **exclusivamente** ZzzScene.cpp:2353, dentro do tick, **5× consecutivas** por tick.
- Alvo: `CPhysicsManager::Move` [PhysicsManager.cpp:1207] → vento (random walk) + `CPhysicsCloth::Move` → `CPhysicsVertex::Move` [PhysicsManager.cpp:121]: **integração explícita de Euler** (`v += F/m·dt; x += v·dt`) de **cloth** (capas/cabelos — "SideHair", itens de pano).
- Significado de 0.005: **substep fixo de 5 ms** de simulação de tecido. 5 substeps × 5 ms = **25 ms de sim por tick de 40 ms** (tecido roda a **0,625× do tempo real** — levemente slow-mo; provável ajuste "a olho" de peso visual, não timestep de gameplay).
- Render do cloth é por frame (`g_PhysicsManager.Render()` ZzzScene.cpp:2469). **Não é física de gameplay** (colisões/movimento de personagens) — é visual.
- Classificação: fixed substep de subsistema visual, dependente do tick (não do frame).

## 12. INPUT

- **Eventos** (mouse/teclado de texto): WndProc → globals (`MouseX`, `MouseLButton`…) — imediatos, por mensagem do Windows (bombeados a cada iteração do while).
- **Polling** (`GetAsyncKeyState` wrapper): `g_pNewKeyInput->ScanAsyncKeyState()` **1× por tick (25 Hz)** — ZzzScene.cpp:2323. Movimento/combate leem o estado no tick seguinte.
- **DirectInput:** `DIRECTINPUT_VERSION 0x0800` definido (_define.h:18) e SDK presente, mas **sem `DirectInput8Create` no código** — input real = WM messages + GetAsyncKeyState. (SDK embutido usado p/ headers de som/device.)
- Buffering: fila de mensagens do Windows (OS); sem buffer próprio. **Input de gameplay é efetivamente 25 Hz.**

## 13. NETWORK DENTRO DO LOOP (fluxo real)

**Caminho clássico (C1–C4):**
```
SOCKET (non-blocking, WSAAsyncSelect → WM_ASYNCSELECTMSG)
 ↓ FD_READ (WndProc, Winmain.cpp:597 — thread principal, orientado a evento)
CWsctlc::nRecv() [WSctlc.cpp:432]
 ├─ recv() → m_RecvBuf[8192] (MAX_RECVBUF; overflow → log "Receive Packet Buffer Overflow")
 └─ parser de frames C1/C2/C3/C4 no buffer → m_pPacketQueue->PushPacket(frame,size)   [FILA]
 ↓ (1× POR FRAME, no while principal)
ProtocolCompiler [WSclient.cpp:11649]
 ├─ GetReadMsg() da fila (loop while(1) até esvaziar)
 ├─ C3/C4 → g_SimpleModulusSC.Decrypt + checagem de serial (g_byPacketSerialRecv)
 └─ ProtocolCore(HeadCode, ...) → handlers Receive* (WSclient.cpp)
```
**Caminho moderno (ASIO, ativo):** thread própria `thrContext = std::thread(m_context.run())` [ProtocolAsio.h:667] → callbacks async → `m_qMessagesIn` (deque com mutex) → **drenada por `gProtocolSend.RecvMessage()` 1×/frame** no while principal.

**Conclusões:** recepção é assíncrona (mensagens/thread), mas **dispatch é acoplado ao frame**; a 25 FPS o processamento de pacotes tem latência adicional de até ~40 ms e a fila pode acumular (pico de pacotes = processados em rajada no frame seguinte); FPS alto → drains mais frequentes (latência menor). **Pacotes não são perdidos por frame lento** (fila), exceto overflow do buffer bruto (8192).

## 14. THREADS DO CLIENTE

| Thread | Origem | Entrada | Sincronização |
|---|---|---|---|
| Principal (game) | WinMain | `WinMain` → while(1) | — (dona de todo o estado) |
| ASIO context | `ProtocolSend/ProtocolAsio.h:667` | `m_context.run()` | mutex por deque (olc::net) |
| ChatRoom sockets | `CChatRoomSocketList` (WinSock async, msgs na thread principal) | — | fila de janela |
| GameShop download | `GameShop/FileDownloader/FileDownloader.cpp:128` (`_beginthreadex`) | `RunConnectThread` | resultados via objeto |
| GameShop lista | `GameShop/ShopListManager/ListManager.cpp:173` | `RunFileDownLoadThread` | idem |
| Crash dump/upload | `Utilities/Dump/CrashHandler.cpp:408`, `Uploader.cpp:66` (`CreateThread`) | dump/upload | handles |
| Áudio | **externo** (`wzAudio.dll`, DirectSound) | — | interna da DLL |
| Timers Windows | `SetTimer` HACK 20 s / minimize 1 s / UI | WM_TIMER na thread principal | — |

**Não há thread de loading** (cargas são síncronas → freezes/stutters históricos). **Sem critical sections no código do jogo** (single-thread) exceto filas de rede.

## 15. TABELA DE DEPENDÊNCIAS TEMPORAIS (só evidências)

| Sistema | Frequência | Depende de FPS? | Depende de 40 ms? | Thread | Observação |
|---|---|---|---|---|---|
| Render | por frame (≤25) | sim (limiter) | não (roda mesmo sem tick) | principal | immediate mode + glFlush por frame |
| Input (eventos) | por mensagem | indireto | não | principal | MouseX/Y/botões |
| Input (poll lógico) | **25 Hz** | não | **sim** | principal | ScanAsyncKeyState no tick |
| Movimento | **25 Hz** | não | **sim** | principal | path/tile por tick, sem interpolação |
| Animação | **25 Hz** | não | **sim** | principal | AnimationFrame += PlaySpeed por tick |
| Física (cloth) | 25 Hz × 5 substeps | não | **sim** | principal | 25 ms sim/tick (0,625× real) |
| Effects/Joints/Particles | **25 Hz** | não | **sim** | principal | +WorldTime (ms real) p/ ciclos |
| Efeitos cênicos (luz/água) | por frame (render) + tick (frame d'água %32) | parcial | misto | principal | sinf(WorldTime·k) |
| Network receive | evento (msg/thread ASIO) | não | não | principal+ASIO | fila |
| **Network dispatch** | **por frame** | **sim** | indireto | principal | ProtocolCompiler/RecvMessage no while |
| UI (login/char) | por frame (Δ QPC) | parcial | não | principal | clamp 200 ms |
| UI de jogo (NewUI) | **25 Hz** | não | **sim** | principal | Update dentro de MoveMainScene |
| Cooldowns de skill | tempo real (ms) | não | não | principal | CalcSkillDelay(difTime) |
| Áudio 3D | por frame | parcial | não | principal | Set3DSoundPosition |
| Câmera | **por frame** | **sim** | não | principal | MoveMainCamera em RenderMainScene |

## 16/17. LIMITAÇÕES — CADEIA CAUSAL E CLASSIFICAÇÃO

| # | Limitação | Cadeia causal (LOCAL → efeito) | Class |
|---|---|---|---|
| L1 | **Cap 25 FPS por sleep** | ZzzScene.cpp:2518-23 (`sleep_for(40-Δ)` pós-swap) → frame ≥40 ms → render, dispatch de rede e câmera presos ao teto | **CRÍTICA** |
| L2 | Visual de simulação a 25 Hz | movimento/animação/efeitos só mudam no tick (§4/§9/§10) → mesmo sem L1, personagens "andam em degraus" de 40 ms; câmera seria suave, mundo não | **ALTA** |
| L3 | Rede processada por frame | ProtocolCompiler/RecvMessage no while (§13) → +0..40 ms de latência a 25 FPS; rajadas acumulam na fila | **ALTA** |
| L4 | Tick e limiter hardcoded sem constante | literal `40` em 2 pontos (2320 e 2518) → mudar cadência exige tocar 2 locais + auditar todo `MoveSceneFrame %N` (semântica "a cada N ticks") | **MÉDIA** |
| L5 | `CalcFPS` com bug de unidade + overlay duplicado | ZzzAI.cpp:781-812 (ms÷CLOCKS_PER_SEC); DeltaT morto → FPS exibido impreciso; risco de alguém "consertar" DeltaT e ligá-lo à lógica | **MÉDIA** |
| L6 | Cloth 0,625× tempo real | 5×0.005 s = 25 ms por tick de 40 ms (§11) → capas/cabelos em leve slow-mo (comportamento "de fábrica") | **BAIXA** (visual) |
| L7 | `MoveSceneFrame %N` como relógio | CSWaterTerrain:41, GMBattleCastle:733+, GMHellas:317+… → eventos de mapa amarrados a 25 Hz; se o tick mudar, todos os N precisam reavaliação | **MÉDIA** |
| L8 | Input de gameplay a 25 Hz | ScanAsyncKeyState + leitura no tick (§12) → resposta de movimento/ataque limitada ao tick | **MÉDIA** |
| L9 | Immediate mode + glFlush por frame | sem VBO/batch; glFlush força sync CPU→GPU → custo por frame alto; ceiling de performance a alto FPS | **MÉDIA** (perf) |
| L10 | Sem loading assíncrono | cargas síncronas na thread principal → hitches históricos | **MÉDIA** |
| L11 | `while(1)` com PeekMessage: arrastar janela/menus travam o jogo | Winmain.cpp:1759-1770 ( GetMessage só quando há mensagens; Scene só no else) | **BAIXA** |
| L12 | Precisão do `sleep_for` | sleep de SO (~1-15 ms de erro) → jitter real ~24-27 FPS em vez de 25 exato | **BAIXA** |
| L13 | `MAX_RECVBUF 8192` / `MAX_SPE_BUFFERSIZE 2048` | buffer bruto + builder de pacotes — tetos de I/O; não ligados ao FPS | **NÃO É LIMITAÇÃO** para FPS; teto de pacote (herança 0B-protocolo) |
| L14 | VSync ausente | sem wglSwapIntervalEXT → tearing se L1 removido sem presente-based sync | **NÃO É LIMITAÇÃO** hoje; **requisito futuro** |

## 18. NEW-ERA ARCHITECTURAL MAP (v1 — direções, não soluções)

```
L1 Cap 25 FPS (sleep)
 ├ Local: ZzzScene.cpp:2516-2524 (+ literal 40 em 2320)
 ├ Causa raiz: estabilização histórica (física/anim por tick + swap sem VSync)
 ├ Dependências: NENHUMA de lógica — acumulador TimeRemain já decouple tick×frame
 ├ Impacto: teto global de render/rede-visual/câmera
 └ Direção New-Era: frame pacing configurável (uncapped/VSync/cap) + tick fixo independente
L2 Visual 25 Hz
 ├ Local: tick 2320 → Move*/Animation* (§4)
 ├ Causa raiz: estado só mutável no tick; render lê estado corrente sem interpolação
 ├ Dependências: AnimationFrame/PriorAnimationFrame (poses), posições por tile, efeitos
 ├ Impacto: motion "steppy" mesmo a FPS alto
 └ Direção: interpolação/extrapolação no render (posição + bones), com toggle "feel clássico"
L3 Rede por frame
 ├ Local: Winmain.cpp:1810-1826 (ProtocolCompiler/RecvMessage)
 ├ Causa raiz: dispatch single-thread no loop
 └ Direção: drain por tick + drain por frame (híbrido) ou fila lock-free própria
L4/L7 Relógios por tick (MoveSceneFrame %N, AttackTime, WaterTexture)
 ├ Direção: converter semântica "N ticks" → tempo (ms) mantendo 25 Hz como padrão compatível
L9 Render immediate
 └ Direção: batch/VBO não é obrigatório p/ 0B — registrar como frente 0C/0D
L5 CalcFPS
 └ Direção: timer canônico único (QPC) + remover mortos (futuro; nada feito agora)
```

## 19. PERGUNTA CENTRAL — resposta com evidências

**O loop é arquiteturalmente acoplado?** **Sim, estruturalmente** — 1 thread faz tudo (mensagens → tick de lógica → render → rede), e o dispatch de rede + câmera + render compartilham o mesmo frame limitado a 25 FPS. **Porém o núcleo temporal já é semi-desacoplado**: o acumulador `TimeRemain` (ZzzScene.cpp:2320/2526/2819) separa *cadência de lógica* (tick 40 ms fixo, com catch-up) de *cadência de render* (1×/frame).

**É possível elevar o FPS a 60/120/144 sem alterar a lógica do jogo?**
**Mecanicamente, sim — a único obstáculo direto é o limiter (L1):** removendo apenas `sleep_for(40-DifTimer)` (hipótese; nada foi alterado), o acumulador mantém o tick a 25 Hz em qualquer FPS (a 144 FPS: ~1 tick a cada 6 frames), e **animicação/movimento/ataques/efeitos/cooldowns ficam idênticos** (tudo é por tick ou ms reais; `DeltaT`/`FPS` não alimentam lógica — evidência §5/§10). Partes que já são independentes do tick: câmera (por frame), render pass, recepção de rede (eventos), UI de login/char (Δ QPC), cooldowns (ms).
**O que NÃO melhora sem trabalho adicional:** suavidade de movimento/animação (25 Hz visual — L2), input de gameplay (25 Hz — L8), e o custo de CPU do immediate mode pode impedir 144 FPS estáveis (L9); VSync/tearing passa a importar (L14). **Armadilha confirmada:** elevar o *tick* (não o render) a 60/144 Hz aceleraria o jogo ~2,4/5,76× — proibido tocar no `40` do loop de lógica sem re-mapear L4/L7.

## 20. VERIFICAÇÃO FINAL

- `git status --short` → **vazio** (confirmado ao final da fase; nenhum commit, nenhuma alteração).
- Artefato criado: apenas este arquivo, fora do repositório. Nenhum zip, nenhuma cópia, nenhum clone novo, `Client/` não re-materializado.

### Anexo — Índice de continuação (para a sessão futura)
| Tema | Âncora |
|---|---|
| Limiter/tick | ZzzScene.cpp:2320, 2452, 2516-2524, 2819; ZzzAI.cpp:781 |
| Animação | ZzzCharacter.cpp:2280/3489/3836; ZzzBMD.cpp:408 |
| Attack speed↔anim | ZzzCharacter.cpp:779-793 |
| Física cloth | PhysicsManager.cpp:121/639/1207; ZzzScene.cpp:2353 |
| Rede clássica | Winmain.cpp:597; WSctlc.cpp:432; WSclient.cpp:11649 |
| Rede ASIO | ProtocolAsio.h:650-700; ProtocolSend.cpp |
| Input | Winmain.cpp:728-860; Input.cpp:85+; ZzzScene.cpp:2323 |
| UI timing | ZzzScene.cpp:2324-2331; Time/Timer.cpp |
| Relógios de mapa | CSWaterTerrain.cpp:41; GMBattleCastle.cpp:733+; GMHellas.cpp:317+; CSPetSystem.cpp:570 |
| Threads | ProtocolAsio.h:667; GameShop/FileDownloader.cpp:128; Utilities/Dump/CrashHandler.cpp:408 |
