# NEW-ERA 1.3-C4 — Server C4 RX / SimpleModulus + XOR32

## Status
**IMPLEMENTED / CI PASS**

## Objective closed
Implement the classic server-side C4 receive contract recovered from the pinned SocketManager path:
1. validate outer C4 framing with 16-bit big-endian size;
2. decrypt the payload with the already-proven classic SimpleModulus server primitive;
3. recover the inner classic C2 packet;
4. reverse the server PacketManager chained XOR32 over the inner payload;
5. reject malformed outer length and block alignment.

## Evidence basis
Pinned reference: wongddd/muonline@580472e.

NEW_ERA_PHASE_0C10A_C3C4_SIMPLEMODULUS_PIPELINE.md records that C4 uses a 2-byte BE size and that the GameServer CPacketManager performs SimpleModulus plus the 32-byte filter path.

NEW_ERA_PHASE_0C10B_GS_RECV_C3C4_AND_ENCRYPT_STATE.md records the concrete GS receive path:
- C4 size is read from bytes 1..2;
- SimpleModulus decrypt starts after the 3-byte C4 header;
- the decrypted bytes enter PacketManager extraction/XOR handling;
- ENCRYPT_STATE=0, so this transport-whitening gate is not part of the active path.

The public MU protocol description independently documents C4 as the encrypted 16-bit-size frame and SimpleModulus as its encryption layer. citeturn1search7turn1search2

## Implementation
- NEW_ERA_IMPLEMENTATION/mvp_login/crypto_c4_server_rx.h
  - DecodeC4ServerFrame
  - UndoServerStreamXor
  - strict outer/inner size checks
  - 11-byte SimpleModulus block alignment check
- NEW_ERA_IMPLEMENTATION/test_server/ts27_c4_server_rx_loopback.cpp
  - production Enc1-derived server keys
  - 300-byte inner C2 payload, forcing C4/large-frame handling
  - SimpleModulus encrypt/decrypt round-trip
  - XOR32 forward/inverse round-trip
  - negative outer-size regression
- NEW_ERA_IMPLEMENTATION/test_server/CMakeLists.txt
  - target ts27_c4_server_rx_loopback
- .github/workflows/new-era-ts27.yml
  - isolated CI build + execution for TS-27

## Verified result
GitHub Actions run 35390287725: SUCCESS.

The isolated CI target compiled and executed successfully:
TS-27 C4 SimpleModulus + server XOR32 decode: PASS

The test intentionally uses the production Enc1-decoded key values already verified by the project; it does not introduce a new key source.

## Boundary
This closes the classic C4 framing + SimpleModulus + server XOR32 receive layer.

It does not claim:
- GAMESERVER_UPDATE>=701 DES-XEX3 processing;
- live original GameServer socket interoperability;
- connection/session serial semantics beyond the recovered inner-packet decode contract.

Those remain separate objectives.
