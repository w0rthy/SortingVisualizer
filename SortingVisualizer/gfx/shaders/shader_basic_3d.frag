string frag = "#version 330 core\n\
\
uniform vec4 color;\
\
uniform int scrw;\
uniform int scrh;\
\
in vec3 normal_frag;\
\
void main(){\
	vec3 fragPos = vec3(gl_FragCoord)/vec3(scrw*0.5f,scrh*0.5f,1.f) - vec3(1.f,1.f,0.f);\
	vec3 fragVec = normalize(fragPos);\
	float err = degrees(acos(dot(fragVec,normal_frag)))/90.f;\
	float color_mul = 1.f - err*0.5f;\
    gl_FragColor = vec4(color[0]*color_mul,color[1]*color_mul,color[2]*color_mul,color[3]);\
}";