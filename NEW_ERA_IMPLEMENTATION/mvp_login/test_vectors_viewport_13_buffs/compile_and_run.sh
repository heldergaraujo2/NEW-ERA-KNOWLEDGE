#!/bin/sh
# NEW-ERA — golden VIEWPORT 0x13 BUFFS (1.3-F: stride variável). Roda do repo-root; exige exit 0.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN \
    -I NEW_ERA_IMPLEMENTATION/mvp_login \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_13_buffs/genvec_viewport_13_buffs \
    NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_13_buffs/generate_viewport_13_buffs_vectors.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_13_buffs/genvec_viewport_13_buffs
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
