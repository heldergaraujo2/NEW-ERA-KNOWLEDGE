# NEW-ERA — PHASE 0C.7-A-NET: GAMESERVER ProtocolCore switch(head) — CENSUS
> Microteste 0C.7-A-NET (rede ultra-restrita: 1 URL) · READ-ONLY · consolidação 2026-09-04
> Fecha a Pendência §15.1 do Ledger (contagem canônica do switch(head) do ProtocolCore GS) e resolve **H4**.

## 1. Fonte (evidência persistida)
| Campo | Valor |
|---|---|
| URL (única acessada) | `https://raw.githubusercontent.com/wongddd/muonline/580472e/Source%20Server/GameServer/GameServer/Protocol.cpp` |
| Commit canônico | `580472e0d5723f9709cbad594f233deb07f9f351` |
| HTTP / tamanho / linhas | **200** · **135.014 B** (= registro prévio do Ledger) · 5.217 linhas |
| SHA256 | `e8bb4dbb28d1baf23222dbc72762e2ebd55b116dbb8d36e5f017dd744e510f13` |
| Obtenção original / re-validação | 2026-09-03 (0C.7-A-NET) / **2026-09-04 12:06 UTC (09:06 America/Sao_Paulo)** — re-download autorizado, sha idêntico |
| Cópia persistida | `NEW_ERA_EVIDENCE/GS_Protocol.cpp__580472e__sha256_e8bb4dbb.cpp` (+ `.SHA256.txt`) — sha256 da cópia conferido idêntico |

Método: parser brace-depth **string/comment-aware** (mesma metodologia da auditoria 0C.5-B-audit), pré-processador excluído do depth-matching; labels `case` capturados apenas a profundidade 1 (corpo direto do switch). Re-derivação independente na cópia persistida reproduziu todos os números (107/107/155/default=[]).

## 2. Localização
- **Assinatura** (raw :76): `void ProtocolCore(BYTE head,BYTE* lpMsg,int size,int aIndex,int encrypt,int serial) // OK` — `head` entra como **parâmetro** (não é extraído do buffer na função).
- **Função**: linhas **76-1165** (1.090 linhas; `{` :77).
- **switch(head) top-level**: **:98** (`switch(head)` · `{` :99) → **fecha :1164** (`}`); função fecha :1165. Único switch top-level (varredura de switches a profundidade relativa 1 no corpo da função).
- **SEM `default:`** — varredura a profundidade 1 em todo o range 98-1164: **0 labels default**. head desconhecido atravessa o switch em silêncio (espelha o comportamento do cliente: default = break/TRUE).

## 3. Resultado
- **Total de case labels top-level: 107** · **distintos: 107** · duplicados: **0**.
- Fallthrough/grupos top-level: **nenhum** (cada case com corpo + break).
- **1 case de corpo vazio**: `case 0x8A:` @:353 (`break;` apenas).
- **155 labels `case` aninhados** em sub-switches `switch(lpMsg[3])` (:176/:221/:241/:291/:381/:419/:433/:441/:503/:514/:525/…) — **excluídos** do censo top-level (escopo).
- Pré-processador dentro do switch: **171 linhas** (`#if/#endif` de `GAMESERVER_UPDATE` 202→803, `GAMESERVER_TYPE==1`, 1 `#else` @:399). **NENHUM label `case` condicional** (preproc-depth = 0 nos 107 labels) → o conjunto de labels é **independente do build**; apenas os CORPOS dos handlers variam.
- **H4 CONFIRMADO**: `case PROTOCOL_CODE4:` **@:771** → `gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)lpMsg,aIndex,0);` @:772 — CODE4 (0xDB no build LANG=1) TEM uso real no servidor (o cliente não o tem entre seus 156 cases top-level — assimetria legada registrada).

## 4. ERRATA / SUPERSEDED (sem editar o passado — ver Ledger §22)
- A nota do Ledger §5 (0C.6-A-2): "vizinho `case ProtocolHead::BOTH_POSITION:`@110" está **INCORRETA** → **SUPERSEDED**.
- Evidência (raw :108-111):
```
  108: 			break;
  109: 		case 0x0E:
  110: 			CGLiveClientRecv((PMSG_LIVE_CLIENT_RECV*)lpMsg,aIndex);   ← :110 é CORPO do case 0x0E
  111: 			break;
```
- `ProtocolHead::` NÃO ocorre como case em nenhuma linha do arquivo — apenas em `PacketSend` (:1441/:1454 `BOTH_POSITION`, :1938/:1951 `BOTH_MOVE`, :3119 `SERVER_CONNECT`). A lista do Ledger §10.4 (que NÃO incluía @110) permanece **correta**.

