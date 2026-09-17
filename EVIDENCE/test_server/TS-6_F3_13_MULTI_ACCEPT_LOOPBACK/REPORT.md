# INFRA-TS-6 — F3:13 multi-accept/reconnect loopback

## Objective
Validate that the F3:13 `BOTH_MESSAGE` path survives the test-server multi-accept boundary: one listener accepts two clients sequentially, serves the same F3:13 response to each, closes each client, and accepts the next without restarting the listener.

## Implementation
- `NEW_ERA_IMPLEMENTATION/test_server/ts6_f3_13_multi_accept_loopback.cpp`
- normative codec: `NEW_ERA_IMPLEMENTATION/mvp_login/f3_13_equipment_charset.h`
- CMake target: `ts6_f3_13_multi_accept_loopback`

## Assertions
Each of the two independent TCP sessions validates:
1. trigger envelope id `0x000C`;
2. trigger body size `4`;
3. response envelope id `0x000C`;
4. response body size `24`;
5. inner frame is exactly the canonical F3:13 C1 frame;
6. parser recovers `index=0x0123`;
7. all 18 `CharSet` bytes match `A0..B1`.

The server loop explicitly performs `accept -> serve -> close -> accept` twice, proving reconnect behavior in the same process.

## Runtime validation
**PASS — EXECUTED**

Validated locally with C++17/g++:

`g++ -std=c++17 -Wall -Wextra -Wpedantic -pthread`

Observed stdout:

`TS-6 F3:13 multi-accept loopback: PASS`

Process exit status: `0`.

## Boundary
This proves F3:13 survives the reconstructed multi-accept test-server boundary over real TCP loopback. It does not claim execution against the original MU executable, original server, or 3D client renderer.
