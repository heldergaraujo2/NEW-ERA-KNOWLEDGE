# NEW-ERA — CONTINUITY

## Canonical repositories
- Research/reference: `wongddd/muonline` @ canonical commit `580472e`.
- Our reconstruction: `heldergaraujo2/NEW-ERA-KNOWLEDGE`.

## Current completed objective
### 1.3-P(b) — F3:13 Equipment/Model Apply
**STATUS: EXECUTED / DELIVERED / PASS**

The next implementation item after the F3:13 wire codec was executed: the recovered client-side `ReceiveEquipment` → `ChangeCharacterExt` semantic transformation is now represented as a deterministic, renderer-independent state codec.

Completed:
- recovered `ReceiveEquipment` correlation by character key;
- weapon 0/1 model extraction from CharSet high nibbles + low bytes;
- weapon `0x0FFF` sentinel handling;
- wing selection plus documented byte-15/byte-16 overrides;
- helper/pegasus/dark-horse/fenrir selector state;
- five body-part model extraction with `0x1FF` class-body fallback;
- exact packed level decoding and `LevelConvert` table;
- five body-part `Option1` bits;
- five body-part `ExtOption` bits;
- `ExtendState` bit;
- real TCP loopback carrying the exact 24-byte F3:13 frame;
- field/state assertions;
- regression for both weapon sentinels;
- regression for all five body sentinels;
- CMake target `ts9_f3_13_equipment_apply_loopback`.

Implementation:
`NEW_ERA_IMPLEMENTATION/mvp_login/f3_13_equipment_apply.h`

Loopback:
`NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f3_13_equipment_apply/test_f3_13_equipment_apply_loopback.cpp`

Evidence:
`EVIDENCE/1.3-PB_F3_13_EQUIPMENT_MODEL_APPLY/REPORT.md`

## Verified result
`TS-9 F3:13 equipment/model apply loopback: PASS`

Runtime validation: C++17/g++ with `-Wall -Wextra -Wpedantic -pthread`; CMake configure/build also passed; TCP loopback on `127.0.0.1`; exit status `0`.

## Boundary
This closes the recovered **semantic equipment/model state transform** plus executable loopback. It does not claim original MU renderer execution, original `CHARACTER`/`OBJECT` memory integration, execution of pet/renderer/scale/chaos-castle side effects, or complete resolution of every optional upstream feature macro branch.

## Previous completed objective
### 1.3-P — Combat action/skill RX (`0x18` / `0x19`)
**STATUS: EXECUTED / DELIVERED / PASS**

## Previous infrastructure objectives
- INFRA-TS-7 — Test-server CMake integration: PASS
- INFRA-TS-6 — F3:13 multi-accept/reconnect loopback: PASS
- INFRA-TS-5 — F3:13 scripted sequence: PASS
- INFRA-TS-4 — F3:13 through BOTH_MESSAGE: PASS
- 1.3-ZP2 — F3:13 Equipment/CharSet codec + loopback: PASS

## Next continuation rule
Proceed to the next still-open implementation/integration item documented by the project checkpoint. Preserve evidence-first methodology and require executable/golden/loopback proof before marking it complete.

## Latest commits
- `109c215b99171d0611c554031cbd449cb3f95bc9` — 1.3-P(b) semantic equipment/model codec
- `96b39040a917a2cae67bee467b15582342a2bcce` — 1.3-P(b) TCP loopback
- `65d557b04ce334258dbc19dda48f43a2cd07b46f` — 1.3-P(b) CMake target
- `7102f743c2bf8f25f08508f98ea5fa73b8a988b6` — 1.3-P(b) evidence report