## 5. CENSO COMPLETO — 107 cases top-level (linha -> label)
```
 100: case 0x00:              103: case 0x02:              106: case 0x03:
 109: case 0x0E:              112: case PROTOCOL_CODE2:    115: case PROTOCOL_CODE3:
 118: case 0x18:              121: case 0x19:              124: case 0x1B:
 127: case 0x1C:              130: case 0x1E:              133: case 0x22:
 136: case 0x23:              139: case 0x24:              142: case 0x26:
 145: case 0x30:              148: case 0x31:              151: case 0x32:
 154: case 0x33:              157: case 0x34:              160: case 0x36:
 163: case 0x37:              166: case 0x3A:              169: case 0x3C:
 172: case 0x3D:              175: case 0x3F:              198: case 0x40:
 201: case 0x41:              204: case 0x42:              207: case 0x43:
 210: case 0x4A:              215: case 0x4B:              220: case 0x4C:
 240: case 0x4D:              290: case 0x4E:              311: case 0x50:
 314: case 0x51:              317: case 0x52:              320: case 0x53:
 323: case 0x54:              326: case 0x55:              329: case 0x57:
 332: case 0x61:              335: case 0x66:              338: case 0x81:
 341: case 0x82:              344: case 0x83:              347: case 0x86:
 350: case 0x87:              353: case 0x8A:  (vazio)     355: case 0x8E:
 358: case 0x90:              361: case 0x91:              364: case 0x9A:
 367: case 0xA0:              370: case 0xA2:              373: case 0xA7:
 376: case 0xA9:              379: case 0xAA:              403: case 0xAB:
 408: case 0xAC:              413: case 0xAE:              418: case 0xAF:
 429: case 0xB0:              432: case 0xB1:              440: case 0xB2:
 493: case 0xB3:              496: case 0xB4:              499: case 0xB5:
 502: case 0xB7:              513: case 0xB9:              524: case 0xBC:
 535: case 0xBD:              549: case 0xBF:              661: case 0xC0:
 664: case 0xC1:              667: case 0xC2:              670: case 0xC3:
 673: case 0xC4:              676: case 0xC5:              679: case 0xC7:
 682: case 0xC8:              685: case 0xC9:              688: case 0xCA:
 691: case 0xCB:              694: case 0xD0:              734: case 0xD1:
 745: case 0xD2:              768: case PROTOCOL_CODE1:    771: case PROTOCOL_CODE4:  ← H4
 774: case 0xE1:              777: case 0xE2:              780: case 0xE5:
 783: case 0xE6:              786: case 0xE7:              806: case 0xE9:
 809: case 0xEB:              817: case 0xEC:              852: case 0xED:
 902: case 0xEF:              942: case 0xF1:              955: case 0xF3:
1036: case 0xF6:             1081: case 0xF7:             1091: case 0xF8:
1154: case 0xFB:             1159: case 0xFC:
```
(Valores de build LANG=1: PROTOCOL_CODE1=0xD4 · CODE2=0x11 · CODE3=0x15 · CODE4=0xDB — origem 0C.6-C, não deste arquivo.)

## 6. Trechos de evidência (raw verbatim)
**Cabeçalho da função + início do switch** (:76-101):
```
   76: void ProtocolCore(BYTE head,BYTE* lpMsg,int size,int aIndex,int encrypt,int serial) // OK
   77: {
   78: 	//if(gObj[aIndex].Type == OBJECT_USER && gHackPacketCheck.CheckPacketHack(aIndex,head,((lpMsg[0]==0xC1)?lpMsg[3]:lpMsg[4]),encrypt,serial) == 0)
   79: 	//{
   80: 	//	return;
   81: 	//}
   98: 	switch(head)
   99: 	{
  100: 		case 0x00:
  101: 			CGChatRecv((PMSG_CHAT_RECV*)lpMsg,aIndex);
```
**Fechamento do switch e da função** (:1160-1165):
```
 1160: 			#if(GAMESERVER_UPDATE>=801)
 1161: 			CGSNSDataLogRecv((PMSG_SNS_DATA_LOG_RECV*)lpMsg,aIndex);
 1162: 			#endif
 1163: 			break;
 1164: 	}
 1165: }
```
**case PROTOCOL_CODE4** (:768-773):
```
  768: 		case PROTOCOL_CODE1:
  769: 			CGMoveRecv((PMSG_MOVE_RECV*)lpMsg,aIndex);
  770: 			break;
  771: 		case PROTOCOL_CODE4:
  772: 			gSkillManager.CGMultiSkillAttackRecv((PMSG_MULTI_SKILL_ATTACK_RECV*)lpMsg,aIndex,0);
  773: 			break;
```

## 7. Limitações
- Macros PROTOCOL_CODE* não resolvidas no texto (valores do build por 0C.6-C).
- Corpos condicionais por build (171 linhas de pré-processador); labels incondicionais.
- Sub-switches `lpMsg[3]` (155 labels) NÃO censuados — candidatos a 0C.7-B.
- Fallthroughs internos de sub-switches não auditados.

## 8. Artefatos
- `NEW_ERA_EVIDENCE/GS_Protocol.cpp__580472e__sha256_e8bb4dbb.cpp` (135.014 B, sha256 conferido)
- `NEW_ERA_EVIDENCE/GS_Protocol.cpp__580472e__sha256_e8bb4dbb.SHA256.txt`
- Ledger §22 (registro append-only) · atualizações em CURRENT_STATE/MASTER_CHECKPOINT
