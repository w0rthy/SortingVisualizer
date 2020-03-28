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
out vec2 texPos;\
out vec4 color_frag;\
\
void main(){\
	texPos = vec2(position.x+0.5f,-position.y+0.5f);\
	gl_Position = model * vec4(position, 1.0f);\
	color_frag = color;\
}";