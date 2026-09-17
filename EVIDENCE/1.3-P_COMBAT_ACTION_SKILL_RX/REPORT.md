# 1.3-P — Combat action/skill RX

## Status
**EXECUTED / DELIVERED / PASS**

## Objective
Implement the first branch of the documented 1.3-P choice: server-to-client combat visual RX signals for:

- `0x18` action-character (`ReceiveAction`);
- `0x19` magic/skill (`ReceiveMagic`).

The implementation is intentionally byte-level and does not invent the upstream animation table or undocumented gameplay side effects.

## Upstream evidence
Pinned research source: `wongddd/muonline@580472e`.

`WSclient.cpp` dispatches:
- head `0x18` to `ReceiveAction(ReceiveBuffer, Size)`;
- head `0x19` to `ReceiveMagic(ReceiveBuffer, Size, bEncrypted)`.

`WSclient.h` defines:

### 0x18 `PRECEIVE_ACTION`
`PBMSG_HEADER + KeyH + KeyL + Angle + Action + TargetKeyH + TargetKeyL`.

This yields a native C1 frame of **9 bytes**:

`[C1][09][18][KeyH][KeyL][Angle][Action][TargetKeyH][TargetKeyL]`

### 0x19 `PRECEIVE_MAGIC`
`PBMSG_HEADER + MagicH + MagicL + SourceKeyH + SourceKeyL + TargetKeyH + TargetKeyL`.

This yields a native C1 frame of **9 bytes**:

`[C1][09][19][MagicH][MagicL][SourceKeyH][SourceKeyL][TargetKeyH][TargetKeyL]`

The upstream source also shows `ReceiveAction` applying `SetAction` using the received action value, with additional class/action-specific branches. Those branches are deliberately **not reconstructed here** because their complete semantic table is outside the recovered wire contract.

## Implementation

`NEW_ERA_IMPLEMENTATION/mvp_login/combat_action_skill_rx.h`

Provides strict parsers:
- `ParseActionC1`
- `ParseSkillC1`

Both enforce exact 9-byte length and exact C1/head byte before exposing decoded fields.

## Golden vectors

### Action 0x18
`c10918012305780456`

Decoded:
- key `0x0123`
- angle `0x05`
- action `0x78`
- target `0x0456`

### Skill 0x19
`c10919004201230456`

Decoded:
- magic `0x0042`
- source `0x0123`
- target `0x0456`

## Loopback

`NEW_ERA_IMPLEMENTATION/mvp_login/loopback_combat_action_skill_rx/test_combat_action_skill_loopback.cpp`

Real TCP loopback on `127.0.0.1`:
1. client connects;
2. client sends a trigger byte;
3. server sends the exact 0x18 golden frame;
4. server sends the exact 0x19 golden frame;
5. client decodes both;
6. client verifies all fields;
7. truncated 0x18 frame is rejected without acceptance.

Observed runtime:

`TS-8 combat action/skill RX loopback: PASS`

Exit status: `0`.

Compilation:

`g++ -std=c++17 -Wall -Wextra -Wpedantic -pthread`

## CMake

Added target:

`ts8_combat_action_skill_rx_loopback`

with C++17 and Windows `ws2_32` linkage.

## Boundary / non-claims

This closes the **wire-level RX codec + executable loopback** for the two recovered combat signals. It does **not** claim:

- the complete upstream animation/action semantic table;
- original client renderer execution;
- original MU server execution;
- C4/transport decryption for these messages;
- undocumented `ReceiveMagic` side effects;
- that every 0x19 emission uses C1 in every build/path.

Those remain separate evidence items rather than inferred behavior.
