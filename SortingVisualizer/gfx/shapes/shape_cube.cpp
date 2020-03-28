#include "shapes.h"
#if GFX_ENABLED

namespace {
	struct _ : public Shape {
		_() {
			//Front Face
			vertexVector square;
			square.push_back(vec3(-0.5f, 0.5f, -0.5f));
			square.push_back(vec3(0.5f, 0.5f, -0.5f));
			square.push_back(vec3(-0.5f, -0.5f, -0.5f));

			square.push_back(vec3(0.5f, 0.5f, -0.5f));
			square.push_back(vec3(0.5f, -0.5f, -0.5f));
			square.push_back(vec3(-0.5f, -0.5f, -0.5f));
			//

			vertices.reserve(6*6);
			for(int i = 0; i < 4; i++)
				vertices += (mat4)glm::rotate(mat4(1.f), glm::radians(90.f*i), vec3(1.f,0.f,0.f)) * square;
			vertices += (mat4)glm::rotate(mat4(1.f), glm::radians(90.f), vec3(0.f, 1.f, 0.f)) * square;
			vertices += (mat4)glm::rotate(mat4(1.f), glm::radians(-90.f), vec3(0.f, 1.f, 0.f)) * square;
		}
	} _;
}

Shape* shape_cube = &::_;


#endif