# NEW-ERA SERVERS — PHASE 0A: REPOSITORY AUDIT
## Auditoria e Inventário Completo do Repositório Base

| Campo | Valor |
|---|---|
| **Projeto** | NEW-ERA SERVERS — Fase 0A (Aquisição & Inventário) |
| **Data da auditoria** | 2026-09-01 |
| **Auditor** | Agente Arena.ai (missão somente-leitura) |
| **North Star** | "Construir um cliente moderno capaz de preservar a essência do MU, mas remover as limitações técnicas históricas do cliente original." |

---

## 1. Identificação do Repositório

| Campo | Valor |
|---|---|
| URL | `https://github.com/wongddd/muonline` |
| Clone executado | `git clone https://github.com/wongddd/muonline.git` (clone completo, com histórico) |
| Commit HEAD analisado (main) | `580472e0d5723f9709cbad594f233deb07f9f351` — "v8: 添加跨目录纹理搜索，修复 BMD 纹理跨目录加载问题" (busca de texturas cross-directory) |
| Branches | `main` (default, à frente) e `origin/master` (`b9d07d2`, atrás do main por 8 commits — diffs apenas em docs/relatórios + 40 arquivos/888 inserções) |
| Total de commits | **17** |
| Primeiro commit | `5a28af2` "初始提交：完整项目源码" (commit inicial: código-fonte completo) |
| Origem declarada | Projeto de privado de MU Online **Season 5 (EX502)** — título do README: "MU Online Private Server (Season 5 / EX502)" |
| Lineagem do código | GameServer da família **"Louis Emulator" (creandomu.com — comunidade BR)**; cliente derivado das fontes clássicas WebZen (S5) |
| Idioma da documentação dos autores | Chinês (README, CLAUDE.md, fix logs, relatórios); strings do jogo em PT-BR ("PT-BR" hardcoded no log do cliente) |
| Estado do working tree | **LIMPO** — `git status --short` sem nenhuma alteração; nenhum arquivo do repositório foi modificado nesta missão |

### Histórico Git (últimos 20 commits, `--all --decorate`)

```
580472e (HEAD -> main, origin/main, origin/HEAD) v8: cross-directory texture search (BMD textures)
180edde @ 添加资源缺失分析日志 (待修复日志.md)          [log de recursos ausentes]
ad28c63 @ v7: 修复角色无法移动与连接断开问题            [fix movimento/desconexão]
bbed421 @ v6.1: 修复进入游戏后日志刷屏与连接断开         [fix log spam/desconexão]
c806753 @ v6: 修复角色选取与 Loading 协议适配           [fix seleção de personagem/Loading]
1bb58a6 @ 清理 /test 模式调试日志                       [limpeza logs do modo /test]
a5fbef4 @ /test 模式：解除 localhost 连接限制            [modo /test libera localhost]
3b22816 @ /test 模式：修复场景流转与自动选角色           [fluxo de cenas/auto-select]
bf19cb8 @ 调试模式：跳过资源缺失导致的阻塞/崩溃         [skip de recursos ausentes]
b9d07d2 (origin/master) README: 补充 v4/v5 修复日志
319b83f ProtocolSend: 登录成功后自动请求角色列表
f481519 ProtocolSend: 自动化测试模式自动登录
183351b @ 添加纹理加载失败回退与自动化测试模式支持       [textura fallback + autotest]
8c68a0e Update project version from Season 8 to Season 5  [crítico: rebaixa S8→S5]
742bc3e 清理 .gitignore，排除 Encoder 构建缓存
5a28af2 初始提交：完整项目源码                           [commit inicial]
497228b v3 修复：角色名编码、Shader 绑定、Custom UI 集成
(+ 2 commits anteriores de v1/v2)
```

**Leitura do histórico:** o autor pegou uma base S8/EX502, rebaixou para **S5/EX502** (`8c68a0e`) para casar cliente↔servidor, e passou 8 rodadas de correções (v1–v8, todas em 2026-05-26/27) focadas em: compilar, conectar, logar, selecionar personagem, entrar no jogo e carregar texturas.

---

## 2. Commit Analisado

- **Commit:** `580472e` (branch `main`) — clonado íntegro, com todo o histórico (17 commits), refs remotas preservadas (`origin/main`, `origin/master`, `origin/HEAD`).
- `git rev-parse --is-inside-work-tree` → `true`; `--show-toplevel` → `/home/user/muonline`; `git remote -v` → origin apontando para a URL oficial.
- **Nenhuma alteração de histórico foi realizada** (sem rebase/cherry-pick/filter).

---

## 3. Estrutura & Inventário

### 3.1 Números globais

| Métrica | Valor |
|---|---|
| Arquivos rastreados (git) | **18.070** |
| Diretórios | **303** |
| Tamanho do worktree (sem `.git`) | **~1,1 GB** |
| Tamanho do `.git` | ~544 MB (total do clone: 1,7 GB) |
| Branch default | `main` |

### 3.2 Inventário por pasta de topo

| Pasta | Arquivos | Tamanho | Conteúdo |
|---|---|---|---|
| `Client/` | 13.789 | 907 MB | Cliente pré-compilado: `Main.exe.original` (5,8 MB), DLLs, `Data/` (assets: 104 subpastas), `Scripts/*.reg` |
| `MuServer/` | 627 | 20 MB | Deploy do servidor: configs (`Data/` 326 arquivos), `DB/` (SQL), bins ausentes (.exe gitignored — só runtimes msvcr/msvcp100.dll) |
| `Source Client/` | 1.763 | 30 MB | Fonte do cliente (`source/` ~500+ cpp/h), `dependencies/` (ASIO etc.), `sdk-directx9/`, `Main.sln/vcxproj` |
| `Source Server/` | 1.819 | 121 MB | Fontes: GameServer (517 itens), ConnectServer, DataServer, JoinServer, `Util/` (cryptopp ×2, lua headers, MD5, CRC32, mapm) |
| `Source Tools/` | 45 | 44 MB | `Encoder/` — gerador do `Main.info` do cliente |
| `docs/` | 1 | 8 KB | `feature_flags.md` (mapa de feature flags servidor↔cliente) |
| Raiz | ~26 | ~1 MB | README, CLAUDE.md, 6 fix_logs, 2 relatórios de análise (ZH), 12 `.bat`, `Sync-Version.ps1`, `client_screen.png`, logs SQL |

### 3.3 Censo de extensões (top, por quantidade)

| Ext | Qtde | Ext | Qtde | Ext | Qtde |
|---|---|---|---|---|---|
| `.ozj` (textura JPEG MU) | 5.752 | `.wav` | 865 | `.vcxproj` | 14 |
| `.bmd` (modelo 3D MU) | 5.279 | `.txt` | 608 | `.md` | 13 |
| `.ozt` (textura TGA MU) | 1.653 | `.dat` | 149 | `.sln` | 9 |
| `.h` | 1.541 | `.att` (atributos de mapa) | 111 | `.lua` | 4 |
| `.cpp` | 959 | `.hpp` | 522 | `.ini` | 4 |
| `.bat` | 21 | `.reg` | 19+16 | `.ps1` | 1 |

Por tamanho: `.ozj` 279 MB, `.bmd` 238 MB, `.wav` 197 MB, `.ozt` 176 MB, `.sdf` (banco IntelliSense do VS — lixo commitado) 109 MB, `.pch` 34 MB.

**Linguagens:** C/C++ esmagadoramente dominante (959 .cpp + 522 .hpp + 1.541 .h + 69 .ipp + 44 .c + 33 .idl + 9 .rc); scripts de automação em Batch/PowerShell/Reg; SQL (T-SQL); 4 scripts Lua (órfãos — ver §7); sem C#/Java/Python.

