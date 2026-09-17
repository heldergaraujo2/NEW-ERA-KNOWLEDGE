# NEW-ERA — CONTINUITY

## Canonical repositories
- Research/reference: `wongddd/muonline` @ canonical commit `580472e`.
- Our reconstruction: `heldergaraujo2/NEW-ERA-KNOWLEDGE`.

## Current completed objective
### 1.3-R P3 — BOTH_ATTACK1/2 TCP TX loopback
**STATUS: EXECUTED / DELIVERED / PASS**

Completed:
- real TCP loopback on `127.0.0.1` with ephemeral port;
- `BOTH_ATTACK1 (0x0008)` transmitted through the modern 6-byte LE envelope;
- `BOTH_ATTACK2 (0x0009)` transmitted through the same TCP connection;
- server-side validation of envelope id, body size and exact body bytes;
- client-side byte-for-byte echo validation;
- C++17 warning-enabled build;
- independent CMake configure/build and executable run.

Implementation:
`NEW_ERA_IMPLEMENTATION/mvp_login/loopback_both_attack_tx/test_both_attack_tx_tcp.cpp`

CMake target:
`ts11_both_attack_tx_tcp_loopback`

Evidence:
`EVIDENCE/1.3-RP3/NEW_ERA_1_3_RP3_BOTH_ATTACK_TCP_LOOPBACK.md`

## Verified result
`1.3-RP3 BOTH_ATTACK1/2 TCP TX loopback: PASS`

Golden frames exercised:
- `080007000000c1071107017803`
- `090009000000c309db01090932013c`

Runtime validation: C++17/g++ with `-Wall -Wextra -Wpedantic -pthread`; executable exit status `0`.
CMake configure/build + executable run also passed with exit status `0`.

## Boundary
This closes TCP transport validation of the recovered BOTH_ATTACK1/2 TX core. It does not claim original Windows/MU client execution, original ASIO runtime integration, original GameServer execution, or undocumented gameplay semantics.

## Previous completed objective
### 1.3-R P2 — BOTH_ATTACK1/2 TX core commit + validation
**STATUS: EXECUTED / DELIVERED / PASS**

Implementation:
`NEW_ERA_IMPLEMENTATION/mvp_login/both_attack_tx.h`

Regression:
`NEW_ERA_IMPLEMENTATION/mvp_login/loopback_both_attack_tx/test_both_attack_tx.cpp`

Evidence:
`EVIDENCE/1.3-RP2/NEW_ERA_1_3_RP2_CORE_COMMIT_AND_VALIDATION.md`

## Earlier completed objectives
- 1.3-P(b) — F3:13 Equipment/Model Apply: PASS
- 1.3-P — Combat action/skill RX (`0x18` / `0x19`): PASS
- INFRA-TS-7 — Test-server CMake integration: PASS
- INFRA-TS-6 — F3:13 multi-accept/reconnect loopback: PASS
- INFRA-TS-5 — F3:13 scripted sequence: PASS
- INFRA-TS-4 — F3:13 through BOTH_MESSAGE: PASS
- 1.3-ZP2 — F3:13 Equipment/CharSet codec + loopback: PASS

## Next continuation rule
Proceed to the next still-open implementation/integration item documented by the project checkpoint or the latest evidence chain. Preserve evidence-first methodology and require executable/golden/loopback proof before marking it complete.

## Latest commits
- `e76cfae5d4cc0c37f9ce32d5b5b427e04f868639` — 1.3-RP3 TCP loopback initial implementation
- `a64d87953cfd72e52c41320445b06454f3c0e257` — corrected two-frame TCP validation
- `17f516396751710bf8e9c7fbbd2f513685ba1835` — CMake TS11 integration
- `3e7f477e771c127222d08a46794d5244d4965959` — evidence report
