cmake_minimum_required(VERSION 3.22)
project(gles2panvk C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# --- External deps you need to provide paths for ---
# find_package(Vulkan REQUIRED)          # or point at the NDK's Vulkan headers directly
# find_package(glslang CONFIG REQUIRED)  # for shader_translator.cpp
# find_package(SPIRV-Tools CONFIG)       # optional, for validation

add_library(gles2panvk SHARED
    src/entrypoints.c
    src/context.cpp
    src/vk_backend.cpp
    # src/shader_translator.cpp   # currently header-only stub; move logic
    #                             # into a .cpp once glslang is linked
)

target_include_directories(gles2panvk PUBLIC include)

target_compile_options(gles2panvk PRIVATE
    -Wall -Wextra
    -O2 -flto            # release perf; drop -flto for faster edit/build cycles
)

# Uncomment once Vulkan/glslang are found above:
# target_link_libraries(gles2panvk PRIVATE Vulkan::Vulkan glslang SPIRV)

# Android-specific: name the two output libs the launcher's driver-plugin
# loader expects (mirrors how Turnip/adrenotools packages are named).
set_target_properties(gles2panvk PROPERTIES
    OUTPUT_NAME "GLESv2"
)
