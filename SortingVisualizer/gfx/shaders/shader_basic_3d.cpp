#include "shaders.h"
#if GFX_ENABLED

namespace {
#include "shader_basic_3d.vert"
#include "shader_basic_3d.frag"
}
Shader shader_basic_3d(__FILE__,::vert, ::frag);

#endif