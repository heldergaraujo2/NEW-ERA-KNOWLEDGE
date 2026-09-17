# INFRA-TS-7 — Test-server CMake integration

## Status
**EXECUTED / DELIVERED / PASS**

## Objective
Integrate the completed TS-4, TS-5 and TS-6 loopback harnesses into the test-server CMake project so the harnesses are explicit C++17 build targets instead of standalone source files.

## Implementation
Updated:
`NEW_ERA_IMPLEMENTATION/test_server/CMakeLists.txt`

Targets now declared:
- `new_era_test_server`
- `ts4_f3_13_both_message_loopback`
- `ts5_f3_13_scripted_sequence_loopback`
- `ts6_f3_13_multi_accept_loopback`

All targets require C++17. On Windows, each target receives `ws2_32`.

## Runtime/build evidence
The TS-6 source was compiled and executed locally with C++17/g++ and the same F3:13 codec, producing:

```text
TS-6 F3:13 multi-accept loopback: PASS
CMAKE_CONFIGURE=PASS
CMAKE_BUILD=PASS
```

The CMake configure/build validation used an isolated fixture containing the exact TS-6 source and F3:13 header because the available local runtime does not contain the complete GitHub checkout. Therefore this evidence does not claim a full checkout build of `new_era_test_server`, TS-4, or TS-5 in this environment.

## Boundary
This closes the build-system integration for the four declared test-server targets. It does not claim execution against the original MU executable, original server, or client renderer.
