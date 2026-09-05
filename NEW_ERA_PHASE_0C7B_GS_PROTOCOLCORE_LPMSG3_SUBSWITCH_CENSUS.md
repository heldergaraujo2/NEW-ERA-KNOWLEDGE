# NEW-ERA — PHASE 0C.7-B: GAMESERVER ProtocolCore `switch(lpMsg[3])` — SUB-SWITCH CENSUS
> Microteste 0C.7-B · SEM REDE · READ-ONLY · 2026-09-04 · Fecha a pendência derivada do Ledger §22.

## 1. Fonte (evidência persistida — nenhum acesso à rede)
| Campo | Valor |
|---|---|
| Arquivo | `NEW_ERA_EVIDENCE/GS_Protocol.cpp__580472e__sha256_e8bb4dbb.cpp` |
| SHA256 | `e8bb4dbb28d1baf23222dbc72762e2ebd55b116dbb8d36e5f017dd744e510f13` (conferido) |
| Tamanho / linhas | 135.014 B · 5.217 · commit `580472e` (raw GitHub, persistido em 0C.7-A-NET) |

## 2. Localização (re-validada nesta análise)
- `void ProtocolCore(BYTE head,BYTE* lpMsg,int size,int aIndex,int encrypt,int serial)` — **:76-1165**.
- `switch(head)` top-level — **:98-1164** (107 cases, sem default — 0C.7-A-NET).
- **Todos os switches internos da função são `switch(lpMsg[3])`** (varredura completa: 26 ocorrências, predicado único; zero switches com outros predicados; zero casos aninhados em profundidade ≥3).

## 3. Resumo numérico
| Métrica | Valor |
|---|---|
| Sub-switches `switch(lpMsg[3])` | **26** (todos a profundidade relativa 1 — corpo direto de um case HEAD) |
| Total de labels `case` (soma) | **155** — **divergência vs esperado ~155: NENHMA** (= exatamente os 155 aninhados excluídos no 0C.7-A-NET; 0 labels em outros contextos) |
| Labels distintos POR sub-switch | 155/155 (nenhuma duplicata dentro de um mesmo sub-switch) |
| Sub-opcodes distintos GLOBAL | **54** |
| Sub-opcodes repetidos em >1 HEAD | **21** (ver §6) |
| `default:` | **1 único** — HEAD **0xBF** @**:656**: `LogAdd(LOG_RED, "[0xBF] Unknown packet BF:%d",lpMsg[3]); break;` (LOG + break — único ponto defensivo do dispatch; o switch(head) top-level é silencioso) |
| Pré-processador dentro de sub-switches | presente em 10 deles (corpos condicionais; nenhum label condicional — consistente com 0C.7-A-NET) |

## 4. Mapa por HEAD (lista COMPLETA dos 155 subcases)

