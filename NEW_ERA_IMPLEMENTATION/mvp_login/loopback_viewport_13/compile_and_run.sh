#!/bin/sh
# NEW-ERA — loopback VIEWPORT 0x13 (1.3-E: C2 spawn S->C). SOMENTE 127.0.0.1; exit 0 obrigatorio.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN -pthread \
    -I NEW_ERA_IMPLEMENTATION/mvp_login \
    -I NEW_ERA_IMPLEMENTATION/mvp_login/loopback_viewport_13 \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/loopback_viewport_13/loopback_harness \
    NEW_ERA_IMPLEMENTATION/mvp_login/loopback_viewport_13/loopback_harness.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/loopback_viewport_13/loopback_harness
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
