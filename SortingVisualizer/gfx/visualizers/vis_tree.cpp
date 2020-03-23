#include "visualizers.h"
#if GFX_ENABLED

#define CAM_DEG_PER_SEC 10.f

#define CENTER_RADIUS 0.02f
#define HEIGHT 2.f
#define NUMRINGS 23
#define EL_SZ 0.025f
#define MAXLENGTH 0.75f
#define MINLENGTH 0.01f

#define ADDLENGTH (MAXLENGTH-MINLENGTH)

namespace {
	struct _ : public Visualizer {
		_() {
			name = "Tree Visualizer";
			shader = &shader_basic_3d;
		}

		void init() {
			usePerspective(65.f, 1.f, 0.1f, 100.f);
		}

		mat4 cam_mat = getCamMat(vec3(0.f, 0.f, -2.414f));
		float cam_angle = 0.f;

		float angstep = 0.f;
		float zstep = 0.f;
		vec3 startPos = vec3(CENTER_RADIUS, -HEIGHT/2.f, 0.f);
		mat4 mutate = rotMat(vec3(90.f, 0.f, 0.f));

		void prepare(VisualizerListInfo* inf, List<int>* mainList, double elapsedSeconds) {
			cam_angle += elapsedSeconds * CAM_DEG_PER_SEC;
			setViewMat(rotMat(cam_mat, vec3(0.f, cam_angle, 0.f)));
			angstep = (float)NUMRINGS * 360.f / mainList->sz;
			zstep = HEIGHT / mainList->sz;
		}

		bool drawList(VisualizerListInfo* inf, List<int>* l, int depthInd, int depth) { return depth == 0; }


		void drawElement(VisualizerListInfo* inf, List<int>* l, int depthInd, ListElement<int>* e, int pos, int depth, bool marked) {
			float len = (1.f - (float)e->val / (float)inf->realSize) * ADDLENGTH + MINLENGTH;
			float ang = angstep * (float)pos;
			vec3 p = startPos;
			p[0] += len / 2.f;
			p = rotate(p, vec3(0.f, ang, 0.f));
			p[1] += zstep * pos;

			vec4 col = marked ? vec4(0.f, 0.f, 0.f, 1.f) : colHSV((float)e->val / inf->realSize, 1.f, 1.f);

			drawShape(shape_cylinder, mutate, p, vec3(len,EL_SZ,EL_SZ), vec3(0.f, ang, 0.f), col);
		}
	}_;
}

Visualizer* visualizer_tree = &::_;

#endif