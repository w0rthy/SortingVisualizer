string vert = "#version 330 core\n\
\
layout (location = 0) in vec3 position;\
layout (location = 1) in vec3 normal;\
\
uniform mat4 model;\
uniform mat4 view;\
uniform mat4 projection;\
\
void main(){\
	mat4 MVP = projection*view*model;\
	gl_Position = MVP * vec4(position, 1.0f);\
}";