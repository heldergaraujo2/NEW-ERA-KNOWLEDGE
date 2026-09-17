# INFRA-TS-5 — F3:13 scripted BOTH_MESSAGE sequence

## Objective
Close the test-server integration gap left after TS-4 by adding F3:13 to a scripted sequence that is triggered by a client packet and returns the same canonical F3:13 frame through the modern `BOTH_MESSAGE (0x000C)` envelope.

## Implementation
`NEW_ERA_IMPLEMENTATION/test_server/ts5_f3_13_scripted_sequence_loopback.cpp`

## Normative facts used
- Modern envelope: `id:u16 LE + size:u32 LE`.
- `BOTH_MESSAGE = 0x000C`.
- Inner F3:13 is C1, 24 bytes total.
- F3:13 body is `C1 18 F3 13 + index[2] + CharSet[18]`.
- Index is encoded big-endian by the F3:13 codec.
- The scripted harness validates the envelope and then parses the inner F3:13 body.

## Sequence
1. Client connects to loopback TCP.
2. Client sends one `BOTH_MESSAGE` trigger containing a minimal C1 frame.
3. Server sends three identical scripted F3:13 messages through `BOTH_MESSAGE`.
4. Client validates all three modern headers and 24-byte inner bodies.
5. Client parses all three bodies and verifies `index=0x0123` plus every `CharSet[18]` byte.

## Static acceptance criteria
- [x] Source exists in the repository.
- [x] Modern envelope uses little-endian `u16/u32` fields.
- [x] Inner frame remains exactly 24 bytes.
- [x] `BOTH_MESSAGE` id is exactly `0x000C`.
- [x] Three-message scripted sequence is deterministic.
- [x] Parser round-trip checks semantic fields.

## Runtime status
**NOT EXECUTED IN THIS ENVIRONMENT.** The repository write is complete, but no local C++ compiler/runtime or GitHub Actions workflow is available through the current execution environment to honestly record an exit-code result. Therefore this report does **not** claim a runtime PASS.

## Boundary
This closes the source-level scripted integration path. It does not claim successful execution against the original MU executable or a 3D client.
