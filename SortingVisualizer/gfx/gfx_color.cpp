#include "gfx_color.h"
#if GFX_ENABLED

vec4 colHSV(float h, float s, float v, float alpha) {
	h = fmodf(h, 1.f)*360.f;
	float c = s * v;
	float x = c * (1 - abs(fmodf((h / 60.f),2.f) - 1.f));
	float m = v - c;
	float r = 0, g = 0, b = 0;
	switch ((int)h/60) {
	case 0:
		r = c;
		g = x;
		break;
	case 1:
		r = x;
		g = c;
		break;
	case 2:
		g = c;
		b = x;
		break;
	case 3:
		g = x;
		b = c;
		break;
	case 4:
		r = x;
		b = c;
		break;
	case 5:
		r = c;
		b = x;
	}
	return vec4(r + m, g + m, b + m, alpha);
}

#endif