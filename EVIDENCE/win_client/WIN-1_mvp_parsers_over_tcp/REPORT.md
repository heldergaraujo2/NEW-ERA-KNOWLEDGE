# EVIDENCE - WIN-1 (Cliente Windows parseando pacotes ao vivo via TCP)

## Objetivo
Validar que o core MVP (mvp_login_client.cpp) roda no Windows e consegue parsear F3:03 e F3:10 recebidos ao vivo via BOTH_MESSAGE (0x000C).

## Arquivos novos
- NEW_ERA_IMPLEMENTATION/mvp_login/win_client/win_client.cpp
- NEW_ERA_IMPLEMENTATION/mvp_login/win_client/CMakeLists.txt

## Como reproduzir (Windows 11)
Build:
  & "C:\Program Files\CMake\bin\cmake.exe" -S NEW_ERA_IMPLEMENTATION\mvp_login\win_client -B build\win_client
  & "C:\Program Files\CMake\bin\cmake.exe" --build build\win_client --config Release

Run (2 terminais):
  1) .\build\test_server\Release\new_era_test_server.exe 55901 .
  2) .\build\win_client\Release\new_era_win_client.exe 127.0.0.1 55901

## Saída observada (WIN-1)
[WIN-1] RX#0 id=0x000C size=66  -> parse F3:03 OK (x=50,y=60,map=0,dir=3,money=287454020)
[WIN-1] RX#1 id=0x000C size=19  -> parse F3:10 OK (count=1)
[WIN-1] RX#2 id=0x000C size=6   -> parse F3:10 OK (count=0)

## Nota
build/ é artefato local e não deve ser commitado.
