# NEW-ERA 1.3-ZP2 — F3:13 Equipment/CharSet implementation + TCP loopback

## Status
**EXECUTED / DELIVERED — implementation + standalone TCP loopback PASS**

## Normative source
The upstream research repository is `wongddd/muonline` at canonical commit `580472e`.
The existing 1.3-Z evidence proves the exact server structure and emitter:

```cpp
struct PMSG_ITEM_EQUIPMENT_SEND
{
    PSBMSG_HEAD header; // C1:F3:13
    BYTE index[2];
    BYTE CharSet[18];
};
```

and:

```cpp
pMsg.header.set(0xF3,0x13,sizeof(pMsg));
pMsg.index[0] = SET_NUMBERHB(aIndex);
pMsg.index[1] = SET_NUMBERLB(aIndex);
memcpy(pMsg.CharSet,gObj[aIndex].CharSet,sizeof(pMsg.CharSet));
MsgSendV2(&gObj[aIndex],(BYTE*)&pMsg,pMsg.header.size);
```

Therefore the wire frame is exactly 24 bytes:

`C1 18 F3 13 IndexH IndexL CharSet[18]`

The evidence also proves the client dispatches `F3:13` to `ReceiveEquipment`.

## Implementation
Added:

- `NEW_ERA_IMPLEMENTATION/mvp_login/f3_13_equipment_charset.h`
  - normative 24-byte C1 builder;
  - big-endian character index;
  - exact 18-byte CharSet preservation;
  - strict parser with framing/subcode/size validation.

- `NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f3_13/test_f3_13_loopback.cpp`
  - real TCP `127.0.0.1` socket loopback;
  - golden frame build;
  - client → server byte-for-byte comparison;
  - server → client echo;
  - parser validation;
  - negative tests for invalid size and subcode.

## Golden vector used by the loopback

Index = `0x0123`

CharSet = `A0 A1 A2 A3 A4 A5 A6 A7 A8 A9 AA AB AC AD AE AF B0 B1`

Frame =

`C1 18 F3 13 01 23 A0 A1 A2 A3 A4 A5 A6 A7 A8 A9 AA AB AC AD AE AF B0 B1`

## Execution evidence
The exact implementation logic was compiled as C++17 with warnings enabled and the TCP loopback executed locally.

Result:

`F3:13 TCP loopback: PASS`

The loopback additionally exercised the parser and two negative framing cases.

## Scope boundary
This closes the **protocol codec and network loopback** for F3:13.
It does **not** claim that a complete 3D MU client rendering path has been reconstructed. The upstream client-side semantic application remains represented by the recovered `ReceiveEquipment` entry point; no undocumented CharSet transformation was invented here.

## Commits
- Codec: `f3e09831e6acb6b672c24ca0db6d2bd9dccb87a2`
- Loopback: `009841b3e008d7f80094b320f41a312e12af3f5b`
