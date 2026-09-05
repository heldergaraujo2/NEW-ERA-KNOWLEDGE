# BOTH_POSITION (ASIO 0x0006) + equivalência 0x15 — golden vectors (1.3-L)

- both_ok **olc 13 B**: `060007000000c1071501004d58` (key 0x0100 → x/y/target 77/88; hdr3 embutido C1 07 15 OPACO)
- pkt15_ok **C1 7 B**: `c1071501004d58` (clássico — MESMO efeito sobre o estado)
- both_miss **olc 13 B**: `060007000000c1071533330909` (key 0x3333 inexistente → missed=1)
- both_trunc **12 B**: `060007000000c1071501004d` (size diz 7, body 6 — DEVE falhar)
- Equivalência provada: both_ok ⇒ pkt15_ok mantém estado idêntico (campo a campo)
- Wire: enum :7-:26 + ponte :92-:94 + PRECEIVE_MOVE_POSITION header incondicional :892-:898
