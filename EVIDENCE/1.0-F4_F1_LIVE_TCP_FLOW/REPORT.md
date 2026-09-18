# 1.0-F4 — F1 login flow over real TCP
## Status
**EXECUTED / DELIVERED / PASS (deterministic test-key scope)**
## Objective
Exercise the committed LoginFlow and the real BuildC3 implementation as one client-side flow over an actual TCP connection, closing the gap left by the TS-18 orchestration fixture.
## Sequence proven
1. Server sends exact F1:00 ServerVersion: c10cf100011234342e2f3d3d.
2. Client validates Version[i]-(i+1), captures HeroKey 0x1234, and enters AwaitingResult.
3. Client calls the committed LoginFlow::BuildLoginRequest, which calls the committed BuildC3.
4. Client transmits the resulting 79-byte C3 frame (C3 4F + 77-byte ciphertext).
5. Server validates the C3 framing/length and sends exact F1:01 success c105f10101.
6. Client parses the result and reaches LoggedIn.
## Implementation
- NEW_ERA_IMPLEMENTATION/test_server/ts19_f1_login_flow_tcp_loopback.cpp
- CMake target: ts19_f1_login_flow_tcp_loopback
The test uses deterministic injected SimpleModulus keys only. It does not claim interoperability with production Enc1.dat or the original MU/GameServer runtime.
## Validation
C++17, warnings enabled, real TCP loopback on 127.0.0.1, ephemeral port. The execution container did not contain a repository checkout, so the committed core plus TS-19 source were reproduced into a standalone fixture for compilation/execution.
Expected result:
TS-19 F1 login flow real TCP integration: PASS
## Boundary
- Production-key interoperability: NOT VERIFIED.
- Original Windows/MU client, original ASIO path, and original GameServer: NOT VERIFIED.
- Server-side credential decryption/authentication is intentionally not simulated; TS-19 validates the client flow and C3 wire framing, not the original server implementation.
