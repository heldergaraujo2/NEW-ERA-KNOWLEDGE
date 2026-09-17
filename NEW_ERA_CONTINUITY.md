# NEW-ERA — CONTINUITY

## Canonical repositories
- Research/reference: `wongddd/muonline` @ canonical commit `580472e`.
- Our reconstruction: `heldergaraujo2/NEW-ERA-KNOWLEDGE`.

## Current completed objective
### 1.3-P — Combat action/skill RX (`0x18` / `0x19`)
**STATUS: EXECUTED / DELIVERED / PASS**

The checkpoint listed 1.3-P as the next microtest choice. The first documented branch was executed: ACK/animation-side RX wire codec for action `0x18` and magic/skill `0x19`.

Completed:
- recovered `PRECEIVE_ACTION` wire layout from pinned upstream evidence;
- recovered `PRECEIVE_MAGIC` wire layout from pinned upstream evidence;
- strict C1 parsers for both signals;
- exact 9-byte length/header validation;
- golden vectors for both frames;
- real TCP loopback delivering both frames in sequence;
- field-by-field decode assertions;
- truncation rejection regression;
- CMake target `ts8_combat_action_skill_rx_loopback` with C++17 and Windows `ws2_32` linkage.

Implementation:
`NEW_ERA_IMPLEMENTATION/mvp_login/combat_action_skill_rx.h`

Loopback:
`NEW_ERA_IMPLEMENTATION/mvp_login/loopback_combat_action_skill_rx/test_combat_action_skill_loopback.cpp`

Evidence:
`EVIDENCE/1.3-P_COMBAT_ACTION_SKILL_RX/REPORT.md`

## Verified result
`TS-8 combat action/skill RX loopback: PASS`

Runtime validation: C++17/g++ with `-Wall -Wextra -Wpedantic -pthread`, real TCP loopback on `127.0.0.1`, exit status `0`.

## Exact golden vectors
- `0x18`: `c10918012305780456` → key `0x0123`, angle `0x05`, action `0x78`, target `0x0456`.
- `0x19`: `c10919004201230456` → magic `0x0042`, source `0x0123`, target `0x0456`.

## Boundary
This closes the recovered **wire-level RX codec + executable loopback** for `0x18` and `0x19`. It does not claim the complete upstream animation/action semantic table, original MU executable/renderer execution, original MU server execution, C4/transport decryption, undocumented `ReceiveMagic` side effects, or universal C1 emission for every 0x19 path.

## Previous completed objectives
### INFRA-TS-7 — Test-server CMake integration
**STATUS: EXECUTED / DELIVERED / PASS**

### INFRA-TS-6 — F3:13 multi-accept/reconnect loopback
**STATUS: EXECUTED / DELIVERED / PASS**

### INFRA-TS-5 — F3:13 scripted sequence
**STATUS: EXECUTED / DELIVERED / PASS**

### INFRA-TS-4 — F3:13 through BOTH_MESSAGE
**STATUS: EXECUTED / DELIVERED / PASS**

### 1.3-ZP2 — F3:13 Equipment/CharSet
**STATUS: EXECUTED / DELIVERED**

## Next continuation rule
Proceed to the next still-open implementation/integration item documented by the project checkpoint. Preserve evidence-first methodology and require executable/golden/loopback proof before marking it complete.

## Latest commits
- `bcb9fa468f6274e4925f794d5eb03e6c07d52439` — 1.3-P combat RX codec
- `e21435b3fb0323bb997b8d9025437c3b9eb24c22` — 1.3-P TCP loopback
- `455d968fe8066eba044cf20a25d2857ac24ed7b1` — 1.3-P CMake target
- `134c0871e0d7e34e2d3fd85ee5a05f18c1ebb9b8` — 1.3-P evidence report
