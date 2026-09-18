# 1.0-F11 — F1 production login TCP integration

## Status
**EXECUTED / DELIVERED — integration source complete; runtime PASS not claimed**

## Objective
Compose the committed production-key F1:01 TX builder, server-side RX decoder, and server decision/result layer across one real TCP connection.

## Implementation
- NEW_ERA_IMPLEMENTATION/test_server/ts25_f1_production_login_tcp_integration.cpp
- CMake target: ts25_f1_production_login_tcp_integration

Flow implemented:
F1:01 BuildC3 with production Enc1 fixture -> TCP 127.0.0.1 -> DecodeF101 with derived Dec1 -> policy validation -> F1:01 success result C1.

Expected result frame: `c105f10101`.

## Validation boundary
The execution container currently has no repository checkout and no network access, so this turn did not execute the committed TS-25 source from the repository. No `TS-25 PASS` claim is made. Earlier TS-21 and TS-22 independently prove the production-key F1:01 construction, and TS-24 proves the decision/result mapping; TS-25 is the composition target joining those layers.

## Integration safety
The source was finalized with portable POSIX/WinSock socket handling and Windows `ws2_32` linkage in CMake.

## Boundary
Original Windows/MU runtime, original GameServer socket stack, and real account database/JoinServer implementation remain unverified.