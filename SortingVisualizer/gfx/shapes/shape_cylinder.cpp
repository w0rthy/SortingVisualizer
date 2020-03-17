#include "shapes.h"
#if GFX_ENABLED

#define STEPS 30

namespace {
	struct _ : public Shape {

		vec3 offset = vec3(0.f, 0.5f, 0.f); //Controls height of object

		void calcNormal(int triNum, vec3& v1, vec3& v2, vec3& v3) {
			vec3 V1 = vec3(v1[0], 0.f, v1[2]);
			vec3 V2 = vec3(v2[0], 0.f, v2[2]);
			vec3 V3 = vec3(v3[0], 0.f, v3[2]);
			if(triNum%4>1){
				normals += glm::normalize(V1);
				normals += glm::normalize(V2);
				normals += glm::normalize(V3);
			}
			else
				defaultNormal(v1, v2, v3);
		}

		_() {
			vertices.reserve(STEPS * 12);
			constexpr float angstep = glm::radians(360.f / (float)STEPS);

			mat3 hor_mat = glm::rotate(mat4(1.f), angstep, vec3(0.f, -1.f, 0.f));

			vec3 controlVert = vec3(0.5f, 0.f, 0.f);

			vec3 tmp;
			
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
				vertices += tmp - offset;
				vertices += tmp + offset;
				vertices += controlVert-offset;
				//
				vertices += tmp + offset;
				vertices += controlVert + offset;
				vertices += controlVert - offset;
			}
		}
	} _;
}

Shape* shape_cylinder = &::_;

#endif