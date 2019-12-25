#pragma once
#include "gfx_main.h"
#if GFX_ENABLED
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/normal.hpp>

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat3 = glm::mat3;

struct vertexVector : public vector<float> {
	using vector<float>::vector;

	using vector<float>::push_back;

	void push_back(const vec3& v) {
		push_back(v[0]);
		push_back(v[1]);
		push_back(v[2]);
	}

	inline vertexVector& operator+=(const vec3& v) {
		push_back(v);
		return *this;
	}

	inline vertexVector& operator+=(const vertexVector& vec) {
		for (auto& v : vec)
			push_back(v);
		return *this;
	}
};

struct mat4 : public glm::mat4 {
	using glm::mat4::mat4;

	//Convenience
	inline operator GLfloat* () {
		return (GLfloat*)glm::value_ptr(*(glm::mat4*)this);
	}

	vertexVector operator*(vertexVector& vvec) {
		vertexVector tmp;

		vec4 v;
		for (int i = 0; i < vvec.size(); i += 3) {
			v = vec4(vvec[i], vvec[i + 1], vvec[i + 2], 0);
			tmp += (*this) * v;
		}

		return tmp;
	}
};

inline void glUniformMat4f(GLuint location, mat4 mat) {
	glUniformMatrix4fv(location, 1, GL_FALSE, mat);
}

inline void glUniformVec4f(GLuint location, vec4 vec) {
	glUniform4f(location, vec[0], vec[1], vec[2], vec[3]);
}

#endif