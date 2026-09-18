# 0D-3 — Renderer Triangle Batch Contract

## Status
**EXECUTED / DELIVERED / PASS**

## Scope
First geometry-side optimization contract derived from pinned upstream BMD mesh data and its `glBegin(GL_TRIANGLES)` emission. The new layer is an optimization abstraction, not a claim about original internal structures.

## Implemented
`NEW_ERA_IMPLEMENTATION/renderer/renderer_mesh_batch.h`

The contract converts recovered triangle records (`VertexIndex`, `NormalIndex`, `TexCoordIndex`) into GPU-oriented vertex/index arrays. It preserves the per-corner association used by the legacy renderer and emits one indexed triangle per source triangle.

For safety, only `Polygon == 3` is accepted. Other polygon forms are rejected and must use a separate compatibility path rather than being silently triangulated.

## Regression
`NEW_ERA_IMPLEMENTATION/renderer/test_renderer_mesh_batch.cpp`

Checks two source triangles, exact index order, exact corner associations, and rejection/cleanup for a quad polygon.

## Validation
C++17 compilation with `-Wall -Wextra -Wpedantic -pthread` followed by execution.

Observed:
`0D-3 renderer triangle batch contract: PASS`

Exit status: 0.

## Evidence basis
Pinned upstream `ZzzBMD.h` defines `Triangle_t` with four possible corner slots and `Mesh_t` with triangle arrays. Pinned `ZzzBMD.cpp` emits `GL_TRIANGLES` and reads each corner's vertex, normal and texture-coordinate indices before calling `glVertex3fv`.

## Boundary
No GPU API is introduced yet; no original draw-count/performance claim is made; non-triangle compatibility remains open; material-specific vertex generation (chrome/oil/wave/shadow) remains a later layer.
