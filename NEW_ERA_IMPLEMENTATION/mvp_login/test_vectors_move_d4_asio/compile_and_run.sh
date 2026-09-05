#!/bin/sh
# NEW-ERA — golden BOTH_MOVE ASIO (1.3-K: olc 0x0007). Roda do repo-root; exige exit 0.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN \
    -I NEW_ERA_IMPLEMENTATION/mvp_login \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_move_d4_asio/genvec_move_d4_asio \
    NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_move_d4_asio/generate_move_d4_asio_vectors.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_move_d4_asio/genvec_move_d4_asio
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
