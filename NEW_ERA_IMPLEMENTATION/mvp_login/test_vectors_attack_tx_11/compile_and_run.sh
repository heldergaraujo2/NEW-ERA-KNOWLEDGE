#!/bin/sh
# NEW-ERA — golden ATTACK TX 0x11 + RESP damage (1.3-N). TWO-TU; roda do repo-root; exige exit 0.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_attack_tx_11/genvec_attack_tx_11 \
    NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_attack_tx_11/generate_attack_tx_11_vectors.cpp \
    NEW_ERA_IMPLEMENTATION/mvp_login/mvp_login_client.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_attack_tx_11/genvec_attack_tx_11
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
