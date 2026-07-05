# Extra cache entries layered on top of $NDK/build/cmake/android.toolchain.cmake
# via `cmake -C cmake/android-panvk.toolchain.cmake ...`

set(ANDROID_STL "c++_shared" CACHE STRING "")
set(ANDROID_ARM_NEON ON CACHE BOOL "")

# Mali devices you're targeting are all v9 (Bifrost)+ or v10+ (Valhall/CSF);
# no reason to pay codegen cost for armv7/Midgard unless you specifically
# need to support very old hardware.
set(ANDROID_ABI "arm64-v8a" CACHE STRING "")
set(ANDROID_PLATFORM "android-26" CACHE STRING "")

# Point these at wherever you've built/vendored glslang + Vulkan headers
# for Android before running the top-level cmake configure:
#   set(glslang_DIR ".../glslang/build/install/lib/cmake/glslang" CACHE PATH "")
#   set(Vulkan_INCLUDE_DIR ".../Vulkan-Headers/include" CACHE PATH "")
