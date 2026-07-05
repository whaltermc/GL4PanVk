# Building the real GL call inventory

Don't guess which GL entry points to implement -- pull the list from a real
trace, otherwise you'll burn time on calls Minecraft never issues and miss
ones it does.

## How to get the trace

1. Run Minecraft under PojavLauncher/Amethyst with MobileGlues active and its
   debug logging cranked up (`ignoreError` + verbose mode in MobileGlues
   settings) -- its logs already show every GL call it receives from LWJGL,
   since it has to implement the same surface you're targeting.
2. Alternatively, use `apitrace` (works on Android with some setup) against
   a desktop-GL build of the game if you want a byte-exact call/argument
   trace rather than a log-derived list.
3. Cross-reference against Sodium's and vanilla's renderer source for which
   GL 3.2-core-ish subset they assume (both avoid legacy immediate-mode and
   most exotic state, which is why MobileGlues can get away with GLES 3.2 as
   a floor).

## Known must-haves from the MobileGlues precedent

Since MobileGlues already proved these are load-bearing for Minecraft +
Sodium + Iris:
- Buffer objects: `glGenBuffers/glBindBuffer/glBufferData/glBufferSubData`
- VAOs: `glGenVertexArrays/glBindVertexArray/glVertexAttribPointer/glEnableVertexAttribArray`
- Shaders/programs: `glCreateShader/glShaderSource/glCompileShader/glCreateProgram/glAttachShader/glLinkProgram/glUseProgram`
- Uniforms: `glGetUniformLocation/glUniform{1,2,3,4}{f,i}v`, UBOs for Iris (`glBindBufferBase` w/ `GL_UNIFORM_BUFFER`)
- Textures: `glGenTextures/glBindTexture/glTexImage2D/glTexSubImage2D/glGenerateMipmap`
- Framebuffers: `glGenFramebuffers/glBindFramebuffer/glFramebufferTexture2D` (Iris shaderpacks use multiple render targets -- budget for `glDrawBuffers`)
- Draw calls: `glDrawArrays/glDrawElements`, `glDrawElementsInstanced` if any mod uses instancing
- Multi-draw indirect shows up as an optional path in MobileGlues
  (`GL_EXT_multi_draw_indirect`) -- Sodium probes for it but has a fallback,
  so it's a "nice to have, not launch-blocking" item.

Compute shaders: skip initially. MobileGlues gates this behind an explicit
toggle because so little Minecraft content needs it -- correctly deprioritized
in your build too.
