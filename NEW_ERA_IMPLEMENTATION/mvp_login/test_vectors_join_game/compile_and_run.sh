#!/bin/sh
# NEW-ERA — golden JOIN GAME (1.3-C: ASIO TX + C1 F3:03 RX). Roda do repo-root; exige exit 0.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN \
    -I NEW_ERA_IMPLEMENTATION/mvp_login \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_join_game/genvec_join_game \
    NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_join_game/generate_join_game_vectors.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_join_game/genvec_join_game
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