**Ativos do cliente (`Client/Data/`):** `Player/` 2.185, `Item/` 1.200, `Monster/` 874, `Sound/` 865, `Interface/` 714, `Skill/` 366, `NPC/` 321, `Effect/` 281, `Object*/` (mundos) ~3.500+, mais `Enc1.dat`/`Dec2.dat` (chaves SimpleModulus).

### 3.4 Árvore estrutural (categorias)

```
muonline/
├── CLIENT .............................. Client/ (907 MB, pré-compilado)
│   ├── Main.exe.original (5,8 MB — binário do cliente)
│   ├── glew32.dll, ogg.dll, vorbisfile.dll, wzAudio.dll
│   ├── Main.info (config criptografado — gerado pelo Encoder)
│   ├── PacketList.txt (log de pacotes de sessão real, 2024)
│   ├── Data/ ........................... assets (ozj/bmd/ozt/wav/att/dat)
│   └── Scripts/ ........................ .reg (resolução, idioma, som, janela)
│
├── SERVER (deploy) ..................... MuServer/ (20 MB)
│   ├── ConnectServer/ ... ConnectServer.ini (TCP 44405/UDP 55557), ServerList.dat
│   ├── GameServer/ ...... DATA/GameServerInfo - {Common,ChaosMix,Character,Command,Custom,Event,Skill}.dat + LOGs
│   ├── GameServerCS/ .... variante Castle Siege (porta 55919)
│   ├── DataServer/ ...... DataServer.ini (ODBC MuOnline, porta 55960), AllowableIpList
│   ├── JoinServer/ ...... JoinServer.ini (porta 55970, ODBC, MD5 opcional)
│   ├── Data/ ............ 326 configs: Item/, Monster/, Shop/, Skill/, Quest/, Event/, Move/, Terrain/, CashShop/, Custom/...
│   ├── DB/ .............. MuOnline.sql (74 tabelas, UTF-16), .bak S4, .reg ODBC
│   ├── StartUp/, Tools/MAIN_INFO
│
├── SOURCE CLIENT ....................... Source Client/
│   ├── Main.sln / Main.vcxproj (VS2019 v142, Win32/x64, Global Debug|Release)
│   ├── source/ ........................ ~500+ arquivos: Winmain.cpp, Zzz* (engine), NewUI* (UI), GM* (mapas/eventos), w_* (pets/buffs), Protocol*.h/cpp, WSclient.cpp
│   │   ├── OpenGL3/Shader/ ............. STUB VAZIO (Shader.cpp = 0 linhas)
│   │   ├── Utilities/ (Log/ErrorReport, Dump/CrashReporter), ExternalObject/ (Leaf/ExceptionHandler, Chilkat, ResourceGuard)
│   │   └── GameShop/, Math/, Time/Timer
│   ├── dependencies/include/ .......... asio 1.19.1 standalone, glfw, glad, glext/wglext, Jpeglib, ThemidaSDK, object-pool, wzAudio
│   └── sdk-directx9/ ................. DirectX 9 SDK embutido
│
├── SOURCE SERVER ....................... Source Server/
│   ├── GameServer/ ..................... 517 itens (251 .cpp): GameServer.cpp (WinMain), Protocol.cpp (5.216 linhas), SocketManager*.cpp (IOCP+ASIO+UDP), ChaosBox.cpp (4.694), User.cpp, lua/ (4 scripts órfãos)
│   ├── ConnectServer/ ................. ClientManager, ServerList, IOCP, UDP heartbeat
│   ├── DataServer/ .................... QueryManager (ODBC direto), *DBSet (Character, Guild, Warehouse...)
│   ├── JoinServer/ .................... AccountManager, QueryManager, AllowableIpList
│   └── Util/ ......................... cryptopp (×2: legado + VS2019), lua/include (headers 5.1), MD5, CCRC32, Crypt_MT, mapm
│
├── TOOLS .............................. Source Tools/Encoder/ (Encoder.sln)
│         → gera Main.info (IP, versão, serial, CRC32 do AntiHack/ReShader, nome da janela)
│
├── DOCS ............................... README.md, CLAUDE.md, docs/feature_flags.md,
│                                       analysis_report.md, 客户端与服务端匹配分析报告.md (matching ~60%),
│                                       fix_log_v1..v6.md, 待修复日志.md (recursos ausentes)
│
├── CONFIGURAÇÕES ...................... *.ini (4), GameServerInfo*.dat (7), Data/*.txt (326), Main.info, config.ini (cliente), Scripts/*.reg
├── SCRIPTS ............................ 12 *.bat raiz (start_muserver, build_server, sql_*), Sync-Version.ps1, Client/Scripts/*.reg
├── DATABASE ........................... MuServer/DB/MuOnline.sql (74 tabelas) + .bak
├── BUILD .............................. 9 .sln / 14 .vcxproj / 8 .vcproj / 8 .dsp / 4 .bdsproj (legados) + lixo VS commitado (.sdf 109 MB, .suo, .aps, .pch, .tlog)
└── ASSETS ............................. Client/Data (~13,7k arquivos de arte/áudio/modelos)
```

---

## 4. Cliente

| Questão | Resposta |
|---|---|
| Projeto que gera o cliente | `Source Client/Main.vcxproj` |
| Solução | `Source Client/Main.sln` (VS2019, v142, WindowsTargetPlatform 10.0) |
| Configurações | `Global Debug` / `Global Release` × `Win32`/`x64` (build documentado: Global Release\|Win32) |
| Executável | `Main.exe` — presente no repo como **`Client/Main.exe.original`** (5.843.944 bytes; `*.exe` está no `.gitignore`, daí o sufixo) |
| DLLs utilizadas | `glew32.dll` (GLEW — runtime, não referenciada no fonte), `ogg.dll` + `vorbisfile.dll` (áudio Ogg), `wzAudio.dll` (wrapper de áudio MU) + sistema: opengl32/glu32, dinput8, dsound, winmm, imm32 |
| Ponto de entrada | `WinMain()` — `source/Winmain.cpp:1392` |
| Sistema gráfico | **OpenGL clássico de função fixa (immediate mode)** — `ChoosePixelFormat` + `wglCreateContext` (Winmain.cpp:877–893), `wglChoosePixelFormatARB` para multisample (ZzzOpenglUtil.cpp:778+). A pasta `OpenGL3/Shader/` é **um stub vazio** (Shader.cpp = 0 linhas, Shader.h = `#pragma once`) — o "OpenGL 3" do README é aspiracional |
| Versão DirectX/OpenGL | GL 1.x–2.x efetivo (sem contexto atribuído 3.x core, sem GLSL próprio); DirectX 9 SDK embutido (`sdk-directx9/`) usado para **input** (`DIRECTINPUT_VERSION 0x0800`) e **som** (DirectSound; `DSplaysound.cpp`, `wzAudio`) |
| Inicialização | `WinMain` → `CProtect::ReadMainFile("Main.info")` (obrigatório; aborta se ausente) → `leaf::AttachExceptionHandler` → versão do EXE → `OpenMainExe()` → carrega chaves `Data/Enc1.dat` (encrypt) e `Data/Dec2.dat` (decrypt) do **SimpleModulus** → `OpenInitFile()` (config.ini) → `CMultiLanguage` → cria janela/GL → `/test`: força `127.0.0.1:44405` + carrega dados + pula direto para LOG_IN_SCENE |
| Game Loop | `while(1)` com `PeekMessage/GetMessage`; idle → **`Scene(g_hDC)`** (ZzzScene.cpp:2828) que despacha `WebzenScene/LoadingScene/MainScene`; após cada frame: **`ProtocolCompiler()`** (fila de rede) + `g_pChatRoomSocketList->ProtocolCompile()` |
| Render Loop | `MainScene()` (ZzzScene.cpp:2314): `CalcFPS()` → **loop de lógica em passo fixo de 40 ms (25 Hz)**: input, UI update (delta limitado a 200 ms), `MoveMainScene()`, física `g_PhysicsManager.Move(0.005f)` ×5, `MoveSceneFrame++` → render (immediate mode) → `glFlush()` + **`SwapBuffers`** → **limitador pós-swap** (ver §8) |
| Rede inicializada em | `WSclient.cpp` (WinSock2 clássico: `CreateSocket`, `SocketClient`) e, com `NEW_PROTOCOL_SYSTEM` (**ativo** em `Defined_Global.h`), `gProtocolSend` (`ProtocolSend.cpp/h`) sobre **ASIO 1.19.1 standalone** (`ProtocolAsio.h`, estilo `olc::net`); conexão começa no ConnectServer (porta 44405) → recebe IP/porta do GameServer → reconecta no GS |

