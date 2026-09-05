#!/bin/sh
# NEW-ERA — golden MOVE 0x15 (1.3-J: ciclo spawn->move->delete). Roda do repo-root; exige exit 0.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN \
    -I NEW_ERA_IMPLEMENTATION/mvp_login \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_move_15/genvec_move_15 \
    NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_move_15/generate_move_15_vectors.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_move_15/genvec_move_15
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
