# NEW-ERA — 1.3-R P2 (BOTH_ATTACK1/2 TX olc) — Golden + Loopback Report
Status: CONFIRMED (golden byte-a-byte + socket loopback)
Scope: NEW-ERA implementation (não-legacy)

## O que foi provado
1) Builders TX olc nativos:
- BOTH_ATTACK1 (0x0008) — frame 13B
- BOTH_ATTACK2 (0x0009) — frame 15B (layout intercalado do skill)

2) Golden check (byte-a-byte) contra `both_attack_olc_vectors.json` (fonte normativa)

3) Loopback real (127.0.0.1 porta efêmera):
- client envia frame attack1 → server stub memcmp OK → responde damage C1 → client memcmp OK
- client envia frame attack2 → server stub memcmp OK → responde damage C1 → client memcmp OK

## Outputs (registrados)
### Golden check
both_attack1_basic=080007000000c1071101017803  
both_attack2_skill_basic=090009000000c309db00010932013c  
guard_count0=OK  
GOLDEN_CHECK_OK: both_attack1_basic + both_attack2_skill_basic match JSON

### Loopback
LOOPBACK_BOTH_ATTACK_OK  
RESP_SOURCE_JSON=NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_attack_tx_11/attack_tx_11_vectors.json

## Artefatos (sha256)
- Spec: `NEW_ERA_PROTOCOL_MVP_BOTH_ATTACK_TX_SPEC.md` sha256 `5c908975bb00c52785085a8abd8a95c10412a26a7c3eba5c02d8d463f6f229d8`
- Vectors: `both_attack_olc_vectors.json` sha256 `dae7bf4bd4429230acfbbdd6570947c7fd26dc8aca85442c46b98d42c5faac64`
- Core (patched): `NEW_ERA_IMPLEMENTATION/mvp_login/mvp_login_client.cpp` sha256 `31993364c114740b2f8edf5392e4d048a614c30a545ae6ee9471307c03f0d82d`
- Damage resp source json: `NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_attack_tx_11/attack_tx_11_vectors.json` sha256 `4b8c5bfda64299160e899d73ac11f143036286269e6de7e3c4ef15cc35b9c1be`

## Nota
- O loopback usou como RESP o campo `resp_damage_c1_hex` do json acima (golden 1.3-N).