**Proteção/anti-hack do cliente:** `Protect.cpp`, `ProtectSysKey.cpp`, headers Themida (`VM_START/VM_END`), `Nprotect.h`, `nvapi` (ajustes de driver NVIDIA), `CProtect` valida `Main.info` (CRC32 do AntiHack e do "ReShader", serial, versão). `KeyGenerater.cpp` (serial/version keys).

---

## 5. Servidor

Todos os módulos são **aplicações Win32 GUI (WinMain)**, VS2019 v142, Windows SDK 10.0, charset MultiByte, 32 bits.

### 5.1 Módulos

| Módulo | Solução/Projeto | Porta (deploy) | Papel |
|---|---|---|---|
| **GameServer** | `Source Server/GameServer/GameServer.sln` → `GameServer.vcxproj` (Debug / **Release_EX502** / **Release_EX502CS**, Win32) | TCP 55901 | Simulação do jogo: 251 .cpp — combate, itens, skills, quests, eventos, guildas, pets, bots |
| **GameServerCS** | Mesma base, config `Release_EX502CS` (`GAMESERVER_VERSION "GSCS"`) | TCP 55919 | Variante Castle Siege do GameServer (deploy em `MuServer/GameServerCS`) |
| **ConnectServer** | `ConnectServer.sln` | **TCP 44405** / **UDP 55557** | Entry-point do cliente: lista de servidores (`ServerList.dat`), roteamento; recebe heartbeats UDP de GS/JS com contagem de players |
| **DataServer** | `DataServer.sln` | TCP 55960 (restrita por `AllowableIpList.txt`) | Camada de banco: **ODBC direto** (`SQLConnect` em `QueryManager.cpp`), DSN `MuOnline`; classes `*DBSet` (Character, Guild, Warehouse, Castle...) |
| **JoinServer** | `JoinServer.sln` | TCP 55970 | Autenticação de contas (`MEMB_INFO` via ODBC), MD5 opcional, senha global, bloqueio de conta |

### 5.2 Comunicação entre módulos (topologia confirmada nos fontes e configs)

```
CLIENT ──TCP 44405──► CONNECT SERVER ──lista/IP:porta──► CLIENT
CLIENT ──TCP 55901──► GAME SERVER ◄──TCP 55970──► JOIN SERVER ◄──ODBC──► SQL Server (MuOnline)
GAME SERVER ──TCP 55960──► DATA SERVER ──ODBC──► SQL Server
GAME SERVER / JOIN SERVER ──UDP 55557──► CONNECT SERVER (heartbeat + users online)
GAME SERVER ◄──TCP──► GAME SERVER CS (map-server move, via MapServerInfo.dat)
```

- Protocolos inter-servidor: **`DSProtocol`** (GS↔DS), **`JSProtocol`** (GS↔JS), **`CSProtocol`** (CS↔GS), **`ESProtocol`** (eventos), cada qual com dispatch próprio por opcode.
- O GameServer **nunca** acessa o banco diretamente — todo I/O de dados passa pelo DataServer; autenticação passa pelo JoinServer (boa segregação, herança da arquitetura original da WebZen).

### 5.3 Estrutura interna do GameServer

- Entrada: `GameServer.cpp` `WinMain` → `CMiniDump::Start()` (crash dumps) → `gServerInfo.ReadStartupInfo("GameServerInfo",".\\Data\\GameServerInfo - Common.dat")` → janela de console (ServerDisplayer) → IOCP/ASIO listeners.
- Rede: **`SocketManager.cpp` (IOCP clássico**, `CreateIoCompletionPort`, 895 linhas) coexistindo com **`SocketManagerModern.cpp/h` (ASIO/`olc::net`, `NEW_PROTOCOL_SYSTEM=1`)** e `SocketManagerUdp.cpp`.
- Timers: `QueueTimer.cpp` — Windows Timer Queue (`CreateTimerQueueTimer`, primeiro disparo 1000 ms, período configurável por timer).
- Bancada de bots próprios: `BotAlchemist/BotBuffer/BotHelper/BotPkClear/BotReset/BotReward/BotStore/BotWarper` (MAX_OBJECT_BOTS 200 / MAX_OBJECT_USER_AND_BOTS 2000).
- Sobrenome visível: título da janela `"... Louis Emulator - www.creandomu.com"`, `ReiDoMU.cpp` (custom BR).

---

## 6. Network (protocolo, pacotes, buffers)

### 6.1 Quadro clássico (compatibilidade S5)

| Tipo | Formato | Uso |
|---|---|---|
| `0xC1` | `[C1][Size:1B][Head:1B][payload]` | pacote de tamanho fixo, sem cifra |
| `0xC2` | `[C2][SizeH][SizeL][Head:1B][payload]` | pacote de tamanho variável, sem cifra |
| `0xC3` | = C1 + 1 byte de sequência, payload cifrado | credenciais, pacotes sensíveis |
| `0xC4` | = C2 + sequência, payload cifrado | idem, tamanho variável |

- **Cifra:** `CSimpleModulus` (`SimpleModulus.h`, blocos de 8→11 bytes, chaves em `Data/Enc1.dat`/`Dec2.dat` no cliente e equivalentes no servidor) + XOR (`XorData` em `StreamPacketEngine.h`) + `BuxConvert` para ofuscar conta/senha.
- **Ofuscação de opcodes:** `PROTOCOL_CODE1..4` mudam conforme `GAMESERVER_LANGUAGE` (movimento/ataque/posição usam códigos diferentes por idioma) — cliente e servidor **devem** compilar com o mesmo valor (=1 hoje).
- **Serialização:** structs C `#pragma pack(1)` (ex.: `PMSG_CONNECT_ACCOUNT_SEND { char account[10]; char password[20]; DWORD TickCount; BYTE ClientVersion[5]; BYTE ClientSerial[16]; }`). Sem JSON/protobuf.

### 6.2 Quadro moderno (NEW_PROTOCOL_SYSTEM — ativo nos dois lados)

- Cabeçalho: **`{ID: uint16 (enum ProtocolHead), Size: uint32}`** + corpo de tamanho variável (`std::vector<uint8_t>`), sobre **TCP/ASIO 1.19.1** (estilo `olc::net`, fila thread-safe com mutex/deque, thread de contexto dedicada).
- Cliente: `gProtocolSend` (`ProtocolSend.cpp/h`) —mensagens `CLIENT_ACCEPT/LIVE`, `BOTH_CONNECT_LOGIN/CHARACTER`, `BOTH_POSITION/MOVE/ATTACK1..3`, `BOTH_MESSAGE` etc.
- Servidor: `CSocketManagerModern` (acceptor ASIO, `DataReceived`/`PacketSend`) ponteando para o dispatch clássico.

### 6.3 Buffers, filas, threads

