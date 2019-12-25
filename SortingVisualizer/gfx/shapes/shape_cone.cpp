#include "shapes.h"
#if GFX_ENABLED

#define STEPS 50

namespace {
	struct _ : public Shape {

		vec3 offset = vec3(0.f, 0.5f, 0.f); //Controls height of object
		vec3 normalAdd = vec3(0.f, 0.5f/(offset[1]*2.f)*0.5f, 0.f); //Used for calculating normals of side vertices

		void calcNormal(int triNum, vec3& v1, vec3& v2, vec3& v3) {
			vec3 V1 = vec3(v1[0], 0.f, v1[2]);
			vec3 V3 = vec3(v3[0], 0.f, v3[2]);
			if (triNum % 2) {
				normals += glm::normalize(V1 + normalAdd);
				normals += glm::normalize((V1 + V3) *0.5f + normalAdd);
				normals += glm::normalize(V3 + normalAdd);
			}
			else
				defaultNormal(v1, v2, v3);
		}

		_() {
			vertices.reserve(STEPS * 6);
			constexpr float angstep = glm::radians(360.f / (float)STEPS);

			mat3 hor_mat = glm::rotate(mat4(1.f), angstep, vec3(0.f, -1.f, 0.f));

			vec3 controlVert = vec3(0.5f, 0.f, 0.f);
			
			vec3 tmp, tmp2;
			tmp = controlVert - offset;
			for (int i = 0; i < STEPS; i++) {
				//Circle vertices
				vertices += -offset;
				vertices += tmp;
				controlVert = hor_mat * controlVert;
				tmp2 = controlVert - offset;
				vertices += tmp2;
				//Cone vertices
				vertices += tmp;
				vertices += offset;
				vertices += tmp2;

				tmp = tmp2;
			}
		}
	} _;
}

Shape* shape_cone = &::_;

#endif