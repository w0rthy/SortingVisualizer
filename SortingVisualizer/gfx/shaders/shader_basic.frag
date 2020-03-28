string frag = "#version 330 core\n\
\
uniform vec4 color;\
\
in vec4 color_frag;\
\
void main(){\
	gl_FragColor = color*color_frag;\
}";