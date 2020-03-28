#include "shapes.h"
#if GFX_ENABLED

#define STEPS 30

namespace {
	struct _ : public Shape {

		vec3 offset = vec3(0.f, 0.5f, 0.f); //Controls height of object
		vec3 normalVec = vec3(1.f, 0.f, 1.f); //Multiplying by this gives the normal for the cylinder

		_() {
			vertices.reserve(STEPS * 12);
			constexpr float angstep = glm::radians(360.f / (float)STEPS);

			mat3 hor_mat = glm::rotate(mat4(1.f), angstep, vec3(0.f, -1.f, 0.f));

			vec3 controlVert = vec3(0.5f, 0.f, 0.f);

			vec3 tmp, tmp2;
			
			for (int i = 0; i < STEPS; i++) {
				tmp = controlVert;
				//Bottom Circle vertices
				vertices += -offset;
				vertices += tmp - offset;
				controlVert = hor_mat * controlVert;
				vertices += controlVert - offset;
				//Top Circle vertices
				vertices += offset;
				vertices += controlVert + offset;
				vertices += tmp + offset;
				//Cylinder Vertices
				tmp2 = tmp - offset;
				vertices += vertex(tmp2, tmp2*normalVec);
				tmp2 = tmp + offset;
				vertices += vertex(tmp2, tmp2*normalVec);
				tmp2 = controlVert - offset;
				vertices += vertex(tmp2, tmp2*normalVec);
				//
				tmp2 = tmp + offset;
				vertices += vertex(tmp2, tmp2*normalVec);
				tmp2 = controlVert + offset;
				vertices += vertex(tmp2, tmp2*normalVec);
				tmp2 = controlVert - offset;
				vertices += vertex(tmp2, tmp2*normalVec);
			}
		}
	} _;
}

Shape* shape_cylinder = &::_;

#endif