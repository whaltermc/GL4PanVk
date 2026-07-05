#ifndef GLES2PANVK_GL_TYPES_H
#define GLES2PANVK_GL_TYPES_H

/*
 * Minimal GL constant/type subset. Deliberately NOT the full GL spec --
 * scope this to what Minecraft/LWJGL/Sodium/Iris actually call. Expand only
 * when a real trace shows a missing symbol; do not pre-emptively implement
 * the whole spec (this is the mistake that makes these projects balloon).
 */

#include <stdint.h>

typedef unsigned int   GLenum;
typedef unsigned char  GLboolean;
typedef unsigned int   GLbitfield;
typedef signed char    GLbyte;
typedef short          GLshort;
typedef int            GLint;
typedef int            GLsizei;
typedef unsigned char  GLubyte;
typedef unsigned short GLushort;
typedef unsigned int   GLuint;
typedef float          GLfloat;
typedef float          GLclampf;
typedef double         GLdouble;
typedef double         GLclampd;
typedef void           GLvoid;
typedef int64_t        GLint64;
typedef uint64_t       GLuint64;
typedef intptr_t       GLintptr;
typedef intptr_t       GLsizeiptr;
typedef char           GLchar;

/* Buffer targets */
#define GL_ARRAY_BUFFER              0x8892
#define GL_ELEMENT_ARRAY_BUFFER      0x8893
#define GL_UNIFORM_BUFFER            0x8A11
#define GL_SHADER_STORAGE_BUFFER     0x90D2

/* Shader types */
#define GL_VERTEX_SHADER             0x8B31
#define GL_FRAGMENT_SHADER           0x8B30
#define GL_COMPUTE_SHADER            0x91B9

/* Draw modes */
#define GL_TRIANGLES                 0x0004
#define GL_TRIANGLE_STRIP            0x0005
#define GL_LINES                     0x0001

/* Texture targets */
#define GL_TEXTURE_2D                0x0DE1
#define GL_TEXTURE_2D_ARRAY          0x8C1A
#define GL_TEXTURE_CUBE_MAP          0x8513

/* Blend/depth state (subset) */
#define GL_BLEND                     0x0BE2
#define GL_DEPTH_TEST                0x0B71
#define GL_CULL_FACE                 0x0B44

/* Error codes */
#define GL_NO_ERROR                  0
#define GL_INVALID_ENUM              0x0500
#define GL_INVALID_VALUE             0x0501
#define GL_INVALID_OPERATION         0x0502
#define GL_OUT_OF_MEMORY             0x0505

#endif /* GLES2PANVK_GL_TYPES_H */
