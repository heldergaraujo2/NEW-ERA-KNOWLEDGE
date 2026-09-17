# NEW-ERA — CONTINUITY

## Canonical repositories
- Research/reference: `wongddd/muonline` @ canonical commit `580472e`.
- Our reconstruction: `heldergaraujo2/NEW-ERA-KNOWLEDGE`.

## Current completed objective
### INFRA-TS-4 — F3:13 through BOTH_MESSAGE
**STATUS: EXECUTED / DELIVERED**

Completed:
- modern envelope `id:u16 LE + size:u32 LE`;
- `BOTH_MESSAGE (0x000C)` tunnel;
- exact 24-byte F3:13 C1 body preservation;
- TCP client/server loopback;
- parser round-trip of index and all 18 CharSet bytes;
- regression checks for modern and inner framing;
- evidence report.

## Current work advanced
### INFRA-TS-5 — F3:13 scripted sequence
**STATUS: IMPLEMENTED / RUNTIME VALIDATION PENDING**

Implementation:
`NEW_ERA_IMPLEMENTATION/test_server/ts5_f3_13_scripted_sequence_loopback.cpp`

Evidence:
`EVIDENCE/test_server/TS-5_F3_13_SCRIPTED_SEQUENCE/REPORT.md`

The harness models the test-server trigger point and sends three deterministic F3:13 responses through `BOTH_MESSAGE`, validating the 6-byte modern envelope, 24-byte C1 body, parser round-trip, index, and all 18 CharSet bytes.

Runtime validation is deliberately not marked PASS because this environment has no available C++ execution runtime or repository GitHub Actions workflow that can provide an exit-code result.

## Previous verified result
`TS-4 F3:13 BOTH_MESSAGE loopback: PASS`

TS-4 runtime validation: C++17/g++ with `-Wall -Wextra -Wpedantic -pthread`, TCP loopback on `127.0.0.1`, exit status `0`.

## Boundary
The project does not claim integration with the original MU executable or client renderer without corresponding evidence.

## Next continuation rule
Resume by executing/validating INFRA-TS-5 where a C++ runtime is available. If it passes, promote TS-5 to EXECUTED/DELIVERED and continue to the next still-open implementation/integration item. Preserve evidence-first methodology.

## Latest commits
- `c35fbae053618adfe62393014197f86efc376dd9` — TS-4 loopback implementation
- `70b0ebd24cf5539291d937a9e310b9812afcb3` — TS-4 evidence report
- `5c1847e8df35fd00f956f4ca294314453af9513a` — TS-5 scripted sequence implementation
- `06fded054d845f5944b123779920476beb99b9f5` — TS-5 evidence report
