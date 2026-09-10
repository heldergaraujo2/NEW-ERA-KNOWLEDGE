# EVIDENCE — TS-3 (Test Server aceita múltiplas conexões)

## Objetivo
Melhorar o NEW-ERA Test Server para não encerrar após 1 cliente, permitindo repetir testes (WIN client / cliente real) sem reiniciar o servidor.

## Mudança
- O servidor agora fica em loop de accept: após o cliente desconectar, fecha o socket do cliente e volta a aceitar outro.

## Arquivo alterado
- NEW_ERA_IMPLEMENTATION/test_server/new_era_test_server.cpp

## Resultado observado (Windows)
Log do servidor ao aceitar 2 clientes sequenciais:

```text
[NEW-ERA TS] Listening on 0.0.0.0:55901 ...
[NEW-ERA TS] repoRoot=.
[NEW-ERA TS] Client connected from 127.0.0.1:53916
[NEW-ERA TS] RX id=0x7 size=5
[NEW-ERA TS] TX scripted BOTH_MESSAGE: F3:03 (66 B), F3:10 count1 (19 B), F3:10 count0 (6 B)
[NEW-ERA TS] Read header ended: peer closed
[NEW-ERA TS] Client disconnected
[NEW-ERA TS] Client connected from 127.0.0.1:53924
[NEW-ERA TS] RX id=0x7 size=5
[NEW-ERA TS] TX scripted BOTH_MESSAGE: F3:03 (66 B), F3:10 count1 (19 B), F3:10 count0 (6 B)
[NEW-ERA TS] Read header ended: peer closed
[NEW-ERA TS] Client disconnected
```

## Nota
Esta entrega não altera o protocolo: apenas torna o servidor mais usável para repetição de testes.
