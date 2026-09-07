# rx_action_magic_vectors.md

Fonte normativa: `NEW_ERA_PROTOCOL_MVP_RX_ACTION_MAGIC_SPEC.md`

## rx_action_0x18_basic
- Frame: `c1 09 18 01 01 04 78 01 02`
- key=0x0101, angleByte=4 (legacy: 135°), action=0x78, target=0x0102

## rx_magic_0x19_success_basic
- Frame: `c1 09 19 00 09 01 01 81 02`
- magic=0x0009, source=0x0101, targetWord=0x8102 => success=1, target=0x0102

## rx_magic_0x19_fail_basic
- Frame: `c1 09 19 00 09 01 01 01 02`
- success=0
