#pragma once
#include "../gfx_main.h"
#if GFX_ENABLED
#include "../../fwk/lists.h"
#include "../shapes/shapes.h"
#include "../fwk/gfx_fwk_draw.h"
#include "../shaders/shaders.h"
#include "../gfx_color.h"
#include "gfx_fwk_visualizer_ext.h"

struct Visualizer;

extern vector<Visualizer*> visualizers;

extern Visualizer* currentVisualizer;

struct Visualizer {
	string name;
	Shader* shader = &shader_basic;

	//Called when the window is resized
	virtual void resize(int w, int h, float aspect) {}

	void resize_(int w, int h, float aspect) {
		//glUniform1i(uniformLocation("scrw"), w);
		//glUniform1i(uniformLocation("scrh"), h);
		resize(w, h, aspect);
	}

	//Called when this Visualizer is initialized. Set projection and view matrices here (shaders as well if wanted).
	virtual void init() {}

	//Called when this visualizer is deinitialized.
	virtual void deinit() {}

	//Default settings
	void init_() {
		(*shader)();
		setViewMat(mat4(1.f));
		setProjectionMat(mat4(1.f));
		glFrontFace(GL_CW);
		glClearColor(31.f / 255.f, 31.f / 255.f, 31.f / 255.f, 1.f);
		resize_(scrw, scrh, (float)scrw / (float)scrh);
		init();
	}

	//Called before rendering a frame to prepare variables
	virtual void prepare(VisualizerListInfo* inf, List<int>* mainList, double elapsedSeconds) {}

	//Used to determine whether or not to draw this list. Also, can be used to decorate the list area
	virtual bool drawList(VisualizerListInfo* inf, List<int>* l, int depthInd, int depth) { return true; }

	//Function to draw between two associated lists
	virtual void drawListLink(VisualizerListInfo* inf, List<int>* parent, List<int>* child, int parentDepthInd, int childDepthInd, int parentDepth) {}

	//Determines if an element should be considered marked
	virtual bool elementIsMarked(VisualizerListInfo* inf, List<int>* l, int depthInd, ListElement<int>* e, int pos, int depth) {
		return (e->lastInstanceID == instanceID && (threadStates[e->lastThread].accessCount - e->lastAccess) < ((int)pow(10.0/3.0,log10(inf->totalSize)-1.0)));
	}

	//Function to draw an element
	virtual void drawElement(VisualizerListInfo* inf, List<int>* l, int depthInd, ListElement<int>* e, int pos, int depth, bool marked) {}

	//Function to draw between 2 elements in the same list
	virtual void drawLink(VisualizerListInfo* inf, List<int>* l, int depthInd, ListElement<int>* e1, ListElement<int>* e2, int pos1, int pos2, int depth) {}

	Visualizer() {
		visualizers.push_back(this);
	}

	//Loads this visualizer
	inline void operator()() {
		if (currentVisualizer)
			currentVisualizer->deinit();
		init_();
		currentVisualizer = this;
	}
};

#endif