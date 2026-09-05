#!/bin/sh
# NEW-ERA — loopback WORLDTICK (1.3-H2B: 0x12+0x13+trunc, framing len16LE).
# TWO-TU por comando: harness LINKA com ../mvp_login_client.cpp. SOMENTE 127.0.0.1.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN -pthread \
    -I NEW_ERA_IMPLEMENTATION/mvp_login \
    -I NEW_ERA_IMPLEMENTATION/mvp_login/loopback_worldtick \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/loopback_worldtick/loopback_harness \
    NEW_ERA_IMPLEMENTATION/mvp_login/loopback_worldtick/loopback_harness.cpp \
    NEW_ERA_IMPLEMENTATION/mvp_login/mvp_login_client.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/loopback_worldtick/loopback_harness
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
