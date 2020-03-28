#pragma once
#include "gfx_fwk_shape.h"
#if GFX_ENABLED
#include "../gfx_linalg.h"
#include "gfx_fwk_shader.h"
#include "gfx_fwk_font.h"

inline void setViewMat(mat4 m) {
	glUniformMat4f(uniformLocation("view"), m);
}

inline void setProjectionMat(mat4 m) {
	glUniformMat4f(uniformLocation("projection"), m);
}

inline mat4 perspective(float fovy, float aspect, float zNear, float zFar) {
	mat4 tmp = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	tmp[2] *= -1.f; //Fix the x axis
	return tmp;
}

inline void usePerspective(float fovy, float aspect, float zNear, float zFar) {
	setProjectionMat(perspective(fovy, aspect, zNear, zFar));
}

inline mat4 getCamMat(vec3 cam_pos) {
	return glm::lookAt(cam_pos, vec3(), vec3(0.f, 1.f, 0.f));
}

inline void useCamMat(vec3 cam_pos) {
	setViewMat(getCamMat(cam_pos));
}

mat4 calcModelMat(vec3 pos, vec3 size, vec3 angle);

//Draws the specified shape with the given model matrix and color
void drawShape(Shape* shape, mat4 mdl, vec4 color);

//Draws the specified shape with the specified parameters
inline void drawShape(Shape* shape, vec3 pos = vec3(0.f, 0.f, 0.f), vec3 size = vec3(1.f, 1.f, 1.f), vec3 angle = vec3(0.f, 0.f, 0.f), vec4 color = vec4(1.f, 1.f, 1.f, 1.f)) {
	drawShape(shape, calcModelMat(pos, size, angle), color);
}

//Draws the specified shape with the specified parameters and mutation matrix
inline void drawShape(Shape* shape, mat4& mutate, vec3 pos = vec3(0.f, 0.f, 0.f), vec3 size = vec3(1.f, 1.f, 1.f), vec3 angle = vec3(0.f, 0.f, 0.f), vec4 color = vec4(1.f, 1.f, 1.f, 1.f)) {
	drawShape(shape, calcModelMat(pos, size, angle)*mutate, color);
}

//Draws the specified shape such that it is not affected by aspect ratio
void drawShapeCorrected(Shape* shape, vec3 pos = vec3(0.f, 0.f, 0.f), vec3 size = vec3(1.f, 1.f, 1.f), vec3 angle = vec3(0.f, 0.f, 0.f), vec4 color = vec4(1.f, 1.f, 1.f, 1.f));

//Drawing Strings
void drawFontString2D(FontID font, string s, vec3 pos = vec3(0.f, 0.f, 0.f), vec3 size = vec3(1.f, 1.f, 1.f), vec3 angle = vec3(0.f, 0.f, 0.f), vec4 color = vec4(1.f, 1.f, 1.f, 1.f));
void drawFontString2DCentered(FontID font, string s, vec3 pos = vec3(0.f, 0.f, 0.f), vec3 size = vec3(1.f, 1.f, 1.f), vec3 angle = vec3(0.f, 0.f, 0.f), vec4 color = vec4(1.f, 1.f, 1.f, 1.f));

void drawFontString3D(FontID font, string s, vec3 pos = vec3(0.f, 0.f, 0.f), vec3 size = vec3(1.f, 1.f, 1.f), vec3 angle = vec3(0.f, 0.f, 0.f), vec4 color = vec4(1.f, 1.f, 1.f, 1.f));
void drawFontString3DCentered(FontID font, string s, vec3 pos = vec3(0.f, 0.f, 0.f), vec3 size = vec3(1.f, 1.f, 1.f), vec3 angle = vec3(0.f, 0.f, 0.f), vec4 color = vec4(1.f, 1.f, 1.f, 1.f));

inline void drawFontString2DCorrected(FontID font, string s, vec3 pos = vec3(0.f, 0.f, 0.f), vec3 size = vec3(1.f, 1.f, 1.f), vec3 angle = vec3(0.f, 0.f, 0.f), vec4 color = vec4(1.f, 1.f, 1.f, 1.f)) {
	size[0] /= scraspect;
	drawFontString2D(font, s, pos, size, angle, color);
}
inline void drawFontString2DCenteredCorrected(FontID font, string s, vec3 pos = vec3(0.f, 0.f, 0.f), vec3 size = vec3(1.f, 1.f, 1.f), vec3 angle = vec3(0.f, 0.f, 0.f), vec4 color = vec4(1.f, 1.f, 1.f, 1.f)) {
	size[0] /= scraspect;
	drawFontString2DCentered(font, s, pos, size, angle, color);
}

inline void drawFontString3DCorrected(FontID font, string s, vec3 pos = vec3(0.f, 0.f, 0.f), vec3 size = vec3(1.f, 1.f, 1.f), vec3 angle = vec3(0.f, 0.f, 0.f), vec4 color = vec4(1.f, 1.f, 1.f, 1.f)) {
	size[0] /= scraspect;
	drawFontString3D(font, s, pos, size, angle, color);
}
inline void drawFontString3DCenteredCorrected(FontID font, string s, vec3 pos = vec3(0.f, 0.f, 0.f), vec3 size = vec3(1.f, 1.f, 1.f), vec3 angle = vec3(0.f, 0.f, 0.f), vec4 color = vec4(1.f, 1.f, 1.f, 1.f)) {
	size[0] /= scraspect;
	drawFontString3DCentered(font, s, pos, size, angle, color);
}

#endif