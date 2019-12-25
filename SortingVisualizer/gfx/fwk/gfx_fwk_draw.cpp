#include "gfx_fwk_draw.h"
#if GFX_ENABLED
#include "gfx_fwk_shader.h"
#include "../shaders/shaders.h"
#include "../shapes/shapes.h"

mat4 calcModelMat(vec3 pos, vec3 size, vec3 angle) {
	mat4 mdl = mat4(1.f);
	mdl = glm::translate(mdl, pos); //Pos
	mdl = rotMat(mdl, angle);
	mdl = glm::scale(mdl, size); //Size
	return mdl;
}

void drawShape(Shape* shape, mat4 mdl, vec4 color) {
	glUniformMat4f(uniformLocation("model"), mdl);
	glUniformVec4f(uniformLocation("color"), color);

	glBindVertexArray(shape->vao);

	glDrawArrays(GL_TRIANGLES, 0, shape->vertices.size());

	glBindVertexArray(0);
}

void drawFontString(FontID font, string s, Shader* shader, vec2 reloff, vec3 pos, vec3 size, vec3 angle, vec4 color) {
	if (s.length() < 1)
		return;

	Shader* tmpShader = currentShader;
	(*shader)();

	//Correct size for 1080p
	constexpr float scale = 2.f / 1080.f;
	size *= scale;

	//Draw The String
	vec3 dir = rotate(vec3(size.x, 0.f, 0.f), angle);
	vec3 diry = rotate(vec3(0.f, size.y , 0.f), angle);

	pos += dir * (fontStringWidth(font, s) * reloff[0] - (float)getFontChar(font,s[0]).x) + diry * ((float)fontHeight(font) * reloff[1]);

	for (int i = 0; i < s.length(); i++) {
		auto& fc = getFontChar(font, s[i]);

		vec3 csz = size * vec3(fc.w, fc.h, 1.f);
		vec3 cpos = pos + dir * (float)fc.xc + diry * (float)fc.yc;

		glBindTexture(GL_TEXTURE_2D, fc.tex);
		drawShape(shape_square, cpos, csz, angle, color);

		pos += dir * fc.xgap;
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	(*tmpShader)();
}

void drawFontString2D(FontID font, string s, vec3 pos, vec3 size, vec3 angle, vec4 color) {
	drawFontString(font, s, &shader_font_2d, vec2(0.f, -1.f), pos, size, angle, color);
}

void drawFontString2DCentered(FontID font, string s, vec3 pos, vec3 size, vec3 angle, vec4 color) {
	drawFontString(font, s, &shader_font_2d, vec2(-0.5f, -0.5f), pos, size, angle, color);
}

void drawFontString3D(FontID font, string s, vec3 pos, vec3 size, vec3 angle, vec4 color) {
	drawFontString(font, s, &shader_font_3d, vec2(0.f, -1.f), pos, size, angle, color);
}

void drawFontString3DCentered(FontID font, string s, vec3 pos, vec3 size, vec3 angle, vec4 color) {
	drawFontString(font, s, &shader_font_3d, vec2(-0.5f, -0.5f), pos, size, angle, color);
}

//TODO TODO TODO
void drawShapeCorrected(Shape* shape, vec3 pos, vec3 size, vec3 angle, vec4 color) {
	mat4 mdl = calcModelMat(pos, size, angle);

}

#endif