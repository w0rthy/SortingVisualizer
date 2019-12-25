#include "shaders.h"
#if GFX_ENABLED

namespace {
#include "shader_font_3d.vert"
#include "shader_font_3d.frag"
}
Shader shader_font_3d(::vert, ::frag);

#endif