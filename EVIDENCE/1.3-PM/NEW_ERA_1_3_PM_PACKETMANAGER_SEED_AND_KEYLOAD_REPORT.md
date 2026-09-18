# NEW-ERA 1.3-PM — Server PacketManager seed + key loading

## Status
**IMPLEMENTED / ISOLATED REGRESSION PASS**

## Objective
Close the documented PacketManager seeding gap for the classic (<701) GameServer path:
- recover the production m_XorFilter[32] seed;
- reproduce LoadKey semantics for 54-byte ENCDEC files;
- expose server-side encryption/decryption key lanes using the already proven SimpleModulus primitive;
- prove the server packet filter transform against the upstream recurrence.

## Upstream basis
Pinned source: wongddd/muonline@580472e.

Recovered facts:
- CPacketManager::Init seeds m_XorFilter[32] with the production filter beginning E7 6D 3A 89 and ending 29 51 E8 56.
- LoadKey requires header 4370, file size 54, then reads three LE DWORD[4] tables and XORs them with m_SaveLoadXor = {3F08A79B,E25CC287,93D27AB9,20DEA7BF}.
- ExtractPacket accepts C1/C2, validates declared size against buffered bytes, then applies XorData(size-1,end).
- XorData walks backward: m_buff[n] ^= m_buff[n-1] ^ m_XorFilter[n%32].

## Implementation
Added:
- NEW_ERA_IMPLEMENTATION/mvp_login/crypto_cpacketmanager_server.h
- NEW_ERA_IMPLEMENTATION/test_server/ts26_packetmanager_seed_loopback.cpp
- CMake target: ts26_packetmanager_seed_loopback

The server wrapper reuses the already proven PacketCryptoSM SimpleModulus implementation rather than duplicating the cipher.

## Production-key validation
The regression embeds the exact committed 54-byte Enc1 fixture already used by TS-21.

Recovered values checked:
- Modulus[0] = 0x1F44F
- Key[0] = 0x5BC1
- Xor[0] = 0xBD1D

The test checks the filter seed and exercises the packet-filter recurrence by constructing a deterministic C1 wire packet with the inverse recurrence and recovering the original bytes through ExtractPacket.

## Execution
An isolated C++17 fixture equivalent to the committed TS-26 source was compiled with -Wall -Wextra -Wpedantic.

Observed result:

PASS

## Scope boundary
This closes the server PacketManager seed/key-loading layer for the recovered classic path.

It does not claim:
- DES/XEX3 behavior for GAMESERVER_UPDATE>=701;
- original live GameServer socket integration;
- production Dec2 binary execution in this environment, because the binary itself is not exposed through the UTF-8 GitHub file interface.

The existing KEYS_MANIFEST records the canonical Dec2 SHA256: d19e950c44e5648113269cb35d74c25f1c7ca27399e62da95fa127d4d714325e.