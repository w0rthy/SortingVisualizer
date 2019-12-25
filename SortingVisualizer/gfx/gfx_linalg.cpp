#include "gfx_linalg.h"
#if GFX_ENABLED

mat4 rotMat(mat4 m, vec3 angle) {
	vec3 ang_rads = glm::radians(angle);

	mat4 mat = m;

	if(angle[1])
		mat = glm::rotate(mat, ang_rads[1], vec3(0.f, 1.f, 0.f));
	if(angle[0])
		mat = glm::rotate(mat, ang_rads[0], vec3(1.f, 0.f, 0.f));
	if(angle[2])
		mat = glm::rotate(mat, ang_rads[2], vec3(0.f, 0.f, 1.f));

	return mat;
}

#endif