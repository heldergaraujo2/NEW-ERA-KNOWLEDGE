# 1.0-F7 — CMake integration of TS-21 production Enc1 vector

## Status
**EXECUTED / DELIVERED**

## Objective
Promote the already verified TS-21 production Enc1/F1:01 deterministic regression into the shared `NEW_ERA_IMPLEMENTATION/test_server/CMakeLists.txt` build graph.

## Change
Added target:
- `ts21_f1_01_production_enc1_vector`
- source: `../mvp_login/loopback_f1_01_login_request/test_f1_01_production_enc1_vector.cpp`
- include path: `../mvp_login`
- C++17
- Windows link: `ws2_32`

## Existing proof carried forward
TS-21 already passed against the exact pinned upstream 54-byte Enc1.dat fixture and exact production-key 79-byte F1:01 golden:
`c34f1fac23f486f52295d0daefbe0868fac969b3b604d9ecead643f6163f566f2c093ccd111749d3502568e4102542337e4d96cf03b064f7c293bc61e5caf8e00a15c9fc2e397d52c62f214b046f53`

Prior runtime result:
`TS-21 F1:01 production Enc1 loader + C3 golden: PASS`

## Validation boundary
The execution container does not contain a repository checkout and cannot reach GitHub. Therefore this objective's CMake edit was verified structurally against the committed target/source paths, while the executable TS-21 proof remains the previously recorded standalone C++17 validation. No claim of a fresh full-repository CMake build is made.

## Result
The production Enc1 regression is now part of the repository's CMake target set and can be built as `ts21_f1_01_production_enc1_vector`.

## Boundary
This does not prove original Windows/MU client, ASIO, or GameServer interoperability.
