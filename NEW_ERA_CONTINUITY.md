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
