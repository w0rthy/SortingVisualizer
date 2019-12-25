#include "gfx_fwk_shader.h"
#if GFX_ENABLED
#include "../shaders/shaders.h"

vector<Shader*> shaders;

Shader* currentShader = nullptr;

void compileAllShaders() {
	for (auto& s : shaders)
		s->compile();
	shader_basic(); //Activate a default shader for use
}

#endif