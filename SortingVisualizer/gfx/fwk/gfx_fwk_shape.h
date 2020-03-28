#pragma once
#include "../gfx_common.h"
#if GFX_ENABLED

struct Shape;

extern vector<Shape*> shapes;

//Must be called after glew is initialized
void bindAllShapes();

struct Shape {
private:
	bool constructed = false;
public:
	vertexVector vertices;
	GLuint vao;


private:
	GLuint vbo;

public:

	//Construct the vertices
	void build() {

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//Vertex data
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
		//Pos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//Normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vec3)));
		glEnableVertexAttribArray(1);
		//Color
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vec3) * 2));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

	Shape() {
		shapes.push_back(this);
	}

	~Shape() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}
};

#endif