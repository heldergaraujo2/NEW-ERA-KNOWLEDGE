# BOTH_MOVE (ASIO 0x0007) — golden vectors (1.3-K)

- move_ok **olc 11 B**: `07000500000001004d5830` (key 0x0100 → target 77/88; Path0=0x30 dir=3 ⇒ 90°)
- move_miss **olc 11 B**: `0700050000003333090910` (key 0x3333 inexistente → missed=1)
- move_trunc **10 B**: `07000500000001004d58` (size diz 5, body 4 — DEVE falhar)
- Apply: dir/angle/target atualizam; **x/y inalterados** (PathFinding2 :1730 interpola)
- Wire: ponte :95-:96 + Defined_Global :6 + PMOVE_CHARACTER headerless :611-:620
