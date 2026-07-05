/*
 * Exported symbols. LWJGL resolves these by name (dlsym against
 * libGLESv2.so), so signatures must match the real GL ABI exactly even
 * though the implementation underneath is entirely our own.
 *
 * Deliberately small: only the calls a MobileGlues trace / Sodium source
 * read actually shows Minecraft issuing. Add entries here only when a real
 * trace demands it -- see docs/gl-call-inventory.md.
 */

#include "gles2panvk/gl_types.h"

/* Opaque handle to the thread-local active context; real definition lives
 * in context.cpp, exposed here via a tiny C-linkage accessor so this file
 * doesn't need to be C++. */
void* g2p_get_current_context(void);

#define CTX() g2p_get_current_context()

GLuint glCreateProgram(void) {
    /* TODO: dispatch to Context::create_program() */
    (void)CTX();
    return 0;
}

void glUseProgram(GLuint program) {
    /* TODO: Context::bound_program = program; mark raster_dirty */
    (void)program;
}

void glBindBuffer(GLenum target, GLuint buffer) {
    /* TODO: route into Context::bound_array_buffer /
     * bound_element_buffer depending on target */
    (void)target;
    (void)buffer;
}

void glBufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage) {
    /* TODO: allocate/update the backing VkBuffer via vk_backend, staged
     * through a host-visible upload buffer since Mali UMA means you *can*
     * skip a staging copy in many cases -- worth a fast path once the
     * naive version works. */
    (void)target; (void)size; (void)data; (void)usage;
}

void glVertexAttribPointer(GLuint index, GLint size, GLenum type,
                            GLboolean normalized, GLsizei stride,
                            const void* pointer) {
    /* TODO: write into Context::vertex_attribs[index], mark dirty so the
     * next draw's pipeline_key() picks up the new layout */
    (void)index; (void)size; (void)type; (void)normalized; (void)stride; (void)pointer;
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count) {
    /*
     * TODO real dispatch:
     *   1. ctx->raster.draw_mode = mode;
     *   2. uint64_t key = ctx->current_pipeline_key();
     *   3. VkPipeline p = pipeline_cache.lookup(key);
     *      if (!p) { p = build_pipeline(ctx, key); pipeline_cache.insert(key, p); }
     *   4. record vkCmdBindPipeline / vkCmdBindVertexBuffers / vkCmdDraw
     *      into the *current frame's* command buffer -- do NOT submit here,
     *      batch until glfwSwapBuffers()/eglSwapBuffers() equivalent.
     */
    (void)mode; (void)first; (void)count;
}

void glDrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices) {
    (void)mode; (void)count; (void)type; (void)indices;
}

GLenum glGetError(void) {
    return GL_NO_ERROR; /* TODO: surface real tracked error state */
}
