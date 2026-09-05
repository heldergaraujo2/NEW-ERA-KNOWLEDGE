# Viewport spawn 0x13 — golden vectors (1.3-E, C2)

- RESP S->C **C2** (15 B; 1 entidade, 0 buffs): `c2000f1301012302aa64c865c92000`
- expected: count=1, key=0x0123 (291), type=0x02AA (682), x=100, y=200,
  target=101/201, dir=2 (Path=0x20 => 45°), flags=0, buffs=0
- Wire: PWMSG_HEADER :83-:89 + count :202 + PCREATE_MONSTER :588-:600;
  handler :2585-:2712 (stride :2710). NOTA: "ViewPortNumber" do exemplo
  NÃO EXISTE na struct — não inventado (anti-invenção, igual F3:06).
