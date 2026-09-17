# NEW-ERA — 1.3-R P3 BOTH_ATTACK1/2 TCP Loopback

Status: **EXECUTED / DELIVERED / PASS**

## Scope

Validate the recovered `BOTH_ATTACK1 (0x0008)` and `BOTH_ATTACK2 (0x0009)` builders over a real local TCP connection, preserving the normative 6-byte little-endian modern envelope and exact body bytes.

## Implemented

`NEW_ERA_IMPLEMENTATION/mvp_login/loopback_both_attack_tx/test_both_attack_tx_tcp.cpp`

The test creates a real TCP listener on `127.0.0.1` with an ephemeral port. A client builds both frames using the isolated `both_attack_tx.h` core, transmits them, and receives exact byte-for-byte echoes from the server side.

## Expected wire frames

BOTH_ATTACK1:

`080007000000c1071107017803`

BOTH_ATTACK2:

`090009000000c309db01090932013c`

The server independently validates:
- envelope id;
- little-endian body size;
- exact body bytes;
- frame ordering across the same TCP connection.

The client independently validates exact echoed bytes.

## Runtime validation

Compiler:

`g++ -std=c++17 -Wall -Wextra -Wpedantic -pthread`

Result:

`1.3-RP3 BOTH_ATTACK1/2 TCP TX loopback: PASS`

Exit status: `0`.

A separate CMake configure/build of the same test source was also executed successfully, followed by the executable run with the same PASS result and exit status `0`.

## CMake

Added target:

`ts11_both_attack_tx_tcp_loopback`

## Boundary

This proves the recovered TX core can emit both modern frames and carry them over a real local TCP stream with exact bytes. It does **not** claim integration with the original MU Windows client, original ASIO runtime, original GameServer process, or undocumented gameplay semantics.