**HEAD 0x3F** (case@175) — switch @**176-196** — 6 labels:
```
 178: case 0x01:   181: case 0x02:   184: case 0x03:   187: case 0x05:   190: case 0x06:   193: case 0x07:
```
**HEAD 0x4C** (case@220) — @**221-238** — 3 labels: `223: 0x00 · 228: 0x01 · 233: 0x03`
**HEAD 0x4D** (case@240) — @**241-288** — 9 labels: `243: 0x00 · 248: 0x01 · 253: 0x0F · 258: 0x10 · 263: 0x11 · 268: 0x12 · 273: 0x13 · 278: 0x14 · 283: 0x15`
**HEAD 0x4E** (case@290) — @**291-309** — 4 labels: `293: 0x00 · 298: 0x01 · 300: 0x08 · 304: 0x09`
**HEAD 0xAA** (case@379) — @**381-398** — 5 labels: `383: 0x01 · 386: 0x02 · 389: 0x03 · 392: 0x07 · 395: 0x09`
**HEAD 0xAF** (case@418) — @**419-427** — 2 labels: `421: 0x01 · 424: 0x02`
**HEAD 0xB1** (case@432) — @**433-438** — 1 label: `435: 0x01`
**HEAD 0xB2** (case@440) — @**441-491** — 16 labels (Castle Siege):
```
 443: 0x00  446: 0x01  449: 0x02  452: 0x03  455: 0x04  458: 0x05  461: 0x06  464: 0x07
 467: 0x08  470: 0x09  473: 0x10  476: 0x12  479: 0x1B  482: 0x1C  485: 0x1D  488: 0x1F
```
**HEAD 0xB7** (case@502) — @**503-511** — 2: `505: 0x01 · 508: 0x04`
**HEAD 0xB9** (case@513) — @**514-522** — 2: `516: 0x02 · 519: 0x05`
**HEAD 0xBC** (case@524) — @**525-533** — 2: `527: 0x00 · 530: 0x01`
**HEAD 0xBD** (case@535) — @**536-547** — 3: `538: 0x00 · 541: 0x03 · 544: 0x09`
**HEAD 0xBF** (case@549) — @**550-659** — 11 labels **+ default@656**:
```
 552: 0x00  555: 0x02  558: 0x05  561: 0x0B  566: 0x0C  571: 0x0D  576: 0x0E  581: 0x17
 586: 0x20  591: 0x51  651: 0x70   |   656: default → LogAdd(LOG_RED,"[0xBF] Unknown packet BF:%d") + break
```
**HEAD 0xD0** (case@694) — @**695-732** — 7: `697: 0x05 · 702: 0x06 · 707: 0x07 · 712: 0x08 · 717: 0x09 · 722: 0x0A · 727: 0x10`
**HEAD 0xD1** (case@734) — @**735-743** — 2: `737: 0x00 · 740: 0x01`
**HEAD 0xD2** (case@745) — @**746-766** — 6: `748: 0x01 · 751: 0x02 · 754: 0x03 · 757: 0x04 · 760: 0x05 · 763: 0x0B`
**HEAD 0xE7** (case@786) — @**787-804** — 3: `789: 0x01 · 794: 0x02 · 799: 0x03`
**HEAD 0xEB** (case@809) — @**810-815** — 1: `812: 0x01`
**HEAD 0xEC** (case@817) — @**818-850** — 6: `820: 0x00 · 825: 0x01 · 830: 0x02 · 835: 0x03 · 840: 0x31 · 845: 0x33`
**HEAD 0xED** (case@852) — @**853-900** — 9: `855: 0x00 · 860: 0x01 · 865: 0x02 · 870: 0x03 · 875: 0x04 · 880: 0x05 · 885: 0x06 · 890: 0x07 · 895: 0x08`
**HEAD 0xEF** (case@902) — @**903-940** — 7: `905: 0x00 · 910: 0x01 · 915: 0x02 · 920: 0x03 · 925: 0x04 · 930: 0x05 · 935: 0x06`
**HEAD 0xF1** (case@942) — @**943-953** — 3: `945: 0x01 · 948: 0x02 · 951: 0x03`
**HEAD 0xF3** (case@955) — @**956-1034** — **22 labels (o maior — família login/personagens)**:
```
 958: 0x00  961: 0x01  964: 0x02  967: 0x03  970: 0x06  973: 0x12  976: 0x15  981: 0x16
 986: 0x21  991: 0x30  994: 0x31  999: 0x52 1004: 0xE6 1007: 0xE7 1010: 0xE8 1013: 0xE9
 1016: 0xEA 1019: 0xEB 1022: 0xEC 1025: 0xED 1028: 0xF0 1031: 0xF1
```
**HEAD 0xF6** (case@1036) — @**1037-1079** — 8: `1039: 0x0A · 1044: 0x0B · 1049: 0x0D · 1054: 0x0F · 1059: 0x10 · 1064: 0x1B · 1069: 0x30 · 1074: 0x31`
**HEAD 0xF7** (case@1081) — @**1082-1089** — 1: `1084: 0x01`
**HEAD 0xF8** (case@1091) — @**1092-1152** — 14:
```
 1094: 0x01  1099: 0x03  1104: 0x09  1109: 0x0B  1114: 0x20  1119: 0x30  1124: 0x32  1129: 0x34
 1132: 0x36  1135: 0x3C  1138: 0x41  1141: 0x43  1146: 0x45  1149: 0x4B
```

**(verificação de soma: 6+3+9+4+5+2+1+16+2+2+2+3+11+7+2+6+3+1+6+9+7+3+22+8+1+14 = 155 ✓)**

