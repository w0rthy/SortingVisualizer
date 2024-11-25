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
struct mat4;

//Denotes an undefined normal
#define NONORMAL vec3(FLT_MAX,FLT_MAX,FLT_MAX)

struct vertex {
	vec3 pos;
	vec3 normal;
	vec4 color;

	vertex() {
		normal = NONORMAL;
		color = vec4(1.f, 1.f, 1.f, 1.f);
	}

	vertex(vec3 pos) : vertex() {
		this->pos = pos;
	}

	vertex(vec3 pos, vec3 normal) : vertex(pos) {
			this->normal = normal[0]==FLT_MAX?normal:glm::normalize(normal);
	}

	vertex(vec3 pos, vec3 normal, vec4 color) : vertex(pos,normal) {
		this->color = color;
	}
};

struct vertexVector : public vector<vertex> {
	using vector<vertex>::vector;

	void push_back(const vertex& v) {
		vector<vertex>::push_back(v);
		auto sz = size();
		//Check for undefined normals, and define them
		if (sz % 3 == 0) {
			vec3 norm = glm::triangleNormal(at(sz - 3).pos, at(sz - 2).pos, at(sz - 1).pos);
			for (int i = 1; i <= 3; i++) {
				auto& tmp = at(sz - i);
				if (tmp.normal[0] == FLT_MAX)
					tmp.normal = norm;
			}
		}
	}

	inline vertexVector& operator+=(const vertex& v) {
		push_back(v);
		return *this;
	}

	inline vertexVector& operator+=(const vertexVector& vec) {
		for (auto& v : vec)
			push_back(v);
		return *this;
	}

	inline void flipLastTri() {
		auto sz = size();
		push_back(at(sz - 3));
		push_back(at(sz - 1));
		push_back(at(sz - 2));
		//Flip the normals
		for (int i = 0; i < 3; i++)
			at(sz + i).normal *= -1.f;
	}

	inline void setColor(const vec4& col) {
		for (auto& v : *this) {
			v.color = col;
		}
	}

	void mutate(mat4& mat);
};

struct mat4 : public glm::mat4 {
	using glm::mat4::mat4;

	//Convenience
	inline operator GLfloat* () {
		return (GLfloat*)glm::value_ptr(*(glm::mat4*)this);
	}

	vertexVector operator*(vertexVector& vvec) {
		vertexVector tmp;
		tmp.reserve(vvec.size());
		mat3 normMat = glm::transpose(glm::inverse((mat3)*this));
		
		for (auto& v : vvec)
			tmp.push_back(vertex((vec3)(*this * vec4(v.pos, 1.f)), normMat * v.normal, v.color));

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