# NEW-ERA — 1.3-S P2 (RX 0x18/0x19) — Golden + Loopback Report
Status: CONFIRMED (golden field-by-field + socket loopback)
Scope: NEW-ERA implementation (não-legacy)

## O que foi provado
1) Parsers RX no core (C1 fixed size 0x09):
- `0x18` Action: `C1 09 18 | KeyH KeyL Angle Action TargetKeyH TargetKeyL`
- `0x19` Magic:  `C1 09 19 | MagicH MagicL SourceKeyH SourceKeyL TargetWord(b15 success)`

2) Golden check (campo-a-campo) contra `rx_action_magic_vectors.json` (fonte normativa)

3) Loopback real (127.0.0.1 porta efêmera):
- stub GS envia 3 frames golden (9B) em sequência
- client recebe 9B, parseia e asserta contra o JSON

## Outputs (registrados)
### Loopback
LOOPBACK_RX_ACTION_MAGIC_OK

## Artefatos (sha256)
- Spec: `NEW_ERA_PROTOCOL_MVP_RX_ACTION_MAGIC_SPEC.md` sha256 `c3954fb7599cda494c1d2b4610e206537ba0d573fe061d4956ba699366afdec9`
- Vectors: `rx_action_magic_vectors.json` sha256 `cc48b32117855bed9ce0b85e549c5f371bdb117bcc19f07014283d078fd91031`
- Core (patched): `NEW_ERA_IMPLEMENTATION/mvp_login/mvp_login_client.cpp` sha256 `edf9d4528e88a3c9a3f56e65b2a2dc6c815a05d5e364c783d36cb66fb6a20e25`

## Frames exercitados (hex)
- action: c10918010104780102
- magic success: c10919000901018102
- magic fail: c10919000901010102
