#include "shapes.h"
#if GFX_ENABLED

namespace {
	struct _ : public Shape {
		_() {
			vertices.reserve(6);
			vertices.push_back(vec3(-0.5f, 0.5f, 0.f));
			vertices.push_back(vec3(0.5f, 0.5f, 0.f));
			vertices.push_back(vec3(-0.5f, -0.5f, 0.f));

			vertices.push_back(vec3(0.5f, 0.5f, 0.f));
			vertices.push_back(vec3(0.5f, -0.5f, 0.f));
			vertices.push_back(vec3(-0.5f, -0.5f, 0.f));
		}
	} _;
}

Shape* shape_square = &::_;

#endif