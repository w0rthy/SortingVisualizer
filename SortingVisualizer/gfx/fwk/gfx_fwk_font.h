#pragma once
#include "../gfx_common.h"
#if GFX_ENABLED
using FontID = int;

FontID loadFont(string font, int sz);
void unloadFont(FontID font);
void unloadAllFonts();

struct FontChar {
	GLuint tex;
	int x, y, w, h;
	float xc, yc;
	float xgap;
};

FontChar& getFontChar(FontID font, char c);

float fontStringWidth(FontID font, string s);
int fontHeight(FontID font);

#endif