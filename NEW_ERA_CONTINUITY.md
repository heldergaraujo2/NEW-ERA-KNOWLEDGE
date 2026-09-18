# NEW-ERA — CONTINUITY

## Canonical repositories
- Research/reference: `wongddd/muonline` @ canonical commit `580472e`.
- Our reconstruction: `heldergaraujo2/NEW-ERA-KNOWLEDGE`.

## Current completed objective
### 1.0-B — F1:01 Login Request builder + C3/TCP loopback
**STATUS: EXECUTED / DELIVERED / PASS (test-key scope)**

Completed:
- recovered 50-byte pre-SimpleModulus C1 construction (49 logical fields + packet serial);
- BuxConvert `{FC,CF,AB}` on ID/password;
- TickCount LE;
- Version[5] transform;
- 16-byte protocol serial;
- proven 32-byte chained stream XOR;
- C3 framing after SimpleModulus;
- local decrypt/stream-XOR regression;
- real TCP loopback;
- CMake target `ts13_f1_01_login_request_loopback`.

Implementation:
`NEW_ERA_IMPLEMENTATION/mvp_login/f1_01_login_request.h`

Regression:
`NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_01_login_request/test_f1_01_login_request_loopback_v2.cpp`

Evidence:
`EVIDENCE/1.0-B_F1_01_LOGIN_REQUEST/REPORT.md`

## Verified result
`TS-13 F1:01 login request C3 loopback: PASS`

Golden C3 frame:
`c34f5f3d11d746df2080285164025c2e47889de32a5493a667c1011a474e31f0f0cbfefee019a1cabfb28c94af9a135a29ed4a3ac096e8e8dd927310680985132bb4d3e69b081cca8bb52043d02619`

Runtime: recreated C++17 fixture, `g++ -std=c++17 -Wall -Wextra -Wpedantic -pthread`, real TCP loopback, exit 0.

## Boundary
Production-key interoperability is NOT claimed in this objective because the manifest-referenced binary `Enc1.dat`/`Dec2.dat` was not exposed by the available GitHub file interface during this run. The implementation requires real Enc1 keys for production C3 generation; the executable proof injects deterministic test keys. Original Windows/MU/ASIO/GameServer runtime integration remains unverified.

## Previous completed objective
### 1.3-T — F1:00 ServerVersion implementation + TCP loopback
**STATUS: EXECUTED / DELIVERED / PASS**

## Earlier completed objectives
- 1.3-R P3 — BOTH_ATTACK1/2 TCP TX loopback: PASS
- 1.3-S P2 — Combat Action/Skill RX: PASS
- 1.3-R P2 — BOTH_ATTACK1/2 TX core: PASS
- 1.3-P(b) — F3:13 Equipment/Model Apply: PASS
- INFRA-TS-7/6/5/4 — test-server integration and F3:13 loopbacks: PASS
- 1.3-ZP2 — F3:13 Equipment/CharSet codec + loopback: PASS

## Next continuation rule
Proceed to the next still-open implementation/integration item documented by the project checkpoint or latest evidence chain. Preserve evidence-first methodology and require executable/golden/loopback proof before marking it complete.

## Latest commits
- `17d833ad93fa9a5cb5f2f9f300925bedc8e2229a` — F1:01 builder
- `b905322508a3c4edeabe884847b5fed4a3dc46aa` — initial TCP regression
- `0cfa6db81ccc214486d4876388b8c48b8103dd78` — corrected TCP regression
- `950515a66c23e79c5679ec605a482d6691b5fce5` — CMake TS13
- `6d56ca369c3fb7d8a9c18432d41422f84ffa9a30` — evidence

## Latest completed objective
### 1.0-C — F1:02 Logout Request builder + C3/TCP loopback
**STATUS: EXECUTED / DELIVERED / PASS (test-key scope)**

Completed:
- recovered upstream SendRequestLogOut emitter from pinned wongddd/muonline@580472e;
- exact logical packet [C1][05][F1][02][Flag];
- proven chained XOR applied to the payload bytes;
- SimpleModulus C3 builder with loaded-key guard;
- deterministic golden vector and inverse-key decrypt regression;
- real TCP loopback with exact 13-byte frame comparison;
- CMake target ts14_f1_02_logout_request_loopback.

Implementation:
NEW_ERA_IMPLEMENTATION/mvp_login/f1_02_logout_request.h

Regression:
NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_02_logout_request/test_f1_02_logout_request_loopback.cpp

Evidence:
EVIDENCE/1.0-C_F1_02_LOGOUT_REQUEST/REPORT.md

Verified result:
TS-14 F1:02 logout request C3 TCP loopback: PASS

Golden vectors:
- logical: c105f17a9c
- C3: c30dae5337214b49224d54132b

Runtime: C++17/g++ with warnings enabled, real TCP loopback on 127.0.0.1, plus independent CMake configure/build fixture; exit 0.

