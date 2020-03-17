string vert = "#version 330 core\n\
\
layout (location = 0) in vec3 position;\
layout (location = 1) in vec3 normal;\
\
uniform mat4 model;\
uniform mat4 view;\
uniform mat4 projection;\
\
out vec3 normal_frag;\
out vec3 view_frag;\
\
void main(){\
	mat4 MV = view*model;\
	normal_frag = normalize(mat3(transpose(inverse(MV)))*normal);\
	gl_Position = MV * vec4(position, 1.f);\
	view_frag = -normalize(vec3(gl_Position));\
	gl_Position = projection * gl_Position;\
}";