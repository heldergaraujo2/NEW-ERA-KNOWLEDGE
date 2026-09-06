# both_attack_olc_vectors.md

Fonte normativa: `NEW_ERA_PROTOCOL_MVP_BOTH_ATTACK_TX_SPEC.md`

## both_attack1_basic
- Frame hex: `080007000000c1071101017803`
- Interpretação:
  - olc header: id=0x0008, size=7
  - body: `C1 07 11 01 01 78 03`

## both_attack2_skill_basic
- Frame hex: `090009000000c309db00010932013c`
- Interpretação:
  - olc header: id=0x0009, size=9
  - body: `C3 09 DB 00 01 09 32 01 3C`

## both_attack2_count0_invalid_local_guard
- Sem frame: deve falhar no builder (guard local).
