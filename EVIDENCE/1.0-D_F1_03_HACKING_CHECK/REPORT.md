# 1.0-D — F1:03 Hacking Check Request C3/TCP Loopback

## Objective

Implement the next still-open F1 client request after logout: SendHackingChecked(byType, byParam).

## Evidence

Pinned upstream: wongddd/muonline@580472e.
Source: Source Client/source/wsclientinline.h.

Recovered emitter:
- CStreamPacketEngine initialized with 0xC1, 0xF1;
- appends 0x03;
- appends byType and byParam;
- calls spe.Send(TRUE).

Logical packet: [C1][06][F1][03][Type][Param].

The established login transport pipeline applies the proven chained XOR to bytes [3..end), then SimpleModulus and C3 framing.

## Implementation

Core: NEW_ERA_IMPLEMENTATION/mvp_login/f1_03_hacking_check_request.h

Regression: NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_03_hacking_check/test_f1_03_hacking_check_loopback.cpp

CMake target: ts15_f1_03_hacking_check_loopback

## Golden vector

Input Type=0x12, Param=0x34.
Logical after XOR: c106f17bd553
C3 frame length: 13 bytes, header C3 0D.

Deterministic test keys: modulus=65521, encryption key=3, XOR=7 on all lanes; inverse key=43681 where applicable.

## Validation

Validated in a recreated C++17 fixture because the container did not have a repository checkout/network access:
- g++ -std=c++17 -Wall -Wextra -Wpedantic -pthread;
- logical golden-vector assertions;
- C3 framing assertions;
- real TCP loopback on 127.0.0.1 with exact 13-byte comparison.

Observed:
TS-15 F1:03 hacking-check C3 TCP loopback: PASS

Process exit status: 0.

## Boundary

This proves the reconstructed codec and local TCP transport with deterministic injected crypto keys. It does not claim production Enc1.dat interoperability or execution against the original Windows/MU/ASIO runtime.
