# NEW_ERA_PROTOCOL_MVP_RX_INVENTORY_F3_10_SPEC.md
Data: 2026-09-07  
Status: NORMATIVO (Fase 1 — 1.3-V P2)

## Objetivo
Congelar o contrato on-wire (RX) para o pacote de inventário:
- `F3:10` (Item List / Inventory List)

Base (evidência): Ledger §87 + reports `EVIDENCE/1.3-V*`.

---

## 1) Framing (C2 variável)
Formato geral:
`[C2][sizeH][sizeL][head=F3][sub=10][count] + count * entry`

- `C2` = 0xC2 (plain)
- `size` = **u16 big-endian**, tamanho TOTAL do pacote (incluindo header e payload)
- `head` = 0xF3
- `sub`  = 0x10
- `count` = número de entries

### Tamanho esperado
`size == 6 + 13*count`

(6 = 5 bytes do header C2 + 1 byte do count)

---

## 2) Entry (stride 13 bytes)
Cada item tem:
- `slot` (u8)
- `ItemInfo[12]` (12 bytes)

Ou seja: `entry = [slot][12 bytes]`

Observação: `ItemInfo` é "blob" de 12B (packed). A interpretação "humana" (tipo/level/opt) é feita por conversão (`ItemByteConvert` no GS; parsing no client), mas o contrato on-wire é o blob.

---

## 3) Guards normativos (parser NEW-ERA)
O parser deve rejeitar:
- frame[0] != 0xC2
- size < 6
- size != len(frame)
- head/sub != (0xF3,0x10)
- size != 6 + 13*count (inconsistência)
- count tão grande que estoure limites internos (definir teto local seguro; ex.: `count <= 255` e `size <= 4096`)

Aceitar `count=0` (inventário vazio) como válido.

---

## 4) Compatibilidade
Existe variante cifrada C4 (`0xC4`) no protocolo (via `setE`), mas **o pin atual usa C2** para F3:10 (`header.set(...)`).
O MVP cobre C2 primeiro.
