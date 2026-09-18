# NEW-ERA 1.0-B — F1:01 Login Request — implementation + loopback

## Status
**EXECUTED / DELIVERED / PASS — corrected 1.0-F5 packet-serial placement**

## Scope
Implemented fixed 10-byte ID/password, BuxConvert `{FC,CF,AB}`, TickCount LE, Version[5] transform, 16-byte protocol serial, packet serial, 32-byte chained XOR, SimpleModulus C3 framing, and TCP loopback.

Normative reference: `NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md` §3 and §4.2. The recovered request is 50 bytes including the C1 header and packet-serial position. `SendPacket` inserts `g_byPacketSerialSend++` at C1 byte [1] and then encrypts exactly 49 bytes from [1..49]. The chained XOR starts at [3], so the serial byte is not XORed.

## Implementation
`NEW_ERA_IMPLEMENTATION/mvp_login/f1_01_login_request.h`

## Regression
`NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f1_01_login_request/test_f1_01_login_request_loopback_v2.cpp`

## CMake
`ts13_f1_01_login_request_loopback`

## Golden vector
Input: ID `PLAYER`; Password `SECRET`; TickCount `0x12345678`; Version `35 30 32 41 42`; ProtocolSerial `A0 A1 A2 A3 A4 A5 A6 A7 A8 A9 AA AB AC AD AE AF`; PacketSerial `2A`.

Injected deterministic test Enc1 keys: modulus `65521`, key `3`, xor `7` on all four lanes. Matching inverse key: `43681`.

C3 frame:
`c34f0de31e1a4537821084daef63cf193a06e9118ad4d9ecae551825c3a8e23bc4093c87c61b5c4c8403a55c10254f4f154a8c97036638f7c2a29908798c2300c71cc9fc0a0409050aa2421cd86f53`

Frame length: 79 bytes (`C3`, length `0x4F`, 77-byte ciphertext).

## Runtime
Recreated C++17 fixture compiled with `g++ -std=c++17 -Wall -Wextra -Wpedantic -pthread` and executed successfully.

Output: `TS-13 F1:01 login request C3 loopback: PASS`

Checks included logical build, SimpleModulus round trip, inverse stream-XOR recovery, exact fields, and real TCP loopback on `127.0.0.1`.

## Boundary
The available GitHub file interface did not expose the manifest-referenced binary `Enc1.dat`/`Dec2.dat` as readable content in this run. Production-key interoperability is therefore not claimed. The implementation requires real Enc1 keys for production `BuildC3`; the executable proof uses injected deterministic test keys. Original Windows/MU/ASIO/GameServer interoperability is not claimed.

## 1.0-F5 correction
The original implementation incorrectly treated packet serial as byte [49] and encrypted the full 50-byte C1 buffer. Upstream `SendPacket` evidence proves the serial is inserted at byte [1] and the C1 header byte [0] is excluded from SimpleModulus. The implementation and golden vector were corrected accordingly.
