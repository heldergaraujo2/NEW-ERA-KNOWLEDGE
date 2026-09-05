# F3:0x30 Option — golden vectors (1.1-E)

- REQ C->S (C3, 57 B): `c3396a22337fdc0ae04c382d18472942014793d21318c9fc10d6021b151a100cc5f6c3c92c20ffad4532857430059e801853d22ba00b54605c`
- RESP S->C stub GS-style SEM Xor (C3, 57 B): `c339cfd81eeddba4413895d6e3ead724ad802d175860cdf8ead724ad858ea6308d97a2ba71081243fef46c44e9dc451b0d0f125202d4d8c4f8`
- expected: hotKey0=0x1234 (swap :9398), 9x 0xFFFF, gameOption=0xA5, QWE, chatLog=1, keyR=2, qwer=100
- Wire real: C1 PLAIN 34 B (Send() default FALSE — spec §2); C3 é convenção NEW-ERA.
