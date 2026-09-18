# 1.0-F6 — Production Enc1.dat loader/interoperability vector

## Status
**EXECUTED / DELIVERED / PASS**

The pinned upstream repository exposes the real Client/Data/Enc1.dat as a 54-byte Git blob. Its SHA256 is:
ee73a59bf278d42735e537c96c25cc1ad0f69f1de089a7b20a1e6d2910a4372d.

The 54 bytes were decoded from the GitHub base64 representation and used unchanged as a runtime fixture. The existing LoadEncDecKeysFromFile implementation successfully parsed the production file format and recovered:
- Modulus[0] = 0x1F44F
- Key[0] = 0x5BC1
- Xor[0] = 0xBD1D

The test then called the committed BuildC3 with those production Enc1 keys and the canonical F1:01 input.

## Correct production-key golden
c34f1fac23f486f52295d0daefbe0868fac969b3b604d9ecead643f6163f566f2c093ccd111749d3502568e4102542337e4d96cf03b064f7c293bc61e5caf8e00a15c9fc2e397d52c62f214b046f53

79 bytes.

## Validation
C++17, -Wall -Wextra -Wpedantic -pthread, standalone fixture using the exact committed loader/core and the exact 54-byte upstream Enc1 blob. The execution container did not contain a repository checkout, so the fixture reproduced the committed source in isolation.

Result:
TS-21 F1:01 production Enc1 loader + C3 golden: PASS

This is a production-key crypto-vector validation, not an original-client/GameServer interoperability test.
