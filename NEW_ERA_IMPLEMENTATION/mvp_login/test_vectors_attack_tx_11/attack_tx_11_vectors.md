# ATTACK TX 0x11 (SendRequestAttack) + RESP damage — golden vectors (1.3-N)

- req **C1 7 B**: `c1071101017803` (targetKey 0x0101, dir 3, AT_ATTACK1=0x78; builder real)
- resp_damage **C1 10 B**: `c10a1181000123c50044` (= attack_ok do golden 1.3-M; raw 0x8100 ⇒ key 0x0100, Damage 0x0123)
- Ciclo mínimo provado no golden: apply resp em ws c/ dummy 0x0100 ⇒ lastDamage=0x0123 (missed=0)
- Bonus: round-trip BuildAsio_BOTH_MESSAGE_FromClassicC1 -> Extract (13 B; uso C->S NÃO evidenciado)
- Wire TX: macro :518-:527 · AT_ATTACK1=120 :1497 · Key BE sem máscara :524 · C1 plain (Send defaults)
