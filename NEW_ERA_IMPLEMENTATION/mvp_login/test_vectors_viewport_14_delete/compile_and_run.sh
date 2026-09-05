#!/bin/sh
# NEW-ERA — golden VIEWPORT DELETE 0x14 (1.3-I: ciclo spawn->delete). Roda do repo-root; exige exit 0.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN \
    -I NEW_ERA_IMPLEMENTATION/mvp_login \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_14_delete/genvec_viewport_14 \
    NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_14_delete/generate_viewport_14_vectors.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_viewport_14_delete/genvec_viewport_14
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
