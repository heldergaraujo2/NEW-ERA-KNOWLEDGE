#!/bin/sh
# NEW-ERA — golden BOTH_POSITION ASIO (1.3-L: olc 0x0006 + equiv 0x15). Roda do repo-root; exige exit 0.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN \
    -I NEW_ERA_IMPLEMENTATION/mvp_login \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_both_position_0006/genvec_both_position \
    NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_both_position_0006/generate_both_position_vectors.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_both_position_0006/genvec_both_position
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
