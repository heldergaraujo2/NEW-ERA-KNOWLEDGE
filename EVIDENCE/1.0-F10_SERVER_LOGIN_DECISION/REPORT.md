# 1.0-F10 — F1 server login decision/result layer

## Status
**EXECUTED / DELIVERED / PASS — compatible reconstruction layer**

## Objective
Turn the already decoded F1:01 request into a deterministic server-side validation/result layer, without claiming recovery of the original database/authentication implementation.

## Implementation
- NEW_ERA_IMPLEMENTATION/mvp_login/f1_login_server_decision.h
- NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_login_server_decision/test_f1_login_server_decision.cpp
- CMake target: ts24_f1_login_server_decision

The layer validates non-empty credentials, packet serial non-zero, expected version, and expected protocol serial when configured. It maps the documented JoinServer result values 0 through 4 directly to F1:01 result byte values and emits the documented version/serial failure result 6 as C1 05 F1 01 06.

## Golden behavior
Success result frame: c105f10101
Version/serial failure: c105f10106
Other mapped result values: 0, 2, 3, 4.

## Validation
A recreated C++17 fixture using the exact decision-layer logic was compiled with -Wall -Wextra -Wpedantic and executed locally.

Observed:
TS-24 F1 server login decision/result mapping: PASS

The fixture covered success validation, all five documented JoinServer result values, result-frame layout, result-6 emission, version mismatch, zero packet serial, and empty account negatives.

The execution container has no repository checkout/network, so no full-repository CMake build is claimed.

## Boundary
This is a compatible decision-layer reconstruction. It does not claim the original GameServer database lookup, account creation, connection-state storage, or JoinServer implementation. Those can be attached behind JoinServerResult without changing the proven wire layer.