| Local | Constante | Valor |
|---|---|---|
| Cliente `StreamPacketEngine.h` | `MAX_SPE_BUFFERSIZE` | **2048** (builder de pacotes — teto prático de payload) |
| Cliente `WSclient` | `MAX_SENDBUF` / `MAX_RECVBUF` | 8192 / 8192 |
| Servidor | `MAX_MAIN_PACKET_SIZE` | 8192 |
| Servidor | `MAX_QUEUE_SIZE` | 2048 (fila de pacotes por contexto) |
| Servidor | `MAX_SERVER_WORKER_THREAD` | 8 |
| Servidor UDP | `MAX_UDP_PACKET_SIZE` | 4096 |
| GS objetos | `MAX_OBJECT_USER` 1000, `MAX_OBJECT_MONSTER` 8000, `MAX_OBJECT` 4500/10000, bots 200(2000) |

### 6.4 Handlers e dispatch

- **Servidor (GS):** `Protocol.cpp` (**5.216 linhas**) — `switch` gigante por head code (0x00 chat … 0xF1 login … 0xF3 personagem). Handlers C→S nomeados `CGxxxRecv`, S→C `GCxxxSend`. `DSProtocol/JSProtocol/CSProtocol/ESProtocol` para os pares internos.
- **Cliente:** `WSclient.cpp` (~12k linhas, `Receive*` por opcode) + `ProtocolSend.cpp` (caminho moderno) + `ProtocolCompiler()` chamado **a cada frame** do loop principal (rede processada no ritmo do frame).
- `PacketList.txt` na raiz do `Client/` é um log real de sessão (C→S `C1040001FDCD`, S→C `0xF4 0x06` etc.) — evidência de sniffing/validação feita pelos autores.

---

## 7. Lua / Scripting

**Constatação central: Lua existe como ativo, mas NÃO está integrado ao build atual.**

| Evidência | Detalhe |
|---|---|
| Headers | `Source Server/Util/lua/include/` e `Source Server/GameServer/GameServer/lua/include/` (lua.h, lauxlib.h, luaconf.h, lua.hpp, lualib.h — **Lua 5.1**) — apenas headers, sem `lua51.lib` (libs estão gitignored) |
| Scripts | `GS/lua/{EventDrop.lua, MonsterDie.lua, NpcTalk.lua, NpcTalk_Rewrite.lua}` — cabeçalho "zGameServer Season 6 Episode 3, 19.12.2013"; esperam globals `lpObj`/`lpNpc` e funções do host `ChatTargetSend`, `gObjTeleport`, `TestLuaNPC` |
| Código C/C++ | **Zero** chamadas a `luaL_newstate/lua_pcall/lua_register/luaL_dofile` fora dos próprios headers; nenhum `#include "lua...` nos fontes do GS; `GameServer.vcxproj` não referencia Lua |
| Contradição documental | `CLAUDE.md` e `analysis_report.md` afirmam "Uses Lua 5.1 for scripting" — **afirmação desatualizada/aspiracional**; o `NpcTalk.cpp` nativo cobre o caso de uso do `NpcTalk.lua` |

**Implicação para a New-Era:** a "engine de scripts" que a documentação do autor menciona não está wired — qualquer iniciativa data-driven/scriptável parte do zero (ou de um re-wiring deliberado). Não há buffers/limites Lua a auditar porque não há host Lua em execução.

---

## 8. FPS / Rendering (o coração das limitações históricas)

| Aspecto | Constatação | Local |
|---|---|---|
| **Cap de FPS** | **Limite rígido de 25 FPS (40 ms/frame)**: após `SwapBuffers`, se o frame levou <40 ms, `sleep_for(40 - DifTimer)` | `ZzzScene.cpp:2516-2524` |
| **Passo de lógica** | Fixo em **40 ms (25 Hz)** — `for(Remain = TimeRemain; Remain >= 40; Remain -= 40)`; input/movimento/UI/física rodam nesse ritmo | `ZzzScene.cpp:2320` |
| **Física** | `g_PhysicsManager.Move(0.005f)` chamado **5× por tick de lógica** (delta hardcoded) | `ZzzScene.cpp` (MainScene) |
| **Delta time de UI** | `g_pTimer->GetTimeElapsed()` (QueryPerformanceCounter), clamp de 200 ms | `Time/Timer.{h,cpp}`, MainScene |
| **Tempo do mundo** | `WorldTime = timeGetTime()` (ms, winmm) — efeitos/iluminação derivam de `sin(WorldTime*k)` | `ZzzAI.cpp:781 (CalcFPS)` |
| **FPS medido** | `CalcFPS()` com `timeGetTime`; `DeltaT` calculado com unidade confusa (`/CLOCKS_PER_SEC` sobre ms — bug histórico clássico do MU) | `ZzzAI.cpp:781-812` |
| **VSync** | **Ausente** — nenhuma chamada a `wglSwapIntervalEXT`/swap control em todo o fonte |
| **Apresentação** | `glFlush()` + `SwapBuffers(hDC)` imediatos, sem triple buffering explícito | `ZzzScene.cpp:2511-2514` |
| **Frame limiter** | Único mecanismo = o sleep pós-swap de 40 ms (acima) |
| **Shaders** | Nenhum em uso: `OpenGL3/Shader/Shader.cpp` **vazio**; pipeline = função fixa (glBegin/glEnd, display lists, GL_LIGHTING) — o fix_log_v3 menciona "bind de shaders Terrain/SimpleTexture", mas o arquivo-fonte do shader está vazio no HEAD |
| **Aceleração** | `wglChoosePixelFormatARB` para FSAA/multisample (existe tentativa de antialias) | `ZzzOpenglUtil.cpp:778-840` |

**Conclusão de fase:** o cliente é prisioneiro do teto de 25 FPS e do passo fixo de lógica acoplado ao render — exatamente a "limitação técnica histórica" que a New-Era quer remover. Qualquer cliente novo precisa desacoplar: tick de simulação (fixo), render (variável), input (poll por evento) e sincronização (VSync opcional + cap configurável).

---

## 9. Items

| Conceito | Local | Detalhe / Limites |
|---|---|---|
| Definição de item (servidor) | `Item.h` — classe `CItem` | `m_Serial` (DWORD), `m_Index` (short = ID+level), `m_Level`, `m_Durability` (float), `m_SpecialIndex/Value[MAX_SPECIAL=14]`, exc/special flags, `m_SocketOption[MAX_SOCKET_OPTION=5]`+bonus, JewelOfHarmony, 380, Lucky, Pentagram, Muun |
| Flags de categoria | `CItem::IsExcItem/IsSetItem/IsJewelOfHarmonyItem/Is380Item/IsSocketItem/IsLuckyItem/IsPentagram*` | polimorfismo por bitfields/arrays |
| Inventory | `INVENTORY_WEAR_SIZE 12`; `INVENTORY_MAIN_SIZE 76`; **extensões versionadas**: `INVENTORY_FULL_SIZE 108 (S5) → 236/237 (≥602)`; `INVENTORY_SIZE` idem | `Item.h:6-29` (portas `GAMESERVER_UPDATE`) |
| Warehouse | `WAREHOUSE_SIZE 120` (S5) → `240` (≥602) | `Item.h:30-36` |
| Trade | `TRADE_SIZE 32` | `Item.h:37` |
| Chaos Box | `CHAOS_BOX_SIZE 32` | `Item.h:37` (input da Chaos Machine) |
| Options | `MAX_EXC_OPTION 6`, `MAX_SOCKET_OPTION 5`, `MAX_SPECIAL 14`, `MAX_RESISTANCE_TYPE 7`, `MAX_PET_LEVEL 50` | `Item.h:38-44` |
| Preço | `MAX_ITEM_PRICE 3.000.000.000` (Zen em DWORD) | `Item.h:45` |
| Configs de item (deploy) | `MuServer/Data/Item/`: `Item.txt` (114 KB; alt. `Item - Original MUemu.txt` 191 KB), `ItemOption.txt`, `ItemOptionRate.txt`, `ItemStack.txt`, `ItemValue.txt`, `ItemDrop.txt`, `ItemMove.txt`, `SetItemType/Option.txt`, `SocketItemOption.txt`, `JewelOfHarmony{Type,Option}.txt`, `380Item{Type,Option}.txt` | leitor: `CMemScript` (tokenizer) |
| Cliente | `ItemManager/ItemInfo/ItemAddOptioninfo/ZzzInventory/NewUIMyInventory/NewUIStorageInventory (warehouse)/NewUITrade/zzzMixInvetory` | render e manipulação de grade (8×~) |
| Drop | `ItemBag/ItemBagEx/ItemBagManager` + `EventItemBag/` (~pastas de drops por evento) + `CustomEventDrop` | rates em `GameServerInfo - Event.dat` |

