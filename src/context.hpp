#pragma once
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <array>
#include "gles2panvk/gl_types.h"

namespace g2p {

// Vulkan has no global state -- every GL call that "sets state" here just
// writes into this struct and marks a dirty bit. The actual VkPipeline is
// only built (or fetched from cache) at draw time, from a hash of the
// currently-dirty state. This is the single most important design decision
// in the whole shim: never touch Vulkan objects on a per-GL-call basis.

struct BufferObject {
    uint32_t gl_id = 0;
    GLenum   target = 0;
    size_t   size = 0;
    // VkBuffer + VkDeviceMemory handles live in vk_backend's object table,
    // referenced by gl_id here to keep this header Vulkan-header-free.
};

struct TextureObject {
    uint32_t gl_id = 0;
    GLenum   target = GL_TEXTURE_2D;
    int32_t  width = 0, height = 0;
    GLenum   internal_format = 0;
    bool     mips_dirty = true;
};

struct ShaderProgram {
    uint32_t gl_id = 0;
    uint64_t vertex_src_hash = 0;
    uint64_t fragment_src_hash = 0;
    // Filled in once shader_translator produces SPIR-V for each hash.
    bool     linked = false;
};

// Fixed-function-ish state that maps onto VkPipeline's
// rasterization/blend/depth-stencil create-info structs.
struct RasterState {
    bool    blend_enable = false;
    bool    depth_test_enable = true;
    bool    depth_write_enable = true;
    bool    cull_enable = false;
    GLenum  cull_face = 0;
    GLenum  draw_mode = GL_TRIANGLES;

    uint64_t hash() const {
        uint64_t h = 1469598103934665603ull; // FNV offset basis
        auto mix = [&](uint64_t v){ h ^= v; h *= 1099511628211ull; };
        mix(blend_enable);
        mix(depth_test_enable);
        mix(depth_write_enable);
        mix(cull_enable);
        mix(cull_face);
        mix(draw_mode);
        return h;
    }
};

struct VertexAttribBinding {
    uint32_t buffer_gl_id = 0;
    GLenum   type = 0;
    int      components = 0;
    bool     normalized = false;
    size_t   stride = 0;
    size_t   offset = 0;
    bool     enabled = false;
};

class Context {
public:
    Context();

    // --- Object tables (GL ids are opaque uint32_t handles into these) ---
    std::unordered_map<uint32_t, BufferObject>  buffers;
    std::unordered_map<uint32_t, TextureObject> textures;
    std::unordered_map<uint32_t, ShaderProgram>  programs;

    // --- Bound state ---
    uint32_t bound_array_buffer = 0;
    uint32_t bound_element_buffer = 0;
    uint32_t bound_program = 0;
    std::array<uint32_t, 32> bound_textures{}; // per texture unit
    std::array<VertexAttribBinding, 16> vertex_attribs{};

    RasterState raster;
    bool raster_dirty = true;

    uint32_t next_gl_id = 1;
    uint32_t alloc_id() { return next_gl_id++; }

    // Called at draw time: hashes current state, looks up (or triggers
    // creation of) the matching VkPipeline via pipeline_cache. Does NOT
    // touch Vulkan directly -- returns a cache key for vk_backend to use.
    uint64_t current_pipeline_key() const;
};

} // namespace g2p
