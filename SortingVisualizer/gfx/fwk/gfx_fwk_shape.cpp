#include "gfx_fwk_shape.h"
#if GFX_ENABLED

vector<Shape*> shapes;

void bindAllShapes() {
	for (auto& s : shapes)
		s->build();
}

#endif