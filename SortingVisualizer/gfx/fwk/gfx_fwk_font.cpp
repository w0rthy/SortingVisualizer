#include "gfx_fwk_font.h"
#if GFX_ENABLED
#include "ft2build.h"
#include FT_FREETYPE_H

#define MAX_FONTS 10
#define CHARS_PER_FONT ((int)(1<<(sizeof(char)*8)))

bool initialized = false;
FT_Library ftLib;

//Keeps track of which spaces in the fonts array are available (stores the size/height of the font)
int fontsUsageArr[MAX_FONTS];

FontChar fontsArr[MAX_FONTS*CHARS_PER_FONT];

FontID loadFont(string font, int sz) {
	if (!initialized) {
		FT_Init_FreeType(&ftLib);

		for (int i = 0; i < MAX_FONTS; i++)
			fontsUsageArr[i] = 0;

		initialized = true;
	}

	if (sz <= 0)
		return -1;

	//Find an available font space
	int fontNum = -1;
	for (int i = 0; i < MAX_FONTS; i++) {
		if (!fontsUsageArr[i]) {
			fontsUsageArr[i] = sz;
			fontNum = i;
			break;
		}
	}
	if (fontNum < 0)
		return -1;

	//Initialize the face
	FT_Face f;
	if (FT_New_Face(ftLib, font, 0, &f)) {
		printf("Failed to load font '%s'\n", font.c_str());
		return -1;
	}

	FT_Set_Pixel_Sizes(f, 0, sz);
	
	//Generate the glyphs
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	int offset = fontNum * CHARS_PER_FONT;

	for (int i = 0; i < CHARS_PER_FONT; i++) {
		auto& fc = fontsArr[offset + i];
		
		if (FT_Load_Char(f, i, FT_LOAD_RENDER)) {
			printf("Failed to load character number %d\n", i);
		}

		auto& g = f->glyph;

		fc.x = g->bitmap_left;
		fc.y = g->bitmap_top;
		fc.w = g->bitmap.width;
		fc.h = g->bitmap.rows;
		fc.xc = (float)fc.x + (float)fc.w / 2.f;
		fc.yc = (float)fc.y - (float)fc.h / 2.f;
		fc.xgap = (float)(g->advance.x)/64.f;

		glGenTextures(1, &fc.tex);
		glBindTexture(GL_TEXTURE_2D, fc.tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fc.w, fc.h, 0, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		//
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//

	}
	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(f);

	return fontNum;
}

void unloadFont(FontID font) {
	if (!fontsUsageArr[font])
		return;

	int offset = font * CHARS_PER_FONT;
	int max = offset + CHARS_PER_FONT;

	for (int i = offset; i < max; i++) {
		glDeleteTextures(1, &fontsArr[i].tex);
	}

	fontsUsageArr[font] = 0;
}

void unloadAllFonts() {
	for (int i = 0; i < MAX_FONTS; i++)
		unloadFont(i);
}

FontChar& getFontChar(FontID font, char c) {
	return fontsArr[font * CHARS_PER_FONT + (int)c];
}

float fontStringWidth(FontID font, string s) {
	float sum = 0;

	int offset = font * CHARS_PER_FONT;

	for (int i = 0; i < s.length(); i++) {
		auto& fc = fontsArr[offset + s[i]];

		if (i == 0)
			sum -= fc.x;

		if (i == s.length() - 1) {
			sum -= fc.xgap;
			sum += fc.x + (float)fc.w;
		}
		
		sum += fc.xgap;
	}
	return sum;
}

int fontHeight(FontID font) {
	return fontsUsageArr[font];
}

#endif