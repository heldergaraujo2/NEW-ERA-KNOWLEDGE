#!/bin/sh
# NEW-ERA — loopback F3:0x30 (1.1-E). SOMENTE 127.0.0.1; imprime OK apenas se exit 0.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN -pthread \
    -I NEW_ERA_IMPLEMENTATION/mvp_login \
    -I NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f3_30 \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f3_30/loopback_harness \
    NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f3_30/loopback_harness.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/loopback_f3_30/loopback_harness
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