## 5. Trechos de evidência (raw verbatim)
**HEAD com sub-switch** (:175-180):
```
  175: 	case 0x3F:
  176: 		switch(lpMsg[3])
  177: 		{
  178: 			case 0x01:
  179: 				gPersonalShop.CGPShopSetItemPriceRecv((PMSG_PSHOP_SET_ITEM_PRICE_RECV*)lpMsg,aIndex);
  180: 				break;
```
**Fechamento de sub-switch** (:193-198):
```
  193: 			case 0x07:
  194: 				gPersonalShop.CGPShopLeaveRecv((PMSG_PSHOP_LEAVE_RECV*)lpMsg,aIndex);
  195: 				break;
  196: 		}
  197: 		break;
  198: 	case 0x40:
```
**O único default** (:651-660, HEAD 0xBF):
```
  651: 			case 0x70:
  652: 				#if(GAMESERVER_UPDATE >= 803) //esse
  653: 				gIllusionTemple.CGIllusionTempleEnterRecv((PMSG_ILLUSION_TEMPLE_ENTER_RECV*)lpMsg,aIndex); //Original
  654: 				#endif
  655: 				break;
  656: 			default:
  657: 				LogAdd(LOG_RED, "[0xBF] Unknown packet BF:%d",lpMsg[3]);
  658: 				break;
  659: 		}
  660: 		break;
```

## 6. Repetições globais (sub-opcode → HEADs onde ocorre)
54 distintos globais; **21 ocorrem em >1 HEAD** (o espaço de sub-opcode é LOCAL a cada HEAD — reutilizado entre famílias):
```
0x00: 0x4C 0x4D 0x4E 0xB2 0xBC 0xBD 0xBF 0xD1 0xEC 0xED 0xEF 0xF3   (12 HEADs)
0x01: 0x3F 0x4C 0x4D 0x4E 0xAA 0xAF 0xB1 0xB2 0xB7 0xBC 0xD1 0xD2 0xE7 0xEB 0xEC 0xED 0xEF 0xF1 0xF3 0xF7 0xF8   (21)
0x02: 0x3F 0xAA 0xAF 0xB2 0xB9 0xBF 0xD2 0xE7 0xEC 0xED 0xEF 0xF1 0xF3   (13)
0x03: 0x3F 0x4C 0xAA 0xB2 0xBD 0xD2 0xE7 0xEC 0xED 0xEF 0xF1 0xF3 0xF8   (13)
0x04: 0xB2 0xB7 0xD2 0xED 0xEF   ·   0x05: 0x3F 0xB2 0xB9 0xBF 0xD0 0xD2 0xED 0xEF
0x06: 0x3F 0xB2 0xD0 0xED 0xEF 0xF3   ·   0x07: 0x3F 0xAA 0xB2 0xD0 0xED
0x08: 0x4E 0xB2 0xD0 0xED   ·   0x09: 0x4E 0xAA 0xB2 0xBD 0xD0 0xF8
0x0A: 0xD0 0xF6   ·   0x0B: 0xBF 0xD2 0xF6 0xF8   ·   0x0D: 0xBF 0xF6   ·   0x0F: 0x4D 0xF6
0x10: 0x4D 0xB2 0xD0 0xF6   ·   0x12: 0x4D 0xB2 0xF3   ·   0x15: 0x4D 0xF3   ·   0x1B: 0xB2 0xF6
0x20: 0xBF 0xF8   ·   0x30: 0xF3 0xF6 0xF8   ·   0x31: 0xEC 0xF3 0xF6
```
(33 sub-opcodes ocorrem em HEAD único — incluindo os de rangos altos: 0xE6-0xF1 de 0xF3, 0x70 de 0xBF, 0x51 de 0xBF, 0x33/0x31 de 0xEC etc.)

## 7. Observações (fato, sem inferência)
- Maior sub-switch: **0xF3 (22)** → 0xB2 (16) → 0xF8 (14) → 0xBF (11). Menores: 0xB1/0xEB/0xF7 (1 cada).
- O **único default** de todo o dispatch (top + sub) é o do HEAD 0xBF — com LOG; heads desconhecidos no nível superior passam em silêncio.
- Sub-opcode `0x01` ocorre em 21 dos 26 HEADs (o valor mais reutilizado).
- Corpos condicionais por `GAMESERVER_UPDATE` em 10 sub-switches (0x4C/0x4D/0x4E/0xBF/0xD0/0xE7/0xEC/0xED/0xEF/0xF6/0xF7/F8 — preproc linhas contadas na extração); labels incondicionais.

## 8. Artefatos
- Este relatório + Ledger §23 + atualizações CURRENT_STATE/MASTER_CHECKPOINT.
- Fonte: `NEW_ERA_EVIDENCE/GS_Protocol.cpp__580472e__sha256_e8bb4dbb.cpp` (inalterada; sha conferido no início do run).
