cmake_minimum_required(VERSION 3.22)
project(gles2panvk C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# --- External deps ---
find_package(glslang CONFIG REQUIRED)

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
                            -O2 -flto
                            )

                            target_link_libraries(gles2panvk PRIVATE
                                glslang::glslang
                                    glslang::SPIRV
                                        glslang::glslang-default-resource-limits
                                            vulkan
                                            )

                                            # Android-specific: name the output lib the launcher's driver-plugin loader
                                            # expects (mirrors how Turnip/adrenotools packages are named).
                                            set_target_properties(gles2panvk PROPERTIES
                                                OUTPUT_NAME "GLESv2"
                                                )