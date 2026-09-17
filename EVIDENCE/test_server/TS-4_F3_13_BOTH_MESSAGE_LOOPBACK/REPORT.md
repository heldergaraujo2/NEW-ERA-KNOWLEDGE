# INFRA-TS-4 — F3:13 over BOTH_MESSAGE

## Status
**EXECUTED / DELIVERED**

## Objective
Close the transport boundary between the proven F3:13 C1 codec and the NEW-ERA modern envelope used by the test-server lab: `BOTH_MESSAGE (0x000C)` with `id:u16 LE + size:u32 LE + body`.

## Implementation
- `NEW_ERA_IMPLEMENTATION/test_server/ts4_f3_13_both_message_loopback.cpp`
- Reuses the normative codec:
  `NEW_ERA_IMPLEMENTATION/mvp_login/f3_13_equipment_charset.h`

## Normative frame
Inner classic frame:
- C1
- length `0x18` = 24 bytes
- head `0xF3`
- subcode `0x13`
- index `0x0123` big-endian
- CharSet = 18 bytes `A0..B1`

Modern envelope:
- id `0x000C` little-endian
- size `24` little-endian
- body = exact 24-byte F3:13 frame
- total envelope = 30 bytes

## Assertions
The harness validates:
1. modern id is exactly `0x000C`;
2. modern size is exactly `24`;
3. envelope body is byte-for-byte identical to the F3:13 C1 frame;
4. server receives the complete modern header and body over TCP;
5. server echoes the same modern envelope;
6. client receives the same body byte-for-byte;
7. F3:13 parser reconstructs index and all 18 CharSet bytes;
8. regression checks distinguish modern id/size from the inner C1 framing.

## Runtime validation
Executed locally with:
- C++17
- `g++`
- `-Wall -Wextra -Wpedantic -pthread`
- TCP loopback on `127.0.0.1`

Observed stdout:

```text
TS-4 F3:13 BOTH_MESSAGE loopback: PASS
```

Exit status: `0`.

## Boundary
This proves the F3:13 codec through the NEW-ERA `BOTH_MESSAGE` envelope and TCP transport. It does not claim integration with the original MU executable or client renderer.
