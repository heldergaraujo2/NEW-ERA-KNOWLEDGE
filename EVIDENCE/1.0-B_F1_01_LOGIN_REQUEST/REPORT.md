# NEW-ERA 1.0-B — F1:01 Login Request — implementation + loopback

## Status
**EXECUTED / DELIVERED / PASS**

## Scope
Implemented fixed 10-byte ID/password, BuxConvert `{FC,CF,AB}`, TickCount LE, Version[5] transform, 16-byte protocol serial, packet serial, 32-byte chained XOR, SimpleModulus C3 framing, and TCP loopback.

Normative reference: `NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md` §3 and §4.2. The recovered logical fields are 49 bytes before packet-serial insertion; implementation builds 50 bytes before SimpleModulus.

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
`c34f5f3d11d746df2080285164025c2e47889de32a5493a667c1011a474e31f0f0cbfefee019a1cabfb28c94af9a135a29ed4a3ac096e8e8dd927310680985132bb4d3e69b081cca8bb52043d02619`

Frame length: 79 bytes (`C3`, length `0x4F`, 77-byte ciphertext).

## Runtime
Recreated C++17 fixture compiled with `g++ -std=c++17 -Wall -Wextra -Wpedantic -pthread` and executed successfully.

Output: `TS-13 F1:01 login request C3 loopback: PASS`

Checks included logical build, SimpleModulus round trip, inverse stream-XOR recovery, exact fields, and real TCP loopback on `127.0.0.1`.

## Boundary
The available GitHub file interface did not expose the manifest-referenced binary `Enc1.dat`/`Dec2.dat` as readable content in this run. Production-key interoperability is therefore not claimed. The implementation requires real Enc1 keys for production `BuildC3`; the executable proof uses injected deterministic test keys. Original Windows/MU/ASIO/GameServer interoperability is not claimed.
