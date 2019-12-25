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
	vertexVector normals;
	GLuint vao;
	
	void defaultNormal(vec3& v1, vec3& v2, vec3& v3) {
		vec3 norm = glm::triangleNormal(v1, v2, v3);
		for (int i = 0; i < 3; i++)
			normals += norm;
	}

	//Calculate the normal(s) for a tri and add them to the normals vector
	virtual void calcNormal(int triNum, vec3& v1, vec3& v2, vec3& v3) {
		defaultNormal(v1, v2, v3);
	}

private:
	GLuint vbo, nbo;

	//Calc extra information for vertices
	void calcNormals() {
		normals.reserve(vertices.size());
		vec3 v1, v2, v3;
		for (int i = 0; i < vertices.size(); i += 9) {
			for (int j = 0; j < 3; j++) {
				v1[j] = vertices[i + j];
				v2[j] = vertices[i + 3 + j];
				v3[j] = vertices[i + 6 + j];
			}
			calcNormal(i/9,v1, v2, v3);
		}
	}

public:

	//Construct the vertices
	void build() {
		//Calculate based off of vertices
		calcNormals();

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//Vertex positions
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices.data())*vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//Normal vectors
		glGenBuffers(1, &nbo);
		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(*normals.data()) * normals.size(), normals.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
		glEnableVertexAttribArray(1);
		
		glBindVertexArray(0);
	}

	Shape() {
		shapes.push_back(this);
	}

	~Shape() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &nbo);
	}
};

#endif