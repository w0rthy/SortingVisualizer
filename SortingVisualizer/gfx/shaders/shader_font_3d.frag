string frag = "#version 330 core\n\
\
uniform sampler2D tex;\
uniform vec4 color;\
\
in vec3 normal_frag;\
in vec3 view_frag;\
in vec2 texPos;\
\
const float ambientAmt = 0.1f;\
const float diffuseAmt = 1.f-ambientAmt;\
const float specAmt = 1.f;\
const float specStrength = 128.f;\
\
void main(){\
	float dotRes = max(dot(normal_frag,view_frag),0.f);\
	gl_FragColor = color*ambientAmt;\
	gl_FragColor += color*diffuseAmt*dotRes;\
	gl_FragColor += vec4(1.f,1.f,1.f,0.f)*specAmt*pow(dotRes,specStrength);\
	gl_FragColor[3] = color[3]*texture(tex, texPos).r;\
}";