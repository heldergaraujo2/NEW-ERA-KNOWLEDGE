#!/bin/sh
# NEW-ERA — golden SKILL TX 0xDB + RESP damage (1.3-O). TWO-TU; roda do repo-root; exige exit 0.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_skill_db/genvec_skill_db \
    NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_skill_db/generate_skill_db_vectors.cpp \
    NEW_ERA_IMPLEMENTATION/mvp_login/mvp_login_client.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors_skill_db/genvec_skill_db
rc=$?
if [ "$rc" -eq 0 ]; then echo "OK"; fi
exit $rc
