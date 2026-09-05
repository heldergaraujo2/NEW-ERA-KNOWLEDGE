#!/bin/sh
# NEW-ERA — golden DAMAGE RX 0x11 + túnel BOTH_MESSAGE 0x000C (1.3-M). TWO-TU; roda do repo-root; exige exit 0.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_damage_11/genvec_damage_11 \
    NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_damage_11/generate_damage_11_vectors.cpp \
    NEW_ERA_IMPLEMENTATION/mvp_login/mvp_login_client.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_damage_11/genvec_damage_11
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
