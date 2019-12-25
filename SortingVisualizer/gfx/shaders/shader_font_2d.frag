string frag = "#version 330 core\n\
\
in vec2 texPos;\
\
uniform sampler2D tex;\
\
uniform vec4 color;\
\
uniform int scrw;\
uniform int scrh;\
\
void main(){\
	vec4 val = vec4(1.f, 1.f, 1.f, texture(tex, texPos).r);\
	gl_FragColor = color*val;\
}";