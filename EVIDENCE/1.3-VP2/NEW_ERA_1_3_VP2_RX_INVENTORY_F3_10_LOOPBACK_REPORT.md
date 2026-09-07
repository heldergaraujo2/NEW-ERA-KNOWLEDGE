# NEW-ERA — 1.3-V P2 (RX Inventário F3:10 C2 variável) — Golden + Loopback Report
Status: CONFIRMED (golden incl. caso negativo + socket loopback)
Scope: NEW-ERA implementation (não-legacy)

## O que foi provado
1) Parser RX no core:
- `ParseFrame_InventoryF3_10_C2`
- Framing: `[C2][size u16 BE][F3][10][count] + count*([slot][ItemInfo 12B])`
- Guards: `size==len`, `size==6+13*count`, head/sub, teto local.

2) Golden check contra `rx_inventory_f3_10_vectors.json`:
- aceita count=1 (memcmp ItemInfo 12B)
- aceita count=0
- rejeita size mismatch (`expect_ok:false`)

3) Loopback real (127.0.0.1 porta efêmera) com sequência:
- válido -> inválido -> válido no mesmo socket (rejeita sem quebrar)

## Outputs (registrados)
LOOPBACK_RX_INVENTORY_F3_10_OK
GOLDEN_RX_INVENTORY_F3_10_OK

## Artefatos (sha256)
- Spec: `NEW_ERA_PROTOCOL_MVP_RX_INVENTORY_F3_10_SPEC.md` sha256 `7c2a9a33277cb50b9fcc2eaba0c3ef68879c94fdaa2ebc2fc40d02fd599c69a2`
- Vectors: `rx_inventory_f3_10_vectors.json` sha256 `71b096abf640429b9f723f02d0464474377380e6e4d2b490e2a1e16e4eb7031b`
- Core (patched): `NEW_ERA_IMPLEMENTATION/mvp_login/mvp_login_client.cpp` sha256 `4881e942d33a73b7868122ef20d1e2631ea9d7ec397b118c0a0552a76eac0e03`

## Frames exercitados (hex)
- ok/count1: c20013f31001000102030405060708090a0b0c
- invalid(size mismatch): c20012f31001000102030405060708090a0b0c
- ok/count0: c20006f31000

## Nota
- Há warning cosmético esperado: guard `count > 255` é vacuo pois `count` é u8 (registrar em governança).
