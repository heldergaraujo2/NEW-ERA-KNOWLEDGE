# NEW-ERA — CONTINUITY

## Canonical repositories
- Research/reference: `wongddd/muonline` @ canonical commit `580472e`.
- Our reconstruction: `heldergaraujo2/NEW-ERA-KNOWLEDGE`.

## Current completed objective
### 1.3-T — F1:00 ServerVersion implementation + TCP loopback
**STATUS: EXECUTED / DELIVERED / PASS**

Completed:
- isolated 12-byte C1 F1:00 builder/parser;
- Result + HeroKey + five opaque version bytes;
- golden vector and exact-byte regression;
- negative truncated-frame regression;
- real TCP loopback on `127.0.0.1` with ephemeral port;
- CMake target `ts12_f1_00_server_version_loopback`;
- C++17 warning-enabled validation.

Implementation:
`NEW_ERA_IMPLEMENTATION/mvp_login/f1_00_server_version.h`

Regression:
`NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_00_server_version/test_f1_00_server_version_loopback.cpp`

Evidence:
`EVIDENCE/1.3-T-IMPLEMENTATION/REPORT.md`

## Verified result
`1.3-T F1:00 ServerVersion TCP loopback: PASS`

Golden frame:
- `c10cf1000112343530324142`

Runtime validation: C++17/g++ with `-Wall -Wextra -Wpedantic -pthread`; exit status `0`.
CMake configure/build fixture + executable run also passed with exit status `0`.

## Boundary
This closes the recovered F1:00 wire codec and local TCP transport validation. It does not claim execution of the original Windows/MU client, original GameServer process, original ASIO runtime, or undocumented server-version semantics beyond the recovered fields.

## Previous completed objectives
### 1.3-R P3 — BOTH_ATTACK1/2 TCP TX loopback
**STATUS: EXECUTED / DELIVERED / PASS**

Implementation:
`NEW_ERA_IMPLEMENTATION/mvp_login/loopback_both_attack_tx/test_both_attack_tx_tcp.cpp`

Evidence:
`EVIDENCE/1.3-RP3/NEW_ERA_1_3_RP3_BOTH_ATTACK_TCP_LOOPBACK.md`

### 1.3-S P2 — Combat Action/Skill RX
**STATUS: EXECUTED / DELIVERED / PASS**

Implementation:
`NEW_ERA_IMPLEMENTATION/mvp_login/combat_action_skill_rx.h`

Evidence:
`EVIDENCE/1.3-SP2/NEW_ERA_1_3_SP2_CORE_COMMIT_AND_VALIDATION.md`

## Earlier completed objectives
- 1.3-R P2 — BOTH_ATTACK1/2 TX core commit + validation: PASS
- 1.3-P(b) — F3:13 Equipment/Model Apply: PASS
- 1.3-P — Combat action/skill RX (`0x18` / `0x19`): PASS
- INFRA-TS-7 — Test-server CMake integration: PASS
- INFRA-TS-6 — F3:13 multi-accept/reconnect loopback: PASS
- INFRA-TS-5 — F3:13 scripted sequence: PASS
- INFRA-TS-4 — F3:13 through BOTH_MESSAGE: PASS
- 1.3-ZP2 — F3:13 Equipment/CharSet codec + loopback: PASS

## Next continuation rule
Proceed to the next still-open implementation/integration item documented by the project checkpoint or latest evidence chain. Preserve evidence-first methodology and require executable/golden/loopback proof before marking it complete.

## Latest commits
- `975c0fe7daeeff2b5c5820a895def3c14f74822c` — F1:00 ServerVersion codec
- `6820b22155409e823a5075049d4ce01bd2a566fb` — F1:00 TCP loopback
- `1c3a0603b34bf7cdac63b5f7c3878bfff5b05226` — CMake TS12 integration
- `d8797b84de33bed65d19e9112f72b7e5e5061a56` — F1:00 evidence
