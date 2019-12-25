#include "shaders.h"
#if GFX_ENABLED

namespace {
#include "shader_font_2d.vert"
#include "shader_font_2d.frag"
}
Shader shader_font_2d(::vert, ::frag);

#endif