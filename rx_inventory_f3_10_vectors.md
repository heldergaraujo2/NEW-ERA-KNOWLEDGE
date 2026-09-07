# rx_inventory_f3_10_vectors.md

Fonte normativa: `NEW_ERA_PROTOCOL_MVP_RX_INVENTORY_F3_10_SPEC.md`

## inv_f3_10_count1_slot0_pattern
- Frame hex: `c2 00 13 f3 10 01 00 01 02 03 04 05 06 07 08 09 0a 0b 0c`
- size=0x0013 (19), count=1
- entry0: slot=0, ItemInfo=`01..0C` (12 bytes)

## inv_f3_10_count0_empty
- Frame hex: `c2 00 06 f3 10 00`
- size=0x0006 (6), count=0

## inv_f3_10_size_mismatch_invalid
- Frame hex: `c2 00 12 f3 10 01 ...`
- inválido: size != len(frame)
