#include "shapes.h"
#if GFX_ENABLED

#define STEPS 30

namespace {
	struct _ : public Shape {

		_() {
			vertices.reserve(STEPS * 3);
			constexpr float angstep = glm::radians(360.f / (float)STEPS);

			mat3 hor_mat = glm::rotate(mat4(1.f), angstep, vec3(0.f, 0.f, -1.f));

			vec3 controlVert = vec3(0.f, 0.5f, 0.f);
			for (int i = 0; i < STEPS; i++) {
				vertices += vec3();
				vertices += controlVert;
				controlVert = hor_mat * controlVert;
				vertices += controlVert;
			}
		}
	} _;
}

Shape* shape_circle = &::_;

#endif