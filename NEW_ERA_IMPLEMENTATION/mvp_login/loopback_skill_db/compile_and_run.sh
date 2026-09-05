#!/bin/sh
# NEW-ERA — loopback SKILL TX 0xDB + RESP damage (1.3-O). TWO-TU; SOMENTE 127.0.0.1; exit 0 obrigatorio.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN -pthread \
    -I NEW_ERA_IMPLEMENTATION/mvp_login/loopback_skill_db \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/loopback_skill_db/loopback_harness \
    NEW_ERA_IMPLEMENTATION/mvp_login/loopback_skill_db/loopback_harness.cpp \
    NEW_ERA_IMPLEMENTATION/mvp_login/mvp_login_client.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/loopback_skill_db/loopback_harness
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
