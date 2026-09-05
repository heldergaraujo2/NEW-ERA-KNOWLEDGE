#!/bin/sh
# NEW-ERA — loopback VIEWPORT DELETE 0x14 (1.3-I: ciclo spawn->delete). SOMENTE 127.0.0.1.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN -pthread \
    -I NEW_ERA_IMPLEMENTATION/mvp_login \
    -I NEW_ERA_IMPLEMENTATION/mvp_login/loopback_viewport_14_delete \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/loopback_viewport_14_delete/loopback_harness \
    NEW_ERA_IMPLEMENTATION/mvp_login/loopback_viewport_14_delete/loopback_harness.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/loopback_viewport_14_delete/loopback_harness
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
