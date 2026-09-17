# NEW-ERA — CONTINUITY

## Canonical repositories
- Research/reference: `wongddd/muonline` @ canonical commit `580472e`.
- Our reconstruction: `heldergaraujo2/NEW-ERA-KNOWLEDGE`.

## Current completed objective
### 1.3-S P2 — Combat Action/Skill RX
**STATUS: EXECUTED / DELIVERED / PASS**

Completed:
- isolated parser for classic C1 `0x18` action;
- isolated parser for classic C1 `0x19` magic/skill;
- exact 9-byte frame validation;
- golden-byte vectors and field assertions;
- negative truncated-frame regression;
- real TCP loopback over one local connection;
- CMake target `ts8_combat_action_skill_rx_loopback`;
- C++17 warning-enabled validation.

Implementation:
`NEW_ERA_IMPLEMENTATION/mvp_login/combat_action_skill_rx.h`

Regression:
`NEW_ERA_IMPLEMENTATION/mvp_login/loopback_combat_action_skill_rx/test_combat_action_skill_loopback.cpp`

Evidence:
`EVIDENCE/1.3-SP2/NEW_ERA_1_3_SP2_CORE_COMMIT_AND_VALIDATION.md`

## Verified result
`TS-8 combat action/skill RX loopback: PASS`

Golden frames:
- `c10918012305780456`
- `c10919004201230456`

Runtime validation: C++17/g++ with `-Wall -Wextra -Wpedantic -pthread`; exit status `0`.
CMake configure/build + executable run also passed with exit status `0`.

## Boundary
This closes the recovered wire parser and executable loopback for `0x18/0x19`. It does not claim a complete semantic animation/action table, undocumented `ReceiveMagic` side effects, original Windows/MU renderer execution, or original runtime integration.

## Previous completed objective
### 1.3-R P3 — BOTH_ATTACK1/2 TCP TX loopback
**STATUS: EXECUTED / DELIVERED / PASS**

Implementation:
`NEW_ERA_IMPLEMENTATION/mvp_login/loopback_both_attack_tx/test_both_attack_tx_tcp.cpp`

Evidence:
`EVIDENCE/1.3-RP3/NEW_ERA_1_3_RP3_BOTH_ATTACK_TCP_LOOPBACK.md`

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
- `29bc7d6cb9bc1641e00c3564140e1f030f52890a` — 1.3-SP2 evidence report
- `9d4ae243d69234e278eca6a9153e5ae5d7a6dcde` — 1.3-RP3 continuity
- `3e7f477e771c127222d08a46794d5244d4965959` — 1.3-RP3 evidence
