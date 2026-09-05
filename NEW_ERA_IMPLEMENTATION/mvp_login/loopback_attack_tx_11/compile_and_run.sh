#!/bin/sh
# NEW-ERA — loopback ATTACK TX 0x11 + RESP damage (1.3-N). TWO-TU; SOMENTE 127.0.0.1; exit 0 obrigatorio.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN -pthread \
    -I NEW_ERA_IMPLEMENTATION/mvp_login/loopback_attack_tx_11 \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/loopback_attack_tx_11/loopback_harness \
    NEW_ERA_IMPLEMENTATION/mvp_login/loopback_attack_tx_11/loopback_harness.cpp \
    NEW_ERA_IMPLEMENTATION/mvp_login/mvp_login_client.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/loopback_attack_tx_11/loopback_harness
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
