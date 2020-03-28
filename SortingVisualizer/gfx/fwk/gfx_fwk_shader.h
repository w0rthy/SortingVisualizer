#pragma once
#include "../gfx_common.h"
#if GFX_ENABLED
#include <fstream>
#include <sstream>

struct Shader;

extern Shader* currentShader;

extern vector<Shader*> shaders;

//Must be called after glew is initialized
void compileAllShaders();

struct Shader {
private:
	GLuint prgrm;
	const char* name, * vert, * frag, * geom;
public:
	inline void operator()() {
		glUseProgram(prgrm);
		currentShader = this;
	}

	//Implicitly casts to the program, allows ease of use
	inline operator GLuint() {
		return prgrm;
	}

	void compile() {
		GLuint shader_vert, shader_frag, shader_geom;
		shader_vert = glCreateShader(GL_VERTEX_SHADER);
		shader_frag = glCreateShader(GL_FRAGMENT_SHADER);
		if (geom)
			shader_geom = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(shader_vert, 1, &vert, 0);
		glShaderSource(shader_frag, 1, &frag, 0);
		if (geom)
			glShaderSource(shader_geom, 1, &geom, 0);
		glCompileShader(shader_vert);
		glCompileShader(shader_frag);
		if (geom)
			glCompileShader(shader_geom);
		checkCompile(shader_vert, "vertex");
		checkCompile(shader_frag, "fragment");
		if (geom)
			checkCompile(shader_geom, "geometry");

		prgrm = glCreateProgram();
		glAttachShader(prgrm, shader_vert);
		glAttachShader(prgrm, shader_frag);
		if (geom)
			glAttachShader(prgrm, shader_geom);
		glLinkProgram(prgrm);
		checkLink();
		glDeleteShader(shader_vert);
		glDeleteShader(shader_frag);
		if (geom)
			glDeleteShader(shader_geom);
	}

	Shader(const char* name, const char* vert, const char* frag, const char* geom = nullptr) {
		this->name = name;
		this->vert = vert;
		this->frag = frag;
		this->geom = geom;
		shaders.push_back(this);
	}

private:
	void checkCompile(GLuint shader, const char* type) {
		GLint success;
		GLchar info_log[1024];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, 0, info_log);
			printf("%s:\nFailed to compile %s shader: \n  %s\n", name, type, info_log);
		}
	}

	void checkLink() {
		GLint success;
		GLchar info_log[1024];
		glGetProgramiv(prgrm, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(prgrm, 1024, 0, info_log);
			printf("%s:\nFailed to link shader program: \n  %s\n", name, info_log);
		}
	}
};

//Ease of use
inline GLint uniformLocation(const GLchar* var) {
	return glGetUniformLocation(*currentShader, var);
}

#endif