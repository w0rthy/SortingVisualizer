string frag = "#version 330 core\n\
\
in vec2 texPos;\
\
uniform sampler2D tex;\
\
uniform vec4 color;\
\
void main(){\
	gl_FragColor = color;\
	gl_FragColor[3] *= texture(tex, texPos).r;\
}";