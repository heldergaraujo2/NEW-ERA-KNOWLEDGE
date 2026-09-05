# SKILL TX 0xDB (SendRequestMagicAttack) + RESP damage — golden vectors (1.3-O)

- req **C1 12 B**: `c10cdb0009323c0101010102` (Type 0x0009, (x,y)=(50,60), serial 0x01, Count=1, key 0x0101, skillSerial 0x02)
- resp_damage **C1 10 B**: `c10a1181000123c50044` (= golden 1.3-N/1.3-M; key 0x0100, Damage 0x0123)
- Layout: [C1][size=9+3·Count][DB][Type BE][x][y][Serial][Count] + por alvo [Key BE][SkillSerial]
- spe.Send(**TRUE**) pede encrypt — builder = C1 plain pré-encrypt (wrap C3 [NOT RECOVERED])
- GS: case 0x19 → CGSkillAttackRecv (:121-:122); tradução 0xDB→0x19 [NOT RECOVERED]
- Provas extra no gerador: Count=2 ⇒ 15 B (loop) e guard count=0 rejeitado
