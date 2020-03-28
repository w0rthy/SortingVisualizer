#include "shapes.h"
#if GFX_ENABLED

#define STEPS 50

namespace {
	struct _ : public Shape {

		vec3 offset = vec3(0.f, 0.5f, 0.f); //Controls height of object
		float normalAdd = 0.125f/(offset[1]); //Used for calculating normals of side vertices

		vec3 normalizeConeVert(vec3 pos) {
			pos[1] = 0.f;
			pos = glm::normalize(pos);
			pos[1] = normalAdd;
			return pos;
		}

		vec3 normalizeConeVert2(vec3 pos1, vec3 pos2) {
			return normalizeConeVert(pos1) + normalizeConeVert(pos2);
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
				vertices += vertex(tmp,normalizeConeVert(tmp));
				vertices += vertex(offset,normalizeConeVert2(tmp,tmp2));
				vertices += vertex(tmp2,normalizeConeVert(tmp2));

				tmp = tmp2;
			}
		}
	} _;
}

Shape* shape_cone = &::_;

#endif