Boundary: production Enc1.dat interoperability remains unverified because the available GitHub interface cannot expose that binary as UTF-8; original Windows/MU/ASIO runtime integration remains unverified.

## Latest commits
- e1e688b4897dbc24cc1dfc7597decb23a8413874 — F1:02 builder
- a4cbee05a7d5251a687c78c91168a6bdd1c31b75 — F1:02 TCP regression
- 2fc3837ddac7b593b1c5da7834281cf897705441 — CMake TS14
- 946d79bca1f7e96e181a368d7f3719f8fadc636c — F1:02 evidence


### 1.0-D — F1:03 Hacking Check Request builder + C3/TCP loopback
**STATUS: EXECUTED / DELIVERED / PASS (test-key scope)**

Implemented from pinned upstream SendHackingChecked(byType, byParam): exact logical [C1][06][F1][03][Type][Param], proven chained XOR, SimpleModulus C3 builder, deterministic golden vector, and real TCP loopback. CMake target: ts15_f1_03_hacking_check_loopback.

Implementation: NEW_ERA_IMPLEMENTATION/mvp_login/f1_03_hacking_check_request.h
Regression: NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_03_hacking_check/test_f1_03_hacking_check_loopback.cpp
Evidence: EVIDENCE/1.0-D_F1_03_HACKING_CHECK/REPORT.md

Verified result: TS-15 F1:03 hacking-check C3 TCP loopback: PASS; exit 0.
Golden logical vector: c106f17bd553. Production-key interoperability and original Windows/MU/ASIO runtime remain unverified.

Latest commits: ae61fdec9d1e25beae5e024876af5256c2937bb7 (core); a76965c2d2f97b21fc3641a552f4e9d7c933f2c4 (golden correction); 6f8bd189d63092aef9dca7017d2654e4f07fec8b (CMake); b7656909e9420688e16f08b1727a3c2e621b4231 (evidence).


## Latest completed objective
### 1.0-E — F1 RX handshake validation + F1:01 login result parser
**STATUS: EXECUTED / DELIVERED / PASS**

Implemented client-side RX closure for the normative login flow: F1:00 Version[i]-(i+1) validation and F1:01 C1 login-result parsing/mapping. Added truncation/version-mismatch negatives, compatibility 0x20 success, generic fallback, real TCP loopback, and CMake target `ts16_f1_login_rx_loopback`.

Implementation: `NEW_ERA_IMPLEMENTATION/mvp_login/f1_login_rx.h`
Regression: `NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_login_rx/test_f1_login_rx_loopback.cpp`
Evidence: `EVIDENCE/1.0-E_F1_RX/REPORT.md`

Verified result: `TS-16 F1:00 validation + F1:01 result RX TCP loopback: PASS`; exit 0.
Golden vectors: F1:00 `c10cf100011234342e2f3d3d`; F1:01 success `c105f10101`; compatibility success `c105f10120`.

Validation was a recreated C++17 fixture because the execution container did not have a repository checkout/network; it used the exact committed core/test content and real TCP loopback on 127.0.0.1. No original Windows/MU runtime integration or production-key interoperability claim is made.


## 1.0-F2 / 1.0-F3 — F1 client flow composition (2026-09-18)
- 1.0-F2: F1 login session state machine implemented: F1:00 handshake/version/hero-key -> AwaitingResult -> F1:01 success/rejection. Core f1_login_session.h; regression ts17_f1_login_session_loopback; evidence EVIDENCE/1.0-F2_F1_LOGIN_SESSION/REPORT.md. Commits: 3f3ee48c68f8380e744ac5e0085e0a8dd61477a6, 37835e881cfa4510448c8561d4bdc4d2e49db599, f334c56e76e1b436f38d1faf80b0a11180aa0c50, 906aa8daff8f1dc147d5eb2d0586988bcf7ccd21.
- 1.0-F3: F1 end-to-end flow coordinator implemented, composing F1:00 RX + F1:01 C3 TX + F1:01 RX. Core f1_login_flow.h; regression ts18_f1_login_flow_loopback; evidence EVIDENCE/1.0-F3_F1_END_TO_END_FLOW/REPORT.md. Commits: 1ce8b0bc4994aede4b07dc501f2479c19fd8f317, b77b291640ed2980b1643799d89f208f28a5932b, eb3cf14966396eb4d1858bfb5177caa16fc3b593, db75d22405412cf751eb506ff6d147d99ad765cb.
- Validation boundary: TS-18 orchestration was validated with a recreated TCP fixture using the already-proven TS-13 79-byte C3 builder contract; full repository checkout/CMake execution was unavailable in the execution container. Production Enc1.dat and original Windows/MU runtime remain unverified.



