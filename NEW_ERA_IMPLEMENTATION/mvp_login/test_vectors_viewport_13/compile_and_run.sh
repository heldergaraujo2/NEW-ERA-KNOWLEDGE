#!/bin/sh
# NEW-ERA — golden VIEWPORT 0x13 (1.3-E: C2 spawn). Roda do repo-root; exige exit 0.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN \
    -I NEW_ERA_IMPLEMENTATION/mvp_login \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_13/genvec_viewport_13 \
    NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_13/generate_viewport_13_vectors.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_13/genvec_viewport_13
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