**Nota:** os limites de inventário/warehouse são **compilados por versão** (`GAMESERVER_UPDATE`) — aumentar slots exige recompilar cliente e servidor em sintonia (limitação arquitetural para gameplay data-driven).

---

## 10. Chaos Machine

| Questão | Resposta |
|---|---|
| Onde fica | Servidor: **`ChaosBox.cpp` (4.694 linhas) + `ChaosBox.h`** — classe `CChaosBox`; Cliente: `MixMgr.cpp`, `zzzMixInvetory.cpp`, `NewUIMixInventory.cpp` |
| Entrada de protocolo | Pacote **`C1:86`** → `CGChaosMixRecv(PMSG_CHAOS_MIX_RECV{header, type, info}, aIndex)` (`ChaosBox.cpp:4359`); resposta `GCChaosMixSend` (result + `ItemInfo[MAX_ITEM_INFO]`) |
| "Receitas" | **Enum hardcoded `eChaosMixNumber` com ~55 tipos** (CHAOS_MIX_CHAOS_ITEM=1 … CHAOS_MIX_SOCKET_WEAPON=57) — cada mix é uma **função C++ dedicada** (`ChaosItemMix`, `DevilSquareMix`, `PlusItemLevelMix(1..6)`, `Wing1/2/3/4Mix`, `DinorantMix`, `FruitMix`, `BloodCastleMix`, `PetMix`, `SiegePotionMix`, `LifeStoneMix`, `SeniorMix`, `HornOfFenrirMix/Upgrade`, `ShieldPotionMix`, `JewelOfHarmony Purity/Smelt/Restore`, `Item380Mix`, `IllusionTempleMix`, `FeatherOfCondorMix`, `ChaosCardMix`, `CherryBlossomMix`, `SocketItem CreateSeed/SeedSphere/Mount/UnMount`, `ImperialGuardianMix`, `ChestMix`, `SummonScrollMix`, `LuckyItem Create/Refine`, `MonsterWingMix`, `SocketWeaponMix`, `Pentagram Mithril/Elixir/Jewel/Decomposite/UpgradeLevel/UpgradeRank`) |
| Onde ficam as taxas | **Parcialmente data-driven**: `MuServer/GameServer/DATA/GameServerInfo - ChaosMix.dat` (rate por tipo de mix, lido por `CServerInfo`) — mas ingredientes/estrutura/resultado permanecem **no código** |
| Extensões data-driven | `CustomMix.cpp` (319 linhas) + `CustomWingMix.cpp` — carregam receitas de arquivos (`CCustomMix::Load(path)` via `CMemScript`, vetores `m_CustomMixInfo/m_CustomMixItemInfo/m_CustomMixRewardInfo`) e são invocados por `CChaosBox::CustomMix(lpObj,type)` — **já existe um caminho data-driven embrionário** |
| Requisitos/custos | hardcoded por mix (level, zen, itens específicos, `m_ChaosMixRate` ± `TalismanOfLuckRate`/`ElementalTalismanOfLuckRate`) |
| Resultados | sucesso → item gerado + `GCChaosBoxSend`/`GCChaosMixSend`; falha → itens consumidos/dispensados conforme mix; box persistida via `ChaosBoxItemSave` → DataServer |
| Limite estrutural | `CHAOS_BOX_SIZE 32` slots de input; ~55 mixes; enum ordenado (número do mix = byte no pacote) — adicionar mix novo hoje = tocar enum + função + switch + config |

**Para o futuro data-driven:** o par (enum + função por mix) é o gargalo; `CustomMix.cpp` é o molde natural de receita genérica (ingredientes + condições + taxas + recompensas em arquivo).

---

## 11. Scripts (automação/ops)

| Script | Papel |
|---|---|
| `start_muserver.bat` | sobe a bateria de servidores na ordem |
| `start_sql.bat`, `sql_debug.bat`, `check_sql_error.bat`, `install_sql_service.bat`, `set_sql_auto.bat`, `fix_sql_perms.bat`, `fix_sql_reg.bat`, `fix_sql_reg2.bat`, `build_server.bat` | operação de SQL Server e build (herança do ambiente de teste dos autores) |
| `Sync-Version.ps1` | sincroniza versão/serial cliente↔servidor (criado no fix v2) |
| `Client/Scripts/*.reg` | resolução (640×480→1920×1080), idioma (EN/PT/ES), som on/off, janela/tela cheia — o cliente original lia essas chaves de registro |
| `MuServer/StartUp/Configuration` | ordem/parâmetros de inicialização dos módulos |

---

## 12. Database

- **Schema:** `MuServer/DB/MuOnline.sql` — dump T-SQL **UTF-16 LE**, 364 KB, **74 tabelas** (`Character`, `AccountCharacter`, `MEMB_INFO`*, `MasterSkillTree`, `Warehouse/ExtWarehouse`, `Guild*`, `Castle*`/`WZ_CW_INFO`, `CashShop*`, `LuckyItem/LuckyCoin`, `Marry`, `ItemLog`, `Custom*` — Attack/Store/Gift/Skin/ItemBank/JewelBank/Flag/CommandDelay, `Event*`, `DefaultClassType`, `LOG_CREDITOS`, …).
- **Acesso:** DataServer e JoinServer via **ODBC** (`SQLConnect`, DSN `MuOnline`; `MuOnline.reg`/`MuOnline64.reg` criam o DSN). GameServer não toca o banco.
- **Backups:** `MuOnline.bak` (3,1 MB), `Backup Season 4 update 15 main 5.2.bak` (4 MB) — dados reais de servidor incluídos.
- **Persistência de runtime:** GS → DS por opcodes 0x30–0x42 (save de personagem/inventário/opções/pets/ranking) e 0x80+ (Castle Siege); logs em `MuServer/*/LOG/`, `COMMAND_LOG`, `CONNECT_LOG`, `HACK_LOG`.

---

## 13. Configuration

| Arquivo | Leitor | Formato | Comportamento se inválido |
|---|---|---|---|
| `MuServer/GameServer/DATA/GameServerInfo - {Common,ChaosMix,Character,Command,Custom,Event,Skill}.dat` | `CServerInfo::ReadStartupInfo` + `CMemScript` | seções/chaves custom (tokenizer) | `ErrorMessageBox` / default + log |
| `MuServer/Data/*.txt` (item, monster, shop, skill, quest, move, terrain, message…) | gerenciadores (`CItemManager::Load`, etc.) via `CMemScript` | texto estruturado com `end` | erro de load exibido/registrado; GS não sobe |
| `ConnectServer.ini` / `ServerList.dat` / `MapServerInfo.dat` | ConnectServer | INI / tabela | default/falha de listen |
| `DataServer.ini` / `JoinServer.ini` (+`AllowableIpList.txt`, `BadSyntax.txt`) | respectivos | INI | aborta conexão ODBC |
| Cliente `Main.info` | `CProtect::ReadMainFile` | **binário codificado** (gerado pelo `Encoder`) — IP, porta, versão, serial, CRCs | `MessageBox` + `ExitProcess` |
| Cliente `config.ini` | `OpenInitFile()` | INI | aborta com log no ErrorReport |
| `Client/Scripts/*.reg` | instalador/usuário | registro Windows | — |
| Client `Data/Enc1.dat`/`Dec2.dat` | `CSimpleModulus::Load*Key` | binário (chaves) | sem cifra/falha de conexão |

