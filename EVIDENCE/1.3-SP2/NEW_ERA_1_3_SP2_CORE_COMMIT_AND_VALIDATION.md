# NEW-ERA — 1.3-S P2 Combat Action/Skill RX

Status: **EXECUTED / DELIVERED / PASS**

## Scope
Close the isolated RX implementation for classic C1 `0x18` action and `0x19` magic/skill frames recovered from the canonical research source.

## Implemented

Core:
`NEW_ERA_IMPLEMENTATION/mvp_login/combat_action_skill_rx.h`

Loopback:
`NEW_ERA_IMPLEMENTATION/mvp_login/loopback_combat_action_skill_rx/test_combat_action_skill_loopback.cpp`

The parser enforces the exact 9-byte C1 frame size and header and decodes:
- `0x18`: key, angle, action, target;
- `0x19`: magic, source, target.

## Golden vectors

`0x18`:
`c10918012305780456`

`0x19`:
`c10919004201230456`

The TCP loopback sends both frames over one local connection, parses them, validates all decoded fields, and rejects a truncated 8-byte action frame.

## Build/runtime validation

Compiler:
`g++ -std=c++17 -Wall -Wextra -Wpedantic -pthread`

Result:
`TS-8 combat action/skill RX loopback: PASS`

Exit status: `0`.

CMake target:
`ts8_combat_action_skill_rx_loopback`

A CMake configure/build/run validation was also completed successfully with exit status `0`.

## Evidence classification

This closes the parser/loopback implementation supported by the recovered wire layout. It does not claim a complete semantic animation/action table, undocumented `ReceiveMagic` side effects, original Windows/MU renderer execution, or original client runtime integration.
