# NEW-ERA 1.3-P(b) — F3:13 Equipment/Model Apply

## Status
**EXECUTED / DELIVERED / PASS**

## Objective
Close the next implementation step after the F3:13 wire codec: reproduce the recovered client-side semantic transformation performed by `ReceiveEquipment` → `ChangeCharacterExt`, without pretending to recreate the original renderer or undocumented engine side effects.

## Evidence source
Pinned research repository: `wongddd/muonline` @ `580472e`.

The recovered client path is:

```cpp
ReceiveEquipment(BYTE *ReceiveBuffer)
{
    LPPRECEIVE_EQUIPMENT Data = (LPPRECEIVE_EQUIPMENT)ReceiveBuffer;
    int Key = ((int)(Data->KeyH)<<8) + Data->KeyL;
    ChangeCharacterExt(FindCharacterIndex(Key),Data->Equipment);
}
```

`ChangeCharacterExt` was recovered in `ZzzCharacter.cpp`. The implementation evidence covers:

- weapon 0/1 type extraction from CharSet bytes 0/1 and high nibbles of bytes 11/12;
- weapon sentinel `0x0FFF` → type `-1`;
- wing type extraction and the later byte-15/byte-16 overrides;
- helper/horse/pegasus/fenrir selection from the documented bit fields;
- five body-part model selectors with `0x1FF` fallback to class-body models;
- the 24-bit packed level field and exact `LevelConvert` table `{0,3,5,7,9,11,13,15}`;
- five body-part `Option1` bits from byte 9;
- five body-part `ExtOption` bits from byte 10;
- `ExtendState = Equipment[10] & 0x01`.

The F3:13 wire frame remains the previously closed 24-byte C1 frame:

`C1 18 F3 13 IndexH IndexL CharSet[18]`

## Implementation

Added:

- `NEW_ERA_IMPLEMENTATION/mvp_login/f3_13_equipment_apply.h`
  - pure deterministic state transform;
  - model bases are explicit inputs, avoiding invented enum values;
  - preserves sentinel/fallback behavior;
  - does not call renderer, pet creation, scale, chaos-castle, or other undocumented engine side effects.

- `NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f3_13_equipment_apply/test_f3_13_equipment_apply_loopback.cpp`
  - real TCP `127.0.0.1` loopback;
  - exact 24-byte F3:13 frame;
  - server byte-for-byte echo;
  - extraction of all 18 CharSet bytes;
  - equipment/model application assertions;
  - regression for both weapon `0xFFF` sentinels;
  - regression for all five armor `0x1FF` body fallbacks.

- CMake target:
  `ts9_f3_13_equipment_apply_loopback`

## Golden/input vector

Frame:

`C1 18 F3 13 01 23 23 34 10 20 30 00 39 00 00 A8 F1 10 20 00 00 00 00 00`

Model-base fixture is intentionally explicit and synthetic; it validates the recovered arithmetic without claiming those fixture IDs are the upstream enum values.

## Runtime proof

Compiler:
`g++ -std=c++17 -Wall -Wextra -Wpedantic -pthread`

CMake configure/build was also executed against the same target logic.

Result:

`TS-9 F3:13 equipment/model apply loopback: PASS`

Exit status: `0`.

## Boundary

This closes the **recovered semantic equipment/model state transform** and executable TCP loopback.

It does **not** claim:

- original MU 3D renderer execution;
- actual `CHARACTER`/`OBJECT` memory layout integration;
- execution of `CreateBug`, `CreateBugSub`, pet systems, `DeleteBug`, `ChangeChaosCastleUnit`, `SetCharacterScale`, or `CharacterMachine->CalculateAll`;
- complete compile-time branch resolution for every optional upstream feature macro;
- undocumented transformations beyond the recovered `ChangeCharacterExt` evidence.

## Commits

- Codec: `109c215b99171d0611c554031cbd449cb3f95bc9`
- TCP loopback: `96b39040a917a2cae67bee467b15582342a2bcce`
- CMake: `65d557b04ce334258dbc19dda48f43a2cd07b46f`