**Versionamento crítico:** `ClientVersion`/`ClientSerial` precisam bater entre `Main.info`, `ConnectServer` e `JoinServer` — hoje sincronizados manualmente (motivo de existir do `Sync-Version.ps1`).

---

## 14. Diagnostics (crash/erros)

| Camada | Mecanismo | Local |
|---|---|---|
| GameServer & demais servidores | **`CMiniDump`** — `SetUnhandledExceptionFilter` → `MiniDumpWriteDump` (dbghelp) com `MiniDumpScanMemory+WithIndirectlyReferencedMemory` | `*/MiniDump.cpp` (cada módulo tem o seu) |
| Servidores (log) | `CLog::Output(type, ...)` com janela-console `CServerDisplayer`; arquivos em `MuServer/*/LOG/`, `HACK_LOG`, `CONNECT_LOG`, `COMMAND_LOG` | `Log.cpp` |
| Servidores (erros fatais) | `ErrorMessageBox` (janela modal) + `gLog` | `Util.cpp` |
| Cliente (exceções) | **`leaf::AttachExceptionHandler(ExceptionCallback)`** + `ExternalObject/Leaf/ExceptionHandler` | `Winmain.cpp`, `ExternalObject/Leaf/` |
| Cliente (dump) | `Utilities/Dump/CrashReporter` (minidump) | `source/Utilities/Dump/` |
| Cliente (log corrido) | `CErrorReport g_ErrorReport` (`Utilities/Log/ErrorReport`) — grava execução, sysinfo, passos; `muConsoleDebug` para console | `Winmain.cpp` |
| Cliente (rede) | `AddDebugText` + `SAVE_PACKET` → `PacketList.txt` | `WSclient.cpp:149` |
| GS anti-cheat logs | `HackLog`/`HackCheck` e pasta `Data/Hack` | GS |
| Assertions | **Não há** sistema de assert próprio digno de nota; diagnóstico é por log/dump/modal | — |

**Avaliação:** existe minidump nos dois lados e log textual razoável, mas não há telemetria central, symbols pipeline, nem crash-reporting assíncrono — ponto de melhoria para a New-Era.

---

## 15. Dependencies

