# NEW-ERA 1.3-T — F1:00 ServerVersion implementation + TCP loopback

Status: **EXECUTED / DELIVERED / PASS**

## Scope
Implement the recovered F1:00 server-version C1 packet as an isolated codec and validate it over a real local TCP connection.

Research evidence already persisted in `EVIDENCE/1.3-T/NEW_ERA_1_3_T_SERVER_VERSION_AND_701_MODE_EVIDENCE.md` establishes the F1:00 handshake as a 12-byte C1 message containing the result, HeroKey and five version bytes.

## Implementation

- `NEW_ERA_IMPLEMENTATION/mvp_login/f1_00_server_version.h`
  - builder for the exact 12-byte C1 frame;
  - parser with exact size/header checks;
  - HeroKey decoded as the documented two-byte field;
  - five version bytes preserved as opaque bytes.

- `NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_00_server_version/test_f1_00_server_version_loopback.cpp`
  - real TCP listener on `127.0.0.1` with ephemeral port;
  - byte-for-byte server validation and echo;
  - parser field assertions;
  - truncated 11-byte negative regression.

- CMake target: `ts12_f1_00_server_version_loopback`.

## Golden vector

Input fields:
- Result `0x01`
- HeroKey `0x1234`
- Version bytes `35 30 32 41 42` (`502AB` as bytes)

Expected frame:

`c10cf1000112343530324142`

## Runtime validation

Exact test source was compiled with:

`g++ -std=c++17 -Wall -Wextra -Wpedantic -pthread`

Runtime result:

`1.3-T F1:00 ServerVersion TCP loopback: PASS`

Exit status: `0`.

A CMake configure/build fixture containing the same implementation and test source was also executed; target build and executable completed with exit status `0` and the same PASS output.

## Boundary

This proves the recovered F1:00 layout, codec and local TCP transport behavior. It does not claim execution of the original Windows/MU client, original GameServer process, original ASIO runtime, or undocumented server-version semantics beyond the recovered wire fields.