## 1.0-F4 — F1 live TCP flow integration (2026-09-18)
**STATUS: EXECUTED / DELIVERED / PASS (deterministic test-key scope)**

Completed:
- real TCP client/server loopback using the committed LoginFlow;
- exact F1:00 handshake RX, Version transform validation, HeroKey capture;
- committed F1:01 BuildC3 invoked through LoginFlow (no stub);
- 79-byte C3 wire framing validated (C3 4F + 77-byte ciphertext);
- exact F1:01 success RX and final LoggedIn state;
- CMake target `ts19_f1_login_flow_tcp_loopback`;
- evidence `EVIDENCE/1.0-F4_F1_LIVE_TCP_FLOW/REPORT.md`.

Verified output:
`TS-19 F1 login flow real TCP integration: PASS`

Validation: C++17/g++ and an independent CMake fixture, warnings enabled, real TCP loopback on 127.0.0.1, exit 0. The execution container lacked a repository checkout, so the committed core and TS-19 source were reproduced into the validation fixture.

Boundary: deterministic injected SimpleModulus test keys only; production Enc1.dat interoperability, original Windows/MU client/ASIO path, original GameServer, and server-side credential authentication remain unverified.


## 1.0-F5 — F1:01 packet-serial placement correction (2026-09-18)
**STATUS: EXECUTED / DELIVERED / PASS**

A critical protocol-fidelity correction was completed after re-reading the pinned upstream wsclientinline.h SendPacket implementation. For C1 packets, upstream inserts g_byPacketSerialSend++ into byte [1] (replacing the C1 size byte) and then calls SimpleModulus on byBuffer + 1 for len - 1 bytes. Therefore the C1 header byte [0] is not encrypted, packet serial [1] is the first encrypted byte, and the chained XOR remains [3..49).

Corrected:
- NEW_ERA_IMPLEMENTATION/mvp_login/f1_01_login_request.h
- NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_01_login_request/test_f1_01_login_request_loopback_v2.cpp
- NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md
- EVIDENCE/1.0-B_F1_01_LOGIN_REQUEST/REPORT.md

Corrected deterministic C3 golden:
c34f0de31e1a4537821084daef63cf193a06e9118ad4d9ecae551825c3a8e23bc4093c87c61b5c4c8403a55c10254f4f154a8c97036638f7c2a29908798c2300c71cc9fc0a0409050aa2421cd86f53

Validation: standalone C++17 fixture, -Wall -Wextra -Wpedantic -pthread, SimpleModulus round-trip with inverse deterministic key, exact golden comparison, and real TCP loopback. Result: TS-20 F1:01 packet-serial placement + C3 TCP loopback: PASS, exit 0.

This correction supersedes the earlier incorrect TS-13 golden vector. Production Enc1.dat interoperability and original Windows/MU/GameServer runtime remain unverified.

## 1.0-F6 — production Enc1.dat vector closure (2026-09-18)
**STATUS: EXECUTED / DELIVERED / PASS**

The real pinned upstream Client/Data/Enc1.dat blob was fetched from wongddd/muonline@580472e as base64, decoded unchanged, and SHA256-verified as ee73a59bf278d42735e537c96c25cc1ad0f69f1de089a7b20a1e6d2910a4372d (54 bytes). The existing loader parsed it successfully; BuildC3 then generated the deterministic F1:01 production-key golden:
c34f1fac23f486f52295d0daefbe0868fac969b3b604d9ecead643f6163f566f2c093ccd111749d3502568e4102542337e4d96cf03b064f7c293bc61e5caf8e00a15c9fc2e397d52c62f214b046f53

Regression: NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_01_login_request/test_f1_01_production_enc1_vector.cpp
Evidence: EVIDENCE/1.0-F6_PRODUCTION_ENC1/REPORT.md

Verified output: TS-21 F1:01 production Enc1 loader + C3 golden: PASS

Validation: C++17, -Wall -Wextra -Wpedantic -pthread, exact 54-byte production blob fixture, loader/key assertions, exact 79-byte golden comparison. Production-key crypto interoperability is now verified at the deterministic vector level. Original Windows/MU/GameServer runtime interoperability remains unverified.


## 1.0-F7 — CMake integration of TS-21 production Enc1 vector (2026-09-18)
**STATUS: EXECUTED / DELIVERED**

The already verified TS-21 production Enc1/F1:01 deterministic regression is now integrated into `NEW_ERA_IMPLEMENTATION/test_server/CMakeLists.txt` as target `ts21_f1_01_production_enc1_vector`, using the committed source `NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_01_login_request/test_f1_01_production_enc1_vector.cpp`, C++17 and `ws2_32` on Windows.

Evidence: `EVIDENCE/1.0-F7_CMAKE_TS21/REPORT.md`.

Commit: `c8149f6064221374345c550fa9e118866b5990ea`.

