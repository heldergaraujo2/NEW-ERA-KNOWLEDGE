# Test Vectors — MVP Login 0xF1 (1.0-F)

Golden vectors **determinísticos** (sem rede, sem sockets, sem RNG) para regressão do TX/RX do MVP de login.

## Regenerar
```sh
sh NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors/compile_and_run.sh   # da raiz do workspace
```
Compila `generate_vectors.cpp` (incluindo o TU `mvp_login_client.cpp` com `-DNEWERA_MVP_NO_MAIN`) e executa `genvec`, que reescreve `login_vectors.json` / `login_vectors.md` e valida TODOS os asserts (aborta em falha).

## Casos
| Caso | Descrição | Valida |
|---|---|---|
| **T1** | login request C→S (ID/PASS "111", Tick 0x11223344, Ver [1..5], Serial16 00..0F, serialPkt 1) | plain 49 B campo a campo; envelope C3 68 B; **roundtrip cripto com Dec1 DERIVADO (inverso modular de Enc1 — §48)** |
| **R1** | F1:00 handshake simulado com Xor+Encrypt (modelo stream) | RX completo: decrypt → serial → unXor → parse (result/heroKey/versionOk) |
| **R2** | F1:01 login result estilo GS real (**Encrypt SEM Xor** — DataSend :443-:470) | RX com `streamXored=false` → Value=1 |

## Chaves
Reais de `../keys/` (Enc1 TX / Dec2 RX). Pares ausentes (Dec1/Enc2) **derivados por inverso modular** em código (`ModInv`), conforme relação provada no §48.

## Nota de auditoria (1.0-F)
Os vetores **pegaram um bug real** no `DecryptBlock`: o retorno de `AddBits` é `targetPos+size` (:469) e não deve ser usado para avançar a posição da FONTE no RX — o evidence avança `BitPos += 16/2` explicitamente (:388-:391). Corrigido antes da geração; vetores agora travam essa classe de regressão.
