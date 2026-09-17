# NEW-ERA — 1.3-R P2 Core Commit + Validation

Status: **EXECUTED / DELIVERED / PASS**

## Scope
Formalize BOTH_ATTACK1/2 modern (olc/ASIO) TX as isolated NEW-ERA implementation core, with normative golden-byte regression and CMake integration.

## Normative evidence
`NEW_ERA_PROTOCOL_MVP_BOTH_ATTACK_TX_SPEC.md` defines:
- `BOTH_ATTACK1 = 0x0008`, header 6 B LE + body 7 B = 13 B total;
- `BOTH_ATTACK2 = 0x0009`, header 6 B LE + body 9 B = 15 B total;
- ATTACK1 body `C1 07 11 + index BE + action + dir`;
- ATTACK2 body `C3 09 DB + skillH + count + skillL + x + serial + y`;
- guards: ATTACK1 index != 0 and dir <= 7; ATTACK2 count >= 1 and serial 1..50.

Prior 1.3-RP2 evidence recorded the same golden frames and a real TCP loopback.

## Core delivered
`NEW_ERA_IMPLEMENTATION/mvp_login/both_attack_tx.h`

Deterministic builders:
- `BuildAttack1(...)`
- `BuildAttack2(...)`

They emit the 6-byte little-endian modern envelope and exact documented body layout. No renderer or undocumented engine side effects are introduced.

## Golden regression
`NEW_ERA_IMPLEMENTATION/mvp_login/loopback_both_attack_tx/test_both_attack_tx.cpp`

Verified vectors:
- BOTH_ATTACK1: `080007000000c1071101017803`
- BOTH_ATTACK2: `090009000000c309db00010932013c`

Negative guards verified:
- ATTACK1 index 0 rejected;
- ATTACK1 dir 8 rejected;
- ATTACK2 count 0 rejected;
- ATTACK2 serial 0 rejected;
- ATTACK2 serial 51 rejected.

Runtime validation with C++17, `-Wall -Wextra -Wpedantic`: **PASS**.

Output:
`1.3-RP2 BOTH_ATTACK1/2 core golden+guards: PASS`

## CMake
Added target `ts10_both_attack_tx_loopback`.

CMake configure/build and executable run were independently validated with exit status 0.

## Boundary
This closes the isolated core/commit and executable regression for the recovered BOTH_ATTACK1/2 wire builders. It does not claim original Windows/MU renderer execution, original ASIO runtime integration, or undocumented semantic behavior beyond the recovered wire contract.
