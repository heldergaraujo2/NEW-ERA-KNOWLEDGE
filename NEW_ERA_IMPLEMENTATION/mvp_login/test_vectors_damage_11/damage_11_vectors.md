# DAMAGE RX 0x11 (PACKET_ATTACK) + túnel BOTH_MESSAGE 0x000C — golden vectors (1.3-M)

- attack_ok **C1 10 B**: `c10a1181000123c50044` (raw 0x8100 ⇒ Success=1, key 0x0100; Damage 0x0123; DT 0xC5 ⇒ type=5/double/combo; Shield 0x0044)
- attack_miss **C1 10 B**: `c10a1133330010000000` (key 0x3333 inexistente → missed=1)
- attack_trunc **9 B**: `c10a1181000123c500` (sem ShieldL — DEVE falhar)
- both_ok **olc 16 B**: `0c000a000000c10a1181000123c50044` (0x000C + inner attack_ok — túnel)
- both_trunc **15 B**: `0c000a000000c10a1181000123c500` (size diz 10, body 9 — DEVE falhar)
- Apply: lastDamage=0x0123 (c->Hit :3188); miss contabilizada; trunc não altera estado
- Wire: PACKET_ATTACK=0x11 :26 · PRECEIVE_ATTACK :674-:685 · dispatch :13143-:13144 · túnel :99-:137
