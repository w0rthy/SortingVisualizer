#include "shapes.h"
#if GFX_ENABLED

#define STEPS 30

namespace {
	struct _ : public Shape {

		_() {
			vertices.reserve(STEPS * STEPS * 6);
			constexpr float angstep = glm::radians(360.f / (float)STEPS);

			mat3 hor_mat = glm::rotate(mat4(1.f), angstep, vec3(0.f, 1.f, 0.f));
			mat3 ver_mat = glm::rotate(mat4(1.f), angstep/2.f, vec3(1.f, 0.f, 0.f));

			vec3 controlVert = vec3(0.f, 0.5f, 0.f);
			
			vec3 control2Vert;
			vec3 control3Vert;
			for (int i = 0; i < STEPS; i++) {
				control2Vert = controlVert;
				control3Vert = ver_mat * control2Vert;
				for (int j = 0; j < STEPS; j++) {
					vertices += vertex(control2Vert,control2Vert);
					vertices += vertex(control3Vert,control3Vert);
					control3Vert = hor_mat * control3Vert;
					vertices += vertex(control3Vert,control3Vert);
					vertices += vertex(control2Vert,control2Vert);
					control2Vert = hor_mat * control2Vert;
					vertices += vertex(control3Vert,control3Vert);
					vertices += vertex(control2Vert,control2Vert);
				}
				controlVert = ver_mat * controlVert;
			}
		}
	} _;
}

Shape* shape_sphere = &::_;

#endif