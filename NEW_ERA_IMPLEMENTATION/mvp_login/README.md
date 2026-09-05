# NEW-ERA — MVP LOGIN (fluxo 0xF1) — implementação

## Objetivo
Implementar o **MVP de login** cliente-side do NEW-ERA (fluxo `0xF1`: handshake `0x00` → request `0x01` (C3) → result `0x01`), conforme a spec normativa.

## Spec de referência (obrigatória)
- Arquivo: `/home/user/NEW_ERA_PROTOCOL_MVP_LOGIN_SPEC.md`
- SHA256: `e583db6b4ed7e22abcf8803dfdf36ab5af98ef02d7da245b9f87f47deeacf4c4`
- Proveniência do conhecimento: Ledger §36-§44 (microtestes 0C.9-A..F, 0C.10-A/B, 0C.11-A), evidências `NEW_ERA_EVIDENCE/` @580472e.

## Escopo do scaffold (1.0-A)
Apenas estrutura: headers de framing, BuxConvert (provado §41), XorData32 (algoritmo provado §43; tabela = placeholder), stub do CPacketManager, skeleton build/parse. **Nenhum socket.**

## NÃO implementado ainda (próximos passos)
- [ ] 1.0-B: BuxConvert + XorData32 com **tabela real ativada** + construção do pacote `0xF1:0x01` lógico (49 B) e envelope C3
- [ ] CPacketManager/SimpleModulus (Encrypt/Decrypt por bloco `((Key·d)%Modulus)^Xor`; chaves Enc1/Dec2) — hoje é stub
- [ ] Serial por pacote/conexão (TX `g_byPacketSerialSend` / RX `gSerialCheck`)
- [ ] Parser real dos 3 pacotes + máquina de estados (LogIn/CurrentProtocolState)
- [ ] Integração de rede (sockets) — fora do escopo do scaffold
- [ ] Testes/vetores de verificação (round-trip BuxConvert; XorData vs tabela canônica)
- [ ] Lado servidor/JS (fica em peers separados; spec §5)

## Regras herdadas (FASE 1)
- Implementação ISOLADA em `NEW_ERA_IMPLEMENTATION/` — nada no repo legado (`muonline/`), nada em `NEW_ERA_EVIDENCE/`.
- Anti-invenção: todo comportamento referencia a spec (seção/linhas); gaps ficam como `TODO(spec §…)`.
