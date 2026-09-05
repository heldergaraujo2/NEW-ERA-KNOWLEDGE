# Position update 0x15 — golden vectors (1.3-J: spawn→move→delete)

- move_ok **C1 7 B**: `c1071501004d58` (key 0x0100 → 77,88; target=idem :1764)
- move_miss **C1 7 B**: `c1071533330909` (key 0x3333 inexistente → ignored, missed=1)
- move_trunc **6 B**: `c1071501004d` (sem Y — DEVE falhar, ws intocado)
- Ciclo: ws=4 → move(77/88) → miss → trunc falho → delete → **ws=2**
- Wire: PRECEIVE_MOVE_POSITION :892-:898 (Key BE sem máscara :1749)
