#!/bin/sh
# NEW-ERA — golden F3:0x02 (1.1-D). Roda do repo-root; exige exit 0.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN \
    -I NEW_ERA_IMPLEMENTATION/mvp_login \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_02/genvec_f3_02 \
    NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_02/generate_f3_02_vectors.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_02/genvec_f3_02
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
