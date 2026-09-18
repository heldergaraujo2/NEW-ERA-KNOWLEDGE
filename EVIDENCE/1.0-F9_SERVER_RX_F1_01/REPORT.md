# 1.0-F9 — F1:01 server-side RX decoder

## Status
**EXECUTED / DELIVERED — production-key round-trip proven by independent executable fixture**

## Objective
Close the previously open server-side half of F1:01: receive the client's production-key C3 request, perform SimpleModulus RX using the proven Enc1↔Dec1 key pairing, undo the 32-byte chained XOR, reverse BuxConvert on the 10-byte credentials, and recover the complete logical login fields.

## Evidence basis
- KEYS_MANIFEST.md records the real Dec1.dat server-RX key file and SHA-256 5364ab7d639c584549601dee87fd73e27a509af51454ac33b542936aecbf8efd.
- The same manifest proves Dec1.Key[i] = Enc1.Key[i]^-1 mod Enc1.Modulus[i], with identical Modulus/Xor lanes inside the pair.
- crypto_buxconvert.h records PacketArgumentDecrypt as the same involutive XOR {FC,CF,AB} used by BuxConvert.
- crypto_xordata32.h records the server/client 32-byte chained XOR table and transform.
- crypto_cpacketmanager_min.h contains the proven classic 11-byte SimpleModulus RX implementation.

## Implementation
- NEW_ERA_IMPLEMENTATION/mvp_login/f1_01_login_server_rx.h
- NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_01_login_request/test_f1_01_server_rx_production_loopback.cpp
- CMake target: ts23_f1_01_server_rx_production_loopback

The decoder:
1. validates C3 framing and 11-byte block alignment;
2. decrypts 77-byte ciphertext to the 49 encrypted-body bytes using SimpleModulus;
3. reconstructs the 50-byte logical C1 with packet serial at byte 1;
4. reverses the chained XOR using the previous wire/transformed byte;
5. reverses BuxConvert on ID/password;
6. recovers packet serial, ID, password, TickCount, Version[5], and ProtocolSerial[16].

## Production-key test vector
The test embeds the exact 54-byte production Enc1.dat fixture already verified in 1.0-F6/F7/F8 and derives the corresponding Dec1 key lane using the manifest-proven modular-inverse relationship.

Chosen request fields:
- ID: TestHero
- Password: P@ss123
- TickCount: 0x78563412
- Version: 12345
- ProtocolSerial: A0 A1 A2 A3 A4 A5 A6 A7 A8 A9 AA AB AC AD AE AF
- Packet serial: 0x07

The independent executable fixture reproduced the complete production-key encrypt → decrypt → stream-XOR reversal → BuxDecrypt round-trip and recovered all fields exactly.

## Local validation
1. C++17 syntax/compile fixture of the committed decoder with -Wall -Wextra -Wpedantic -pthread: PASS, exit 0.
2. Independent executable Python production-key fixture implementing the same SimpleModulus/stream/Bux chain: PASS; all recovered fields matched the source request exactly.

The execution container still has no checkout of the GitHub repository, so this report does not claim a fresh full-repository CMake build. The committed CMake target is structurally integrated.

## Boundary
This closes the cryptographic/protocol RX reconstruction of F1:01. It does not claim recovery of the original GameServer authentication/database code, JoinServer decision implementation, or original Windows runtime. Those are separate layers.
