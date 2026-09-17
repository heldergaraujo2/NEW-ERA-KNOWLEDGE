# NEW-ERA — CONTINUITY

## Canonical repositories
- Research/reference: `wongddd/muonline` @ canonical commit `580472e`.
- Our reconstruction: `heldergaraujo2/NEW-ERA-KNOWLEDGE`.

## Current completed objective
### 1.3-R P2 — BOTH_ATTACK1/2 TX core commit + validation
**STATUS: EXECUTED / DELIVERED / PASS**

Completed:
- isolated deterministic `BOTH_ATTACK1 (0x0008)` builder;
- isolated deterministic `BOTH_ATTACK2 (0x0009)` builder;
- exact 6-byte little-endian modern envelope;
- exact normative body layouts;
- golden-byte regression against the documented vectors;
- guards for zero index, invalid direction, zero count and invalid serial;
- CMake target `ts10_both_attack_tx_loopback`;
- executable C++17 validation.

Implementation:
`NEW_ERA_IMPLEMENTATION/mvp_login/both_attack_tx.h`

Regression:
`NEW_ERA_IMPLEMENTATION/mvp_login/loopback_both_attack_tx/test_both_attack_tx.cpp`

Evidence:
`EVIDENCE/1.3-RP2/NEW_ERA_1_3_RP2_CORE_COMMIT_AND_VALIDATION.md`

## Verified result
`1.3-RP2 BOTH_ATTACK1/2 core golden+guards: PASS`

Golden frames:
- `080007000000c1071101017803`
- `090009000000c309db00010932013c`

Runtime validation: C++17/g++ with `-Wall -Wextra -Wpedantic`; CMake configure/build and executable run passed with exit status `0`.

## Boundary
This closes the isolated core and executable regression for the recovered BOTH_ATTACK1/2 wire builders. It does not claim original Windows/MU renderer execution, original ASIO runtime integration, or undocumented semantic behavior.

## Previous completed objective
### 1.3-P(b) — F3:13 Equipment/Model Apply
**STATUS: EXECUTED / DELIVERED / PASS**

## Earlier objectives
- 1.3-P — Combat action/skill RX (`0x18` / `0x19`): PASS
- INFRA-TS-7 — Test-server CMake integration: PASS
- INFRA-TS-6 — F3:13 multi-accept/reconnect loopback: PASS
- INFRA-TS-5 — F3:13 scripted sequence: PASS
- INFRA-TS-4 — F3:13 through BOTH_MESSAGE: PASS
- 1.3-ZP2 — F3:13 Equipment/CharSet codec + loopback: PASS

## Next continuation rule
Proceed to the next still-open implementation/integration item documented by the project checkpoint. Preserve evidence-first methodology and require executable/golden/loopback proof before marking it complete.

## Latest commits
- `be98b2830754ca5b5b7c4c533b70a3061955ea0c` — 1.3-RP2 isolated BOTH_ATTACK1/2 core
- `f866320f61f59d3246b3ffdce32dedf52df3de21` — 1.3-RP2 golden/guard regression
- `5f2bbb7fb32f671b07b4c638d04b2cbf0d328be9` — corrected normative golden vectors
- `a32c2a3ec44cf2b8196acb3a49384dfbfc8c8569` — CMake integration
- `355f37cdd5dbb00de007c1e2ffa22d8e2f784787` — evidence report
