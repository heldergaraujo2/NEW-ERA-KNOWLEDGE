#!/bin/sh
# NEW-ERA — regenerar golden vectors (1.0-F). Rodar de qualquer dir; ajusta a raiz.
set -e
cd "$(dirname "$0")/../../.."
g++ -std=c++17 -O0 -g -DNEWERA_MVP_NO_MAIN \
    -I NEW_ERA_IMPLEMENTATION/mvp_login \
    -o NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors/genvec \
    NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors/generate_vectors.cpp
NEW_ERA_IMPLEMENTATION/mvp_login/test_vectors/genvec
