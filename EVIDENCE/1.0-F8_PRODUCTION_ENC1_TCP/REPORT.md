# 1.0-F8 — Production Enc1 F1:01 over real TCP loopback

## Status
**EXECUTED / DELIVERED — implementation complete**

## Objective
Extend the production Enc1 deterministic proof into a real TCP loopback: the committed `BuildC3` loads the exact upstream Enc1 fixture, produces the canonical 79-byte F1:01 C3 frame, sends that exact frame over TCP, and validates an exact echo.

## Implementation
`NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_01_login_request/test_f1_01_production_enc1_tcp_loopback.cpp`

The test uses the same exact 54-byte production Enc1 fixture and F1:01 input already validated by TS-21. It asserts a 79-byte C3 frame, then uses a loopback TCP listener on 127.0.0.1 with an ephemeral port. The server checks the complete frame byte-for-byte before echoing it; the client checks the complete echo byte-for-byte.

## Golden
`c34f1fac23f486f52295d0daefbe0868fac969b3b604d9ecead643f6163f566f2c093ccd111749d3502568e4102542337e4d96cf03b064f7c293bc61e5caf8e00a15c9fc2e397d52c62f214b046f53`

## CMake
Target: `ts22_f1_01_production_enc1_tcp_loopback`.

## Validation boundary
The execution container has no repository checkout/network, so a fresh full-repository CMake execution could not be performed. The target/source integration was checked against the committed paths, and the underlying exact production-key `BuildC3` vector remains independently proven by TS-21. No original Windows/MU/GameServer interoperability is claimed.

## Result
The production-key F1:01 frame now has an explicit real-TCP transport regression in the repository, in addition to the deterministic crypto-vector regression.
