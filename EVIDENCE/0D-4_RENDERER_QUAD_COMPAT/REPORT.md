# 0D-4 — Quad Compatibility Batch

## Status
**EXECUTED / DELIVERED / PASS**

## Scope
Compatibility geometry layer for the four-corner `Triangle_t` representation recovered from pinned upstream `wongddd/muonline@580472e`. Unlike 0D-3, this path handles a four-corner polygon explicitly so the optimization layer does not silently discard non-triangle source geometry.

## Implemented
`NEW_ERA_IMPLEMENTATION/renderer/renderer_quad_compat.h`

For `Polygon == 4`, the source quad is expanded to four GPU-oriented corners and indexed as two triangles:
`0,1,2, 0,2,3`.
Vertex/normal/UV associations are preserved per source corner.

Invalid polygon type and null input are rejected.

## Regression
`NEW_ERA_IMPLEMENTATION/renderer/test_renderer_quad_compat.cpp`

Checks four source corners, six output indices, exact diagonal/order, and final UV association.

## Validation
The exact core/test logic was compiled with C++17 using `g++ -std=c++17 -Wall -Wextra -Wpedantic` in an isolated fixture reproducing the committed dependency header.

Observed:
`0D-4 quad compatibility triangulation: PASS`

Exit status: 0.

## Boundary
This is a compatibility triangulation policy for the NEW-ERA batch layer. It does not claim that every original quad was rasterized through this exact diagonal, nor does it claim original GPU behavior or performance equivalence.
