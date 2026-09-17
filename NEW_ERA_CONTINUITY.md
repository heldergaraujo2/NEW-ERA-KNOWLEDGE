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

Implementation: `NEW_ERA_IMPLEMENTATION/test_server/ts4_f3_13_both_message_loopback.cpp`
Evidence: `EVIDENCE/test_server/TS-4_F3_13_BOTH_MESSAGE_LOOPBACK/REPORT.md`

## Verified result
`TS-4 F3:13 BOTH_MESSAGE loopback: PASS`

Runtime validation: C++17/g++ with `-Wall -Wextra -Wpedantic -pthread`, TCP loopback on `127.0.0.1`, exit status `0`.

## Boundary
This proves F3:13 through the NEW-ERA BOTH_MESSAGE envelope and TCP transport. It does not claim integration with the original MU executable or client renderer.

## Previous objective
### 1.3-ZP2 — F3:13 Equipment/CharSet
**STATUS: EXECUTED / DELIVERED**

## Next objective
Proceed to the next still-open implementation/integration item after INFRA-TS-4. Preserve evidence-first methodology and require executable/golden/loopback proof before marking it complete.

## Latest TS-4 commits
- `c35fbae053618adfe62393014197f86efc376dd9` — loopback implementation
- `70b0ebd24cf553929ab1d937a9e310b9812afcb3` — evidence report
