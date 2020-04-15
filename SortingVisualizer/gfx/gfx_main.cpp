#include "gfx_main.h"
#if GFX_ENABLED
#include "gfx_draw.h"
#include "fwk/gfx_fwk_shape.h"
#include "fwk/gfx_fwk_shader.h"
#include "fwk/gfx_fwk_visualizer.h"
#include "gfx_request.h"
#include "../fwk/fwk.h"
#include "fwk/gfx_fwk_font.h"

#define FORCE_PERFECT_ASPECT 0
#define AA_MULTISAMPLES 16

int scrw, scrh;
float scraspect;

void gfx_resize(GLFWwindow* win, int w, int h) {
#if FORCE_PERFECT_ASPECT
	w < h ? h = w : w = h;
#endif
	scrw = w;
	scrh = h;
	scraspect = (float)w / (float)h;
	glViewport(0, 0, w, h);

	if (currentVisualizer)
		currentVisualizer->resize_(w, h, scraspect);
}

void gfx_init(int w, int h, string title) {

	glfwInit();
	//Set number of samples per pixel
	glfwWindowHint(GLFW_SAMPLES, AA_MULTISAMPLES);

	GLFWwindow* win = glfwCreateWindow(w, h, title, 0, 0);
	glfwMakeContextCurrent(win);
	glewExperimental = true;
	glewInit();

	//glew post-init
	bindAllShapes();
	compileAllShaders();
	if(currentVisualizer)
		currentVisualizer->init_();
	loadFont("arial.ttf", 32);

	//Sync to refresh rate
	glfwSwapInterval(1);

	glfwSetFramebufferSizeCallback(win, gfx_resize);
	gfx_resize(win, w, h);
	
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDepthMask(GL_FALSE);

	//double lastFrame = glfwGetTime();
	while (!glfwWindowShouldClose(win)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Use any requested visualizer and/or shader
		gfx_checkRequests();

		state.token_draw();
		//Draw Here
		gfx_drawFrame();

		state.token_draw++;

		glfwSwapBuffers(win);

		glfwPollEvents();
	}

	unloadAllFonts();

	glfwDestroyWindow(win);
	glfwTerminate();
}

#endif