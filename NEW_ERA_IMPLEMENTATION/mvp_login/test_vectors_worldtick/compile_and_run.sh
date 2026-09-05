#!/bin/sh
# NEW-ERA — golden WORLDTICK (1.3-H2A: 0x12+0x13+trunc). TWO-TU por comando:
# gerador LINKA com ../mvp_login_client.cpp (espelho ODR 1.3-H no gerador).
# Roda do repo-root; exige exit 0.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN \
    -I NEW_ERA_IMPLEMENTATION/mvp_login \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_worldtick/genvec_worldtick \
    NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_worldtick/generate_worldtick_vectors.cpp \
    NEW_ERA_IMPLEMENTATION/mvp_login/mvp_login_client.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_worldtick/genvec_worldtick
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
