# 0D-1 — Renderer Architecture Baseline

## Status
**EXECUTED / DELIVERED**

## Scope
Evidence-only renderer reconstruction from pinned upstream `wongddd/muonline@580472e`. No original runtime execution is claimed.

## Evidence

### 1. Window/context and frame presentation
`Source Client/source/ZzzScene.cpp` is the scene-level render coordinator. `NewRenderCharacterScene()` and `NewRenderLogInScene()` establish render passes with `BeginOpengl(...)`, render terrain/objects/characters/effects/UI, close with `EndOpengl()`. The loading-scene path explicitly calls `SwapBuffers(hDC)`.

### 2. OpenGL compatibility pipeline
`Source Client/source/ZzzOpenglUtil.cpp` defines viewport/projection helpers, camera matrix extraction, texture binding cache, depth/cull state, alpha-test and multiple legacy blend modes, immediate-mode helpers using `glBegin/glEnd`, and fixed-function projection/model-view setup in `BeginOpengl()`.

### 3. BMD/model rendering
`Source Client/source/ZzzBMD.cpp` implements `BMD::RenderMesh()` and `RenderMeshAlternative()`. Meshes are emitted as `GL_TRIANGLES`; texture selection includes skin, water, hair, chrome/metal/oil modes. Lighting is transformed per normal and render flags select texture/color/bright/dark/chrome/lightmap/nodepth/wave/shadow behaviors.

### 4. Texture subsystem
`Source Client/source/ZzzTexture.cpp` exposes `LoadBitmap()` / `DeleteBitmap()` over the bitmap/texture manager. Rendering binds the resulting OpenGL texture through `BindTexture()`.

### 5. Camera
`Source Client/source/CameraMove.cpp` contains waypoint/tour camera state, terrain-height following, distance-level interpolation, angle interpolation, pause/forward/backward controls, and OpenGL waypoint visualization.

### 6. Shader layer
`Source Client/source/ZzzShader.cpp` dynamically loads shader entry points with `wglGetProcAddress`. It defines two built-in GLSL 1.20-compatible programs (terrain and simple texture), links them, and falls back to fixed-function rendering if shader functions are unavailable. The shader programs multiply sampled texture color by vertex color; they do not replace the broader legacy path.

## Reconstructed renderer graph
Window/HDC -> OpenGL context -> BeginOpengl (viewport + projection + model-view + state) -> terrain -> objects -> characters/BMD -> effects/particles/leaves/boids/pets -> sprites -> bitmap/UI -> EndOpengl -> SwapBuffers.

Cross-cutting systems: CameraMove/camera matrices; texture cache/bitmap manager; legacy blend/depth/cull state; optional GLSL compatibility shaders.

## Conclusions
**CONFIRMED:** renderer is OpenGL-based, immediate-mode-heavy, stateful, and scene-coordinated rather than command-buffer/PSO based.

**CONFIRMED:** BMD mesh rendering is a central geometry path tightly coupled to texture selection, per-normal lighting, blend modes, and material flags.

**CONFIRMED:** camera/projection math and render-state management are global utility layers consumed by scene and model rendering.

**CONFIRMED:** a shader compatibility layer exists, but evidence does not justify calling the whole renderer shader-driven.

## Optimization implications for NEW-ERA
1. Preserve scene pass ordering as a compatibility contract.
2. Move BMD meshes to persistent GPU vertex/index buffers.
3. Replace immediate-mode triangles with batched draws.
4. Make material/blend/depth state explicit and sortable.
5. Replace global texture binding with resource handles/cache.
6. Keep camera math behaviorally compatible.
7. Isolate legacy material modes behind a material evaluator.
8. Treat GLSL support as a compatibility feature, not proof of a modern renderer.

## Boundary
No exact GPU draw counts, shader-use frequency, original performance measurements, future-renderer equivalence, or every undocumented visual quirk are claimed.

## Primary pinned sources
- `Source Client/source/ZzzScene.cpp`
- `Source Client/source/ZzzOpenglUtil.cpp`
- `Source Client/source/ZzzBMD.cpp`
- `Source Client/source/ZzzTexture.cpp`
- `Source Client/source/CameraMove.cpp`
- `Source Client/source/ZzzShader.cpp`
