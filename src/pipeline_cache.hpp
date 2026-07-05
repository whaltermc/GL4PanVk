#pragma once
#include <cstdint>
#include <unordered_map>
#include <vulkan/vulkan.h>

namespace g2p {

// Building a VkPipeline is expensive (shader compilation to hardware ISA,
// render-pass compatibility checks, etc). Minecraft's per-frame draw count
// makes a naive "build a pipeline per draw call" approach unusable -- this
// is the #1 thing that makes early GL-on-Vulkan shims feel worse than
// native GL despite Vulkan being "faster" on paper.
class PipelineCache {
public:
    // Returns an existing pipeline for `key`, or VK_NULL_HANDLE if the
    // caller needs to build one and call `insert()`.
    VkPipeline lookup(uint64_t key) const {
        auto it = cache_.find(key);
        return it != cache_.end() ? it->second : nullptr;
    }

    void insert(uint64_t key, VkPipeline pipeline) {
        cache_[key] = pipeline;
    }

    size_t size() const { return cache_.size(); }

    // Called on shader hot-reload (e.g. shaderpack switch in Iris) --
    // invalidates only pipelines whose key folded in the affected program,
    // not the whole cache. TODO: track key->program_id reverse mapping
    // to make this precise rather than a blunt clear().
    void invalidate_all() { cache_.clear(); }

private:
    std::unordered_map<uint64_t, VkPipeline> cache_;
    // Persisting this to disk (VkPipelineCache has a native serialize API)
    // between runs is a real, easy win -- MobileGlues does the equivalent
    // for its GLSL cache. Wire that up once basic caching works.
};

} // namespace g2p
