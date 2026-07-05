#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

namespace g2p {

// Desktop GLSL versions Minecraft/Sodium/Iris shaderpacks emit (#version 150
// through 460-ish, plus GL_ARB_* pragmas) need light preprocessing before
// they're valid input to a GLSL->SPIR-V compiler expecting ES-flavored or
// strict-core GLSL. Real implementation needs:
//   - glslang (Khronos reference compiler) for GLSL/ESSL -> SPIR-V
//   - optionally SPIRV-Cross for validating/round-tripping if you need to
//     patch binding numbers post-hoc (UBO/sampler binding slots need to be
//     assigned deterministically -- GL has no concept of descriptor sets)
struct SpirvModule {
    std::vector<uint32_t> words;
    bool valid = false;
};

class ShaderTranslator {
public:
    // Cheap FNV-1a hash of source text -- used as the cache key so repeated
    // shaderpack reloads (Iris does this on every settings change) don't
    // recompile identical source.
    static uint64_t hash_source(const std::string& src) {
        uint64_t h = 1469598103934665603ull;
        for (unsigned char c : src) {
            h ^= c;
            h *= 1099511628211ull;
        }
        return h;
    }

    const SpirvModule& translate(uint64_t hash, const std::string& glsl_source,
                                  bool is_vertex_stage) {
        auto it = cache_.find(hash);
        if (it != cache_.end()) return it->second;

        SpirvModule mod = compile(glsl_source, is_vertex_stage);
        auto [inserted, _] = cache_.emplace(hash, std::move(mod));
        return inserted->second;
    }

private:
    SpirvModule compile(const std::string& glsl_source, bool is_vertex_stage) {
        // TODO: real implementation --
        //   glslang::TShader shader(is_vertex_stage ? EShLangVertex : EShLangFragment);
        //   shader.setStrings(&src_ptr, 1);
        //   shader.parse(&DefaultTBuiltInResource, ...);
        //   glslang::TProgram program; program.addShader(&shader); program.link(...);
        //   glslang::GlslangToSpv(*program.getIntermediate(stage), spirv_words);
        //
        // Also needs deterministic binding assignment: GL uniform/sampler
        // names don't carry descriptor-set/binding info, so you must assign
        // bindings yourself (e.g. by reflection over the parsed AST) and
        // keep that mapping alongside the SPIR-V for descriptor_pool to use.
        (void)glsl_source;
        (void)is_vertex_stage;
        SpirvModule mod;
        mod.valid = false; // flip true once compile() actually calls glslang
        return mod;
    }

    std::unordered_map<uint64_t, SpirvModule> cache_;
};

} // namespace g2p
