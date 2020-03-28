string vert = "#version 330 core\n\
\
layout (location = 0) in vec3 position;\
layout (location = 1) in vec3 normal;\
layout (location = 2) in vec4 color;\
\
uniform mat4 model;\
uniform mat4 view;\
uniform mat4 projection;\
\
out vec3 normal_frag;\
out vec3 view_frag;\
out vec4 color_frag;\
out vec2 texPos;\
\
void main(){\
	mat4 MV = view*model;\
	normal_frag = normalize(mat3(transpose(inverse(MV)))*normal);\
	gl_Position = MV * vec4(position, 1.f);\
	view_frag = -normalize(vec3(gl_Position));\
	color_frag = color;\
	gl_Position = projection * gl_Position;\
	\
	texPos = vec2(position.x+0.5f,-position.y+0.5f);\
}";