#pragma once
#include "gfx_common.h"
#if GFX_ENABLED

//Produces a rotation matrix from m for the rotation vector ang
mat4 rotMat(mat4 m, vec3 ang);

inline mat4 rotMat(vec3 ang) {
	return rotMat(mat4(1.f), ang);
}

inline vec3 rotate(vec3 vec, vec3 ang) {
	return (mat3)rotMat(ang) * vec;
}

#endif