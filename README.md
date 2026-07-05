# gles2panvk

A GLES-over-Vulkan shim that deliberately targets Mesa's **panvk** ICD on Arm
Mali GPUs — the "ANGLE, but pinned to panvk" project. Built to sit under
PojavLauncher/Amethyst the same way MobileGlues or ANGLE do, but skipping the
GLES-driver hop entirely and going straight to Vulkan.

```
Minecraft (LWJGL, desktop GL calls)
   -> libGLESv2.so / libEGL.so   <-- this project
        -> internal state tracker (no GL state lives in the driver)
        -> GLSL/GLSL ES -> SPIR-V (cached by shader hash)
        -> pipeline cache (VkPipeline keyed on state hash)
        -> Vulkan command buffers
           -> panvk ICD (forced selection, not left to loader defaults)
```

## Why this is a separate project from MobileGlues

MobileGlues translates desktop GL -> GLES 3.x and then hands off to *whatever*
GLES driver is present (vendor blob, Panfrost, or ANGLE-over-Vulkan). It never
talks to Vulkan itself. This project removes that indirection: EGL/GLES
entry points are implemented directly against the Vulkan API, with panvk
selected explicitly via `VK_ICD_FILENAMES` / `VK_DRIVER_FILES` so behavior
doesn't depend on loader ICD-enumeration order.

## Directory layout

- `include/gles2panvk/` — public GLES/EGL-compatible headers the game links against
- `src/entrypoints.c` — exported `gl*`/`egl*` symbols, dispatches to the active context
- `src/context.{hpp,cpp}` — per-context GL state (bound buffers, textures, FBOs, pipeline state)
- `src/shader_translator.{hpp,cpp}` — GLSL(ES) -> SPIR-V, cached by source hash
- `src/pipeline_cache.{hpp,cpp}` — VkPipeline cache keyed by (shader hash, vertex layout, blend/depth state)
- `src/vk_backend.{hpp,cpp}` — instance/device creation, explicit panvk ICD selection, queue/command submission
- `src/descriptor_pool.{hpp,cpp}` — descriptor set allocation/reuse for textures & UBOs
- `cmake/android-panvk.toolchain.cmake` — NDK toolchain glue

## Build (once you have the NDK + Vulkan headers + shaderc available)

```bash
cmake -B build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
  -DCMAKE_ANDROID_NDK=$ANDROID_NDK \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-26 \
  -C cmake/android-panvk.toolchain.cmake
cmake --build build -j$(nproc)
```

Outputs `libGLESv2.so` + `libEGL.so`, loaded by PojavLauncher/Amethyst the
same way a custom Turnip/adrenotools driver plugin is loaded — just pointed
at panvk instead.

## What's stubbed vs. real here

Real / structurally complete:
- Entry point table and dispatch mechanism
- State tracker data model (bound state, dirty-flag tracking)
- Pipeline cache key design and lookup path
- Vulkan device init with explicit panvk ICD pinning

Stubbed (needs the actual libs wired in before this runs anything):
- `shader_translator.cpp`: GLSL ES parsing needs glslang; SPIR-V cross-checks need SPIRV-Cross or SPIRV-Tools
- `vk_backend.cpp`: swapchain/present logic needs the platform windowing glue (ANativeWindow on Android)
- No compute shader path yet (Minecraft barely needs it, low priority)

## Priority order if you pick this up

1. Wire real shaderc/glslang into `shader_translator.cpp` — nothing else can
   run without this
2. Get triangle-through-panvk working end-to-end (bypass Minecraft, use a
   trivial test harness) before touching Minecraft's actual GL call patterns
3. Only then implement the ~30 GL entry points Sodium/vanilla actually call
   (see `docs/gl-call-inventory.md` — start this list from a MobileGlues
   trace, don't guess)
4. Pipeline cache correctness before optimizing it — a wrong cache hit is a
   silent rendering bug
