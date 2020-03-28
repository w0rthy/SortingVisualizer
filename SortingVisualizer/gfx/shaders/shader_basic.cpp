#include "shaders.h"
#if GFX_ENABLED

namespace {
#include "shader_basic.vert"
#include "shader_basic.frag"
}
Shader shader_basic(__FILE__,::vert, ::frag);

#endif