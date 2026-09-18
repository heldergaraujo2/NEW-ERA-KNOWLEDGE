# 0D-2 — Renderer Material Evaluator

## Status
**EXECUTED / DELIVERED / PASS**

## Scope
Deterministic reconstruction of the material/state decision layer visible in pinned upstream wongddd/muonline@580472e ZzzBMD.cpp/ZzzBMD.h. This is a NEW-ERA compatibility abstraction, not a claim that the original internal code used this structure.

## Implemented
NEW_ERA_IMPLEMENTATION/renderer/renderer_material.h

The evaluator captures recovered flag families: texture, chrome variants, oil, bright, dark, lightmap and no-depth. It produces an explicit material path plus alpha-blend, alpha-test, depth-test and depth-write decisions.

## Regression
NEW_ERA_IMPLEMENTATION/renderer/test_renderer_material.cpp

Covered opaque texture, bright texture, bright non-textured, translucent texture, RGBA alpha-test, NODEPTH, chrome and oil.

## Validation
Exact committed header/test logic was compiled with g++ -std=c++17 -Wall -Wextra -Wpedantic.

Observed: 0D-2 renderer material evaluator: PASS
Exit status: 0.

## Evidence basis
Pinned upstream ZzzBMD.h defines the recovered render flags. ZzzBMD.cpp shows the corresponding state branches, including texture/bright/dark/lightmap behavior, NODEPTH, chrome texture selection and the GL_TRIANGLES mesh path.

## Boundary
No claim is made for exact original state-cache implementation, complete undocumented flag combinations, GPU equivalence, performance equivalence or original runtime execution.