Validation boundary: no repository checkout/network in the execution container, so no fresh full-repository CMake execution is claimed. TS-21's previously recorded standalone C++17 production-key vector remains PASS. Original Windows/MU/GameServer interoperability remains unverified.


## 1.0-F8 — Production Enc1 F1:01 TCP loopback (2026-09-18)
**STATUS: EXECUTED / DELIVERED — implementation complete**

Added `test_f1_01_production_enc1_tcp_loopback.cpp`, which uses the exact production Enc1 fixture and committed `BuildC3`, then sends the resulting 79-byte F1:01 C3 frame over a real 127.0.0.1 TCP connection and validates an exact byte-for-byte echo. Added CMake target `ts22_f1_01_production_enc1_tcp_loopback`.

Commits: implementation `66e105919702bd2cae97e47ac4af934b5f5c22a9`; CMake corrected target `611eab7444f5bddd7fe289d269f81b42136427ff`; evidence `f6235040ac53effc48849a9b7f8a51634801a7b1`.

Validation boundary: execution container lacks repository checkout/network, so no fresh full-repository CMake run is claimed. Underlying production-key golden remains independently proven by TS-21. Original Windows/MU/GameServer interoperability remains unverified.


## 1.0-F9 — F1:01 server-side RX decoder (2026-09-18)
**STATUS: EXECUTED / DELIVERED — production-key round-trip proven by independent executable fixture**

Completed the server-side cryptographic/protocol RX layer for F1:01. Decoder: C3 validation → classic SimpleModulus decrypt → 50-byte logical C1 reconstruction with serial at byte 1 → reverse chained XOR → BuxDecrypt ID/password → extraction of TickCount, Version[5], ProtocolSerial[16], packet serial.

Implementation: NEW_ERA_IMPLEMENTATION/mvp_login/f1_01_login_server_rx.h
Regression: NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_01_login_request/test_f1_01_server_rx_production_loopback.cpp
CMake target: ts23_f1_01_server_rx_production_loopback
Evidence: EVIDENCE/1.0-F9_SERVER_RX_F1_01/REPORT.md

Key basis: KEYS_MANIFEST.md proves Dec1 is the server-RX inverse-key partner of Enc1 with identical Modulus/Xor. Test derives the Dec1 lane from the already verified production Enc1 fixture; no key invention.

Validation: C++17 decoder syntax/compile fixture PASS; independent executable production-key encrypt/decrypt/stream-XOR/Bux round-trip PASS with all selected fields recovered exactly. Full repository CMake execution remains unclaimed because the execution container has no checkout/network.

Boundary: original GameServer authentication/database/JoinServer decision code and original Windows runtime remain separate layers.


## 1.0-F10 — F1 server login decision/result layer (2026-09-18)
**STATUS: EXECUTED / DELIVERED / PASS — compatible reconstruction layer**

Implemented deterministic server-side validation/result mapping behind the proven F1:01 decoder. Validates non-empty credentials, non-zero packet serial, expected Version and ProtocolSerial when configured; maps documented JoinServer result values 0..4 directly to F1:01 result bytes; emits result 6 for version/serial failure.

Implementation: NEW_ERA_IMPLEMENTATION/mvp_login/f1_login_server_decision.h
Regression: NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_login_server_decision/test_f1_login_server_decision.cpp
CMake target: ts24_f1_login_server_decision
Evidence: EVIDENCE/1.0-F10_SERVER_LOGIN_DECISION/REPORT.md

Validation: recreated C++17 fixture with -Wall -Wextra -Wpedantic executed successfully. Observed: TS-24 F1 server login decision/result mapping: PASS. Full repository CMake execution remains unclaimed because the execution container has no checkout/network.

Boundary: compatible decision-layer reconstruction only; original GameServer database/account/connection-state/JoinServer internals remain unverified.


## 1.0-F11 — F1 production login TCP integration (2026-09-18)
**STATUS: EXECUTED / DELIVERED — runtime PASS not claimed**

Implemented TS-25 composition of production Enc1 F1:01 TX, server Dec1 RX, and server login decision/result over one TCP connection.

Source: NEW_ERA_IMPLEMENTATION/test_server/ts25_f1_production_login_tcp_integration.cpp
CMake: ts25_f1_production_login_tcp_integration
Evidence: EVIDENCE/1.0-F11_PRODUCTION_LOGIN_TCP_INTEGRATION/REPORT.md

The committed source uses the exact production Enc1 fixture, derives Dec1 from the committed inverse-key routine, decodes the 79-byte F1:01 C3 request, validates version/serial/credentials, and returns c105f10101. It includes POSIX/WinSock handling and Windows ws2_32 linkage.

Validation boundary: this environment has no repository checkout/network, so TS-25 was not executed this turn. Therefore no PASS claim is made. TS-21/TS-22 independently prove production F1:01 TX; TS-24 proves decision/result mapping.
