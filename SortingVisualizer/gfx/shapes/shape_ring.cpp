#include "shapes.h"
#if GFX_ENABLED

#define STEPS 30

namespace {
	struct _ : public Shape {

		void calcNormal(int triNum, vec3& v1, vec3& v2, vec3& v3) {
			if (triNum % 2) {
				normals += v1 * vec3(2.f, 2.f, 0.f);
				normals += v2 * vec3(2.f, 2.f, 0.f);
				normals += v3 * vec3(2.f, 2.f, 0.f);
			}
			else {
				normals += v1 * vec3(-2.f, -2.f, 0.f);
				normals += v2 * vec3(-2.f, -2.f, 0.f);
				normals += v3 * vec3(-2.f, -2.f, 0.f);
			}
		}

		_() {
			vertices.reserve(STEPS * 3 * 2 * 2);
			constexpr float angstep = glm::radians(360.f / (float)STEPS);

			mat3 hor_mat = glm::rotate(mat4(1.f), angstep, vec3(0.f, 0.f, -1.f));

			vec3 controlVert = vec3(0.f, 0.5f, 0.5f);
			vec3 controlVert2 = controlVert;
			for (int i = 0; i < STEPS; i++) {
				controlVert2 = hor_mat * controlVert;
				vertices += controlVert;
				controlVert[2] -= 1.f;
				vertices += controlVert;
				vertices += controlVert2;
				vertices.flipLastTri();

				vertices += controlVert2;
				vertices += controlVert;
				vertices += controlVert2 - vec3(0.f, 0.f, 1.f);
				vertices.flipLastTri();
				controlVert = controlVert2;
			}
		}
	} _;
}

Shape* shape_ring = &::_;

#endif