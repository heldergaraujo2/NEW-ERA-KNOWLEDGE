#!/bin/sh
# NEW-ERA — golden F3:0x30 (1.1-E). Roda do repo-root; exige exit 0.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN \
    -I NEW_ERA_IMPLEMENTATION/mvp_login \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_30/genvec_f3_30 \
    NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_30/generate_f3_30_vectors.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_30/genvec_f3_30
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
