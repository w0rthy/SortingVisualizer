#pragma once
#include "gfx_common.h"
#if GFX_ENABLED

//Returns a color given HSV where H is from 0.0 to 1.0
vec4 colHSV(float h, float s, float v, float alpha = 1.f);

#endif