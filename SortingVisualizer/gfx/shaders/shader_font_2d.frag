string frag = "#version 330 core\n\
\
in vec2 texPos;\
in vec4 color_frag;\
\
uniform sampler2D tex;\
\
uniform vec4 color;\
\
void main(){\
	gl_FragColor = color*color_frag;\
	gl_FragColor[3] *= texture(tex, texPos).r;\
}";