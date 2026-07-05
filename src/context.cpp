#include "context.hpp"

namespace g2p {

Context::Context() {
    bound_textures.fill(0);
}

uint64_t Context::current_pipeline_key() const {
    uint64_t h = raster.hash();

    // Fold in the bound program -- different shaders need different
    // VkPipelines even with identical raster state.
    h ^= (uint64_t)bound_program * 0x9E3779B97F4A7C15ull;

    // Fold in the enabled vertex attrib layout, since VkPipeline bakes in
    // VkVertexInputBindingDescription/AttributeDescription at creation time
    // (this is the main reason naive GL->VK shims end up recompiling
    // pipelines every frame if they don't account for it).
    for (size_t i = 0; i < vertex_attribs.size(); ++i) {
        const auto& a = vertex_attribs[i];
        if (!a.enabled) continue;
        uint64_t attr_bits =
            (uint64_t)a.type << 32 |
            (uint64_t)a.components << 16 |
            (uint64_t)a.stride;
        h ^= (attr_bits + i) * 0xC2B2AE3D27D4EB4Full;
    }

    return h;
}

} namespace {
    thread_local g2p::Context* tls_current_context = nullptr;
}
extern "C" void* g2p_get_current_context() {
    if (!tls_current_context) {
        tls_current_context = new g2p::Context();
    }
    return tls_current_context;
}