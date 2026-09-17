# 1.0-C / F1:02 — Logout Request C3/TCP Loopback

## Objective

Implement the next still-open F1 client request after F1:01: SendRequestLogOut(int Flag).

## Primary evidence

Pinned upstream: wongddd/muonline@580472e.

Source: Source Client/source/wsclientinline.h

Recovered emitter:
- initializes CStreamPacketEngine with 0xC1, 0xF1;
- appends subcode 0x02;
- appends one-byte Flag;
- calls spe.Send(TRUE).

Therefore the pre-encryption logical packet is exactly:

[C1][05][F1][02][Flag]

The existing NEW-ERA crypto reconstruction applies the proven 32-byte chained XOR to bytes [3..end) before SimpleModulus C3 encryption, matching the already validated F1:01 pipeline.

## Implementation

Core: NEW_ERA_IMPLEMENTATION/mvp_login/f1_02_logout_request.h

API:
- LogoutRequestInput { flag }
- BuildLogicalC1(...)
- BuildC3(...)

C3 output is 13 bytes for this 5-byte logical packet:
[C3][0D] + 11-byte SimpleModulus ciphertext

The builder refuses to operate without loaded Enc1 keys.

## Golden vector

Test input:
- Flag = 0x5A
- deterministic injected SimpleModulus keys: modulus=65521, encryption key=3, XOR=7 on all lanes
- inverse test key=43681 (3^-1 mod 65521)

Logical wire after chained XOR:
c105f17a9c

C3 frame:
c30dae5337214b49224d54132b

13 bytes total.

## Runtime validation

Regression: NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_02_logout_request/test_f1_02_logout_request_loopback.cpp

Validation performed:
1. C++17 compile with -Wall -Wextra -Wpedantic -pthread.
2. Logical F1:02 size/head assertions.
3. C3 size/header assertions.
4. SimpleModulus decrypt round-trip with deterministic inverse test keys.
5. Real TCP loopback on 127.0.0.1 with exact 13-byte frame comparison.
6. Independent CMake configure/build fixture.

Observed output:
TS-14 F1:02 logout request C3 TCP loopback: PASS

Process exit status: 0.

## CMake

Added target: ts14_f1_02_logout_request_loopback

File: NEW_ERA_IMPLEMENTATION/test_server/CMakeLists.txt

## Boundary

This closes the reconstructed F1:02 codec/TCP path using deterministic injected crypto keys.

It does not claim production-key interoperability because the binary Enc1.dat is not exposed as UTF-8 through the available GitHub file interface. It also does not claim execution against the original Windows/MU executable or original ASIO runtime.
