# NEW-ERA — 1.3-Y P2 (RX F3:03 CharacterInfo) — Golden + Loopback Report
Status: CONFIRMED (golden incl. caso negativo + socket loopback)
Scope: NEW-ERA implementation (não-legacy)

## O que foi provado
1) Parser RX no core:
- `ParseFrame_CharacterInfo_F3_03_C1`
- Framing: `C1 0x42 F3 03` (fixo 66B)
- Endianness: WORD/DWORD LE; Exp/NextExp 8B BE-bytes

2) Golden check contra `rx_character_info_f3_03_vectors.json`:
- aceita vector positivo (32 asserts)
- rejeita size mismatch (`expect_ok:false`)

3) Loopback real (127.0.0.1 porta efêmera) com sequência:
- válido -> inválido -> válido no mesmo socket (rejeita sem quebrar)

## Outputs (registrados)
LOOPBACK_RX_CHARACTER_INFO_F3_03_OK
GOLDEN_RX_CHARACTER_INFO_F3_03_OK

## Artefatos (sha256)
- Spec: `NEW_ERA_PROTOCOL_MVP_RX_CHARACTER_INFO_F3_03_SPEC.md` sha256 `eb2405acb3412e358e00da46dcde7e71701ea9f5ba97e0b0d8e0897e4cc37e76`
- Vectors: `rx_character_info_f3_03_vectors.json` sha256 `e03cbd9e9aa7206ce19a47c3f60dabce9c0155ead2403b82e4e52284051f4018`
- Core (patched): `NEW_ERA_IMPLEMENTATION/mvp_login/mvp_login_client.cpp` sha256 `220481d2b8f9bb5ce1d52cf477e780630bd862b30e25dd36a642c4b55c1c2415`

## Frames exercitados (hex)
- ok (size 0x42): c142f303...
- invalid (size 0x41): c141f303...
