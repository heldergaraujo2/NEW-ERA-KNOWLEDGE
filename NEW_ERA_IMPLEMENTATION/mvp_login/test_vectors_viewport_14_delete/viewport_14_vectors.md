# Viewport DELETE 0x14 — golden vectors (1.3-I: ciclo spawn→delete)

- delete14 **C1 10 B**: `c10a1403010001019999` (count=3; keys BE 0x0100/0x0101/0x9999)
- delete14_trunc **C1 9 B**: `c10a14030100010199` (size diz 0x0A — DEVE falhar)
- Ciclo: 0x12+0x13 → ws=4; delete → ws=2 (ficam 0x0200/0x0202; counts 1/1)
- raw 0x9999: b15=1 → máscara &0x7FFF (:2817) ⇒ key 0x1999 — inexistente, ignorada
- Wire: PHEADER_DEFAULT :113-:118 + PDELETE_CHARACTER :622-:626 (BE, stride 2 :2846)
