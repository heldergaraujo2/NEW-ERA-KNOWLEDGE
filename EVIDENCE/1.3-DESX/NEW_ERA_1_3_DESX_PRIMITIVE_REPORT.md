# NEW-ERA 1.3-DESX — >=701 DES-XEX3 primitive

## Status
**IMPLEMENTED / CI PASS — primitive layer**

## Objective
Close the previously open >=701 cryptographic primitive identified in the pinned GameServer evidence: Crypto++ `DES_XEX3` / DESX.

## Recovered contract
The pinned NEW-ERA evidence records that `GAMESERVER_UPDATE>=701` adds Crypto++ `DES_XEX3` to the GameServer encryption path. The Crypto++ API defines DES_XEX3 as an 8-byte block cipher with a 24-byte key. Its implementation applies 8-byte pre-whitening, DES with the middle 8-byte key, then 8-byte post-whitening.

Independent Crypto++ source confirms the exact construction:
- 24-byte fixed key;
- 8-byte block;
- encryption/decryption direction swaps the whitening halves;
- core operation is whitening -> DES -> whitening.

This implementation uses OpenSSL EVP 3DES with K1=K2=K3 to obtain the single-DES core, then applies the Crypto++-compatible XEX3 whitening layout. This avoids claiming that OpenSSL's 3DES primitive itself is DES-XEX3.

## Implementation
- `NEW_ERA_IMPLEMENTATION/mvp_login/crypto_des_xex3.h`
  - 24-byte key / 8-byte block contract
  - EncryptBlock / DecryptBlock
  - Crypto++ DES_XEX3-compatible whitening
  - OpenSSL EVP backend
- `NEW_ERA_IMPLEMENTATION/test_server/ts28_des_xex3_loopback.cpp`
  - deterministic 24-byte key
  - known 8-byte plaintext/ciphertext vector
  - encryption and decryption round-trip
- `NEW_ERA_IMPLEMENTATION/test_server/CMakeLists.txt`
  - TS-28 target
  - OpenSSL::Crypto linkage
- `.github/workflows/new-era-ts28.yml`
  - isolated CI validation

## Verification
Dedicated GitHub Actions run **35394342879** completed **SUCCESS**.

Regression output:
`TS-28 DES-XEX3 block compatibility: PASS`

Independent reference material:
Crypto++ documents DES_XEX3 as a DESX block cipher with a fixed 8-byte block and 24-byte key. citeturn1search1turn1search0
The upstream Crypto++ implementation shows the exact pre-whitening -> DES -> post-whitening operation used by this compatibility layer. citeturn1search5turn1search7

## Boundary
This closes the **DES-XEX3 primitive**.

It does not yet claim the complete >=701 GameServer transport composition because the pinned NEW-ERA evidence explicitly left the following unrecovered:
- exact production DES-XEX3 key material/source;
- exact dispatch/composition point in the live GameServer socket path;
- exact `ENCRYPT_STATE` value in the original build;
- live original GameServer interoperability.

Those are integration/evidence objectives, not reasons to leave the primitive itself unimplemented.