### Necessárias (para compilar/rodar)
| Dependência | Onde | Observação |
|---|---|---|
| Visual Studio 2019 **v142**, Windows SDK 10.0 | todos os projetos | toolset declarado nos `.vcxproj` |
| Win32/WinSock2/IOCP/ODBC (odbc32) | servidores | sistema |
| **ASIO 1.19.1 standalone** | `Source Client/dependencies/include/asio` + GS modern | header-only |
| DirectX 9 SDK (dinput8, dsound, d3dx) | `Source Client/sdk-directx9` embutido | input/áudio |
| OpenGL (opengl32/glu32) + glext/wglext/KHR headers | cliente | função fixa |
| **cryptopp** (Crypto++) | `Source Server/Util/cryptoppVs2019` | 2ª cópia legada em `cryptopp/`; licença Boost |
| libjpeg headers (`Jpeglib.h`), zlib-indep | cliente (decodifica .ozj) | via header |
| Themida SDK headers (`ThemidaSDK.h`, `ThemidaInclude.h`) | cliente/GS/Encoder | **SDK comercial externo não incluso** — builds protegidos exigem o produto |
| Runtimes: `msvcp100/msvcr100.dll` (deploy), v142 redist | MuServer/*, cliente | — |
| `wzAudio.dll`, `ogg.dll`, `vorbisfile.dll`, `glew32.dll` | cliente (runtime) | áudio Ogg + GLEW |
| MD5/CCRC32/Crypt_MT/mapm (in-tree) | `Source Server/Util` | autenticação/serial |

### Opcionais
- `GLFW`, `glad`, `object-pool`, `nvapi` headers (cliente) — presentes nos includes; **sem uso efetivo** no pipeline atual (GLFW/glad órfãos do plano "GL3").
- `Chilkat`, `ResourceGuard` (ExternalObject) — presentes, uso pontual.

### Legacy
- 2ª cópia de cryptopp (não-VS2019), projetos `.dsp/.vcproj/.bdsproj` (VC6/VS2003/BDS), `.sdf/.suo/.aps/.pch/.tlog` commitados (109 MB só o .sdf), `CUsersAdministratortmpcl.rsp` (arquivo de resposta do compilador com path de máquina do autor), `Main.exe.original`, runtimes msvc100 nos módulos.

---

## 16. Build System

| Aspecto | Detalhe |
|---|---|
| Versão VS | **2019 (v142)**, todas as soluções |
| Windows SDK | 10.0 |
| Plataformas | Win32 primário (cliente declara Win32+x64; **nenhum x64 de servidor**) |
| Configurações | GameServer: `Debug` / `Release_EX502` / `Release_EX502CS`; Cliente: `Global Debug` / `Global Release`; demais módulos: Debug/Release |
| Macros de versão | `GAMESERVER_UPDATE` (**502** — rebaixado de 803 no fix v1), `GAMESERVER_LANGUAGE` (1), `NEW_PROTOCOL_SYSTEM` (1), `PROTECT_STATE`, `ENCRYPT_STATE` (0), `PG_CUSTOM`, `POINT_CUSTOM`, `BOT_BUFFER`, etc. (ver `docs/feature_flags.md`) |
| Build documentado | `msbuild "Source Server\GameServer\GameServer.sln" /p:Configuration=Release_EX502;Platform=Win32` e `msbuild "Source Client\Main.sln" /p:Configuration="Global Release";Platform=Win32` |
| Charset | MultiByte (sem Unicode) — strings PT-BR/KR/ZH misturadas |
| Artefatos | `.exe/.lib/.pdb` **gitignored** — por isso os binários de servidor não estão no repo (só DLLs de runtime e `Main.exe.original`) |
| Build nesta fase | **NÃO executado** (conforme a missão: análise apenas; build geraria artefatos e exigiria Windows/VS2019) |

---

## 17. Possible Limitations (catálogo de limites — LOCAL • VALOR • FUNÇÃO • IMPACTO)

> Nem todo número abaixo é "limitação"; os marcados ⚠️ são candidatos reais a teto histórico.

### Cliente
| Local | Valor | Tipo/Função | Impacto |
|---|---|---|---|
| ⚠️ `ZzzScene.cpp:2516-2524` | **40 ms (25 FPS)** | sleep pós-SwapBuffers | **teto de render de 25 FPS** — limitação #1 do North Star |
| ⚠️ `ZzzScene.cpp:2320` | **40 ms** | passo fixo de lógica | simulação presa a 25 Hz; gameplay atrelado ao frame |
| `ZzzAI.cpp:781` | `CLOCKS_PER_SEC` sobre ms | cálculo de DeltaT/FPS | unidade inconsistente (bug histórico), deltas distorcidos |
| ⚠️ `StreamPacketEngine.h:7` | `MAX_SPE_BUFFERSIZE 2048` | builder de pacotes | **teto de payload por pacote do cliente** |
| `WSclient` | `MAX_SENDBUF/RECVBUF 8192` | buffers de socket | teto de I/O por conexão |
| `ProtocolSend.h` | `account[10]`, `password[20]`, `path[8]` | structs de login/movimento | IDs curtos; path de movimento máx. 8 passos |
| `Define/_define.h` região | `MAX_TEXT_LENGTH 255`, `MAX_TEXTS 3000`, `MAX_TITLELENGTH 32` | chat/UI | mensagens e buffers de texto curtos |
| Client misc | `MAX_VERTICES 15000`, `MAX_SPRITES 1000`, `MAX_SKILLS 600`, `MAX_SKILL_EFFECTS 100`, `MAX_SOCKETS 5`, `MAX_SERVER_LOW/HI 20/30`, `MAX_QUEST_TEXT 100`, `MAX_QUEST_REQUEST 16` | pools fixos de render/dados | risco de overflow/saturação em cenários densos |
| `MapManager.cpp:1543` | loop fixo `ExtTile01..16.jpg` ×44 mundos | tiles de terreno | **704 texturas exigidas — atualmente ausentes** (causa de xadrez magenta; ver 待修复日志.md) |

### Servidor
| Local | Valor | Tipo/Função | Impacto |
|---|---|---|---|
| ⚠️ `User.h:14-22` | `MAX_OBJECT_USER 1000`, `MAX_OBJECT_MONSTER 8000`, `MAX_OBJECT 4500/10000`, bots 200/2000 | array global de objetos | **teto de players/monstros por GS** |
| ⚠️ `SocketManagerModern.h:14` e pares | `MAX_MAIN_PACKET_SIZE 8192`, `MAX_QUEUE_SIZE 2048`, `MAX_SERVER_WORKER_THREAD 8` | rede moderna | teto de pacote/fila/threads |
| `Item.h:6-45` | inventário 108→237, warehouse 120→240, trade 32, chaos box 32, exc 6, socket 5, special 14, preço 3e9 | limites de gameplay por versão | mudar slot = recompilar 2 lados |
| `MasterSkillTree.h` | `MAX_CHARACTER_MASTER_LEVEL 1000` (custom BR; oficial S5=200/400) | progressão | já expandido pelo lineage |
| Misc GS | `MAX_MAP 91`, `MAX_VIEWPORT 75`, `MAX_PARTY_USER 5`, `MAX_TVTEVENT_USER 40`, `MAX_UDP_PACKET_SIZE 4096`, `MAX_COLUMNS 100`, `MAX_WING_MIX 50` | mundo/social | tetos de conteúdo |
| `QueueTimer` | timers do Windows (WT_EXECUTEINTIMERTHREAD) | sched | precisão limitada de tick |

### Possível origem comum
Quase todos os tetos vêm da **era 32-bit/WebZen S2-S5**: arrays fixos globais (evitar alocação), protocolos de tamanho fixo (contra packet-sniffing), e o cap de 25 FPS herdado do timing de máquinas da época (o sleep pós-swap estabilizava física/áudio em CPUs lentas).

---

## 18. Possible Architectural Problems

1. **Acoplamento render↔lógica↔rede no cliente:** o processamento de protocolo (`ProtocolCompiler`) roda dentro do loop de FPS; queda de frame atrasa rede (e vice-versa).
2. **Duplo stack de rede coexistente** (C1–C4/SimpleModulus + ASIO `olc::net`) selecionado por macros — complexidade e superfície de bug (os fix logs v4–v7 do autor são quase todos sobre isso: desconexões, loading travado, protocolo de login).
3. **Dispatch monolítico:** `Protocol.cpp` com 5.216 linhas de switch; `WSclient.cpp` ~12k linhas — sem camada de serialização versionada.
4. **Gameplay hardcoded:** ~55 mixes da Chaos Machine como funções; 28 features "Custom*" do servidor **sem UI cliente** (relatório de matching dos autores: ~60% de aderência global, ~0% em Custom).
5. **Configuração tripulta e manual:** versão/serial em `Main.info` (Encoder) + `JoinServer` + cliente; chaves SimpleModulus em .dat; erro de sincronia = cliente não loga (motivo do `Sync-Version.ps1`).
6. **Charset MultiByte + strings PT-BR/KR/ZH** embutidas no código — internacionalização frágil (o fix v3 foi exatamente um bug de encoding de nome de personagem).
7. **Higiene de repositório:** 109 MB de `.sdf`, `.pch`, `.suo`, `.tlog`, `.aps` commitados; duas cópias de cryptopp; binário `Main.exe.original` versionado; `.gitignore` adicionado depois do commit inicial.
8. **Sem testes automatizados** no repo (o modo `/test` do cliente e o `protocol_test.c` citado no fix_log_v3 — ausente no HEAD — são as únicas ferramentas de validação; a automação é ad-hoc).
9. **Documentação desatualizada em pontos-chave** (Lua "5.1 em uso" — não está; "OpenGL 3" — stub vazio; README diz Season 5, CLAUDE.md diz Season 8).
10. **Banco via ODBC síncrono no DataServer** — gargalo clássico de persistência em picos (fila GS→DS única).
11. **Segurança legada:** Themida/anti-hack com CRC de arquivos, senha global do JoinServer em claro no `.ini` (`GlobalPassword = _H6huC9wbqF9AbgsqHpb5eB`), MD5 opcional.

---

## 19. Pontos que Precisam de Investigação (para as próximas fases)

1. **Onde exatamente o loop moderno ASIO encosta no dispatch clássico** (`SocketManagerModern::DataReceived` → `Protocol.cpp`): mapear a tabela de conversão `ProtocolHead` ↔ opcodes C1/C2 para saber o que já migra limpo para um cliente novo.
2. **O pacote de login com 1 byte de diferença** (`LanguageCode`) citado no relatório de matching dos autores — confirmar struct a struct no HEAD atual.
3. **Assets ausentes:** 704 `ExtTile*.jpg` + texturas de Effect/Skill listadas em `待修复日志.md` — decidir se a New-Era gera, recupera ou ignora (placeholders xadrez hoje).
4. **Formatos proprietários de arquivo** a documentar: `.bmd` (modelos), `.ozj/.ozt/.ozb` (texturas empacotadas), `.att` (atributos de mapa), `.smd` (origem 3D), `Enc1/Dec2.dat` — necessário para qualquer pipeline de assets moderno.
5. **Funciona o `Main.exe.original` com o servidor atual?** (binário S5 vs fonte rebaixada) — teste de sanidade antes de planejar substituição.
6. **O que exatamente o Encoder grava no `Main.info`** (estrutura completa) — reversing documentado parcialmente (`Encoder.cpp`), completar o mapa de campos.
7. **Portas/alocação do GameServerCS** e o protocolo de map-move entre GS↔GSCS (`MapServerInfo.dat`) para topologia multi-servidor futura.
8. **Extensões brasileiras do lineage Louis/ReiDoMU** (bots, PCPoint, custom shops) — decidir o que a New-Era preserva.
9. **Viabilidade de build** em toolchain moderna (v142 → v143/cl.cs, x64) — levantamento apenas; sem build nesta fase.
10. **Snapshot/persistência:** o clone (1,7 GB / 18k arquivos) excede a cota de snapshot do workspace (~128 MB/10k arquivos) — o repo pode precisar de re-clone em fases futuras; este relatório + o remote são a fonte da verdade.

---

## 20. Arquivos Mais Importantes (índice rápido)

| Arquivo | Por quê |
|---|---|
| `Source Client/source/Winmain.cpp` | entry point, init, game loop, /test |
| `Source Client/source/ZzzScene.cpp` | Scene/MainScene/**cap 25 FPS** (2516-2524), ticks |
| `Source Client/source/ZzzAI.cpp` (CalcFPS) | timers, WorldTime, DeltaT |
| `Source Client/source/WSclient.cpp` | protocolo clássico cliente (~12k linhas) |
| `Source Client/source/ProtocolSend.{h,cpp}` + `ProtocolAsio.h` | rede moderna ASIO do cliente |
| `Source Client/source/SimpleModulus.h` + `StreamPacketEngine.h` | cifra e framing clássicos (buffer 2048) |
| `Source Client/source/ZzzOpenglUtil.cpp` | contexto GL, pixel format/FSAA |
| `Source Client/source/OpenGL3/Shader/Shader.cpp` | **stub vazio** (estado do "GL3") |
| `Source Server/GameServer/GameServer/GameServer.cpp` | WinMain do GS |
| `Source Server/GameServer/GameServer/SocketManager{,Modern,Udp}.cpp` | IOCP + ASIO + UDP |
| `Source Server/GameServer/GameServer/Protocol.cpp` | dispatch 5.216 linhas |
| `Source Server/GameServer/GameServer/ChaosBox.{h,cpp}` | Chaos Machine completa |
| `Source Server/GameServer/GameServer/CustomMix.cpp` | molde data-driven de mix |
| `Source Server/GameServer/GameServer/Item.h` | CItem + limites de inventory/warehouse |
| `Source Server/GameServer/GameServer/User.h` | MAX_OBJECT/MAX_USER |
| `Source Server/GameServer/GameServer/stdafx.h` | `GAMESERVER_UPDATE 502` e feature flags |
| `Source Server/DataServer/DataServer/QueryManager.cpp` | ODBC direto |
| `Source Tools/Encoder/Encoder/Encoder.cpp` | geração do `Main.info` |
| `MuServer/DB/MuOnline.sql` | schema (74 tabelas) |
| `docs/feature_flags.md` | mapa de flags servidor↔cliente |
| `客户端端与服务端匹配分析报告.md` | matching 60% cliente×servidor |
| `待修复日志.md` | dívida de assets ausentes |
| `analysis_report.md` + `fix_log_v1..v6.md` | história das decisões dos autores |

## 21. Funções Mais Importantes

| Função | Local | Papel |
|---|---|---|
| `WinMain` | `Winmain.cpp:1392` | boot do cliente (Main.info, chaves, config.ini, janela) |
| `Scene(HDC)` | `ZzzScene.cpp:2828` | frame dispatcher por cena |
| `MainScene(HDC)` | `ZzzScene.cpp:2314` | loop lógica 25 Hz + render + **limiter** |
| `CalcFPS` | `ZzzAI.cpp:781` | FPS/WorldTime/DeltaT |
| `ProtocolCompiler` | `WSclient.cpp` | drena fila de rede por frame |
| `CStreamPacketEngine::Send/End/XorData` | `StreamPacketEngine.h` | monta+ cifra pacotes C1–C4 |
| `CSimpleModulus::Encrypt/Decrypt` | `SimpleModulus.h` | cifra dos C3/C4 |
| `CreateSocket` / `SocketClient` | `WSclient.cpp:168` | conexão clássica |
| `gProtocolSend.ConnectServer/RecvMessage` | `ProtocolSend.cpp` | caminho ASIO moderno |
| `CGChaosMixRecv` / `CChaosBox::*Mix` | `ChaosBox.cpp:4359` | Chaos Machine (entrada + ~55 mixes) |
| `CGProtocolCore/DataParse` (switch) | `Protocol.cpp` | dispatch GS |
| `CSocketManager::IOCPAccept/Recv` | `SocketManager.cpp` | accept/IOCP clássico |
| `CQueueTimer::CreateTimer` | `QueueTimer.cpp` | timers de gameplay do GS |
| `QueryManager::Connect/Execute` | `DataServer/QueryManager.cpp` | ODBC |
| `CProtect::ReadMainFile` | cliente `Protect.cpp` + `Encoder.cpp` | boot seguro do cliente |

---

## 22. Licenças & Direitos

| Item | Situação |
|---|---|
| LICENSE do repositório | **Não existe** (nenhum arquivo LICENSE/COPYING na raiz) |
| Código MU (cliente + servidores) | Derivado de **fontes da WebZen vazadas/leaked** e de emuladores da comunidade (lineagem Louis Emulator/creandomu — BR). Sem licença explícita; uso é de risco legal (propriedade intelectual da WebZen/Kru) — **registro factual, sem aconselhamento jurídico** |
| Crypto++ (`Util/cryptopp*`) | Boost Software License 1.0 (`License.txt` incluso) |
| mapm | Licença própria do autor (`DOCS/LICENSE.TXT`) |
| object-pool (dependencies) | LICENSE incluso (MIT-style) |
| Headers Lua 5.1 (PUC-Rio) | Licença MIT da Lua.org (nos headers) |
| Themida SDK headers | SDK comercial (referenciado, não incluso) |
| **Implicação New-Era** | A base é material de pesquisa/auditoria; qualquer produto derivado precisa de decisão deliberada sobre IP (reimplementação limpa vs. uso do que há) |

---

## 23. Próximos Passos Recomendados (fase 0B+)

1. **Congelar a base:** registrar o hash `580472e` como o commit de referência oficial da New-Era; manter o clone fora de alterações (read-only por padrão).
2. **Resolver a persistência do clone** (1,7 GB > cota de snapshot do workspace): re-clone por demanda a partir do GitHub ou mirror seletivo (fontes + configs + docs, sem `Client/Data` de 900 MB).
3. **Deep-dive de protocolo (0B):** tabela bidirecional opcode-a-opcode (cliente clássico × ASIO × servidor) com structs pack(1) — fundação do "protocol spec" da New-Era.
4. **Deep-dive de formatos (0C):** `.bmd/.ozj/.ozt/.att/Enc1/Dec2/Main.info` — sem isso não existe pipeline de assets moderno.
5. **PoC de desacoplamento de loop (0D, fora do repo base):** sandbox separado provando tick fixo + render desacoplado + VSync (o repo permanece intocado, conforme a REGRA ABSOLUTA).
6. **Inventário de assets:** validar os 704 ExtTile ausentes e demais texturas do `待修复日志.md`; decidir estratégia (recuperar do cliente original / gerar / placeholder).
7. **Decisão de escopo do servidor:** o GameServer Louis (S5/EX502) vira "golden reference" de comportamento, ou a New-Era também o moderniza? (recomendado: referência primeiro).
8. **Plano de build reproduzível:** Windows + VS2019 v142 em VM/CI para validar que a base compila exatamente como está, antes de qualquer mudança.

---

## Resultado Esperado — Status

| Item | Status |
|---|---|
| Repositório completo clonado | ✅ 18.070 arquivos, 1,7 GB, branch `main` @ `580472e` |
| Histórico Git preservado | ✅ 17 commits, 2 branches remotas, nada reescrito |
| Cliente identificado | ✅ Main.sln → Main.exe; OpenGL função fixa; loop 25 FPS mapeado |
| Servidor identificado | ✅ GS/GSCS/CS/DS/JS + topologia de portas e protocolos internos |
| Tools identificadas | ✅ Encoder (Main.info), Sync-Version.ps1, .bat de ops, scripts .reg |
| Network identificada | ✅ C1–C4 + SimpleModulus + ASIO moderno; buffers/filas catalogados |
| Lua identificada | ✅ headers 5.1 + 4 scripts **órfãos** (não integrados ao build — achado relevante) |
| FPS/Rendering identificados | ✅ cap 25 FPS em `ZzzScene.cpp:2516-2524`; sem VSync; shader stub |
| Chaos Machine localizada | ✅ `ChaosBox.cpp` (~55 mixes hardcoded) + `CustomMix` data-driven embrionário |
| Items mapeados | ✅ `CItem` + limites versionados + configs `Data/Item/*` |
| Configurações mapeadas | ✅ GameServerInfo*.dat, .ini, Main.info, .reg, Data/*.txt |
| Diagnostics mapeados | ✅ MiniDump (servidores), leaf+CrashReporter+ErrorReport (cliente), logs |
| Dependências identificadas | ✅ necessárias/opcionais/legacy separadas |
| Limitações potenciais catalogadas | ✅ §17 (com local, valor, função, impacto, origem) |
| Documentação existente analisada | ✅ README, CLAUDE.md, 6 fix logs, 2 relatórios ZH, feature_flags.md |
| Nenhuma alteração funcional realizada | ✅ `git status --short` vazio do início ao fim da missão |

---

*Fim do relatório — NEW-ERA SERVERS, Fase 0A. Único artefato novo criado nesta missão (fora da árvore do repositório).*
