# rx_equipment_f3_13_vectors.md

Fonte normativa: `NEW_ERA_PROTOCOL_MVP_RX_EQUIPMENT_F3_13_SPEC.md`

## equip_f3_13_basic
- Frame (24B): `c1 18 f3 13 01 01 00 01 02 ... 11`
- index=0x0101
- CharSet[18]=`00..11`
- class=0x00, equipment17=`01..11`

## equip_f3_13_size_mismatch_invalid
- Frame inválido: size byte 0x17 com 24B de payload (deve rejeitar).
