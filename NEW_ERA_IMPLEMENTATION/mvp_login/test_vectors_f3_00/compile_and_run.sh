#!/bin/sh
# NEW-ERA — golden F3:0x00 (1.1-C). Roda do repo-root; exige exit 0.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN \
    -I NEW_ERA_IMPLEMENTATION/mvp_login \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_00/genvec_f3_00 \
    NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_00/generate_f3_00_vectors.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_f3_00/genvec_f3_00
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
