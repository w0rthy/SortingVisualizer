#include "visualizers.h"
#if GFX_ENABLED

#define CAM_DEG_PER_SEC 10.f

#define EL_SZ 0.05f
#define NUMRINGS 20

namespace {
	struct _ : public Visualizer {
		_() {
			name = "Basic Galaxy Visualizer";
			shader = &shader_basic_3d;
		}

		void init() {
			usePerspective(45.f, 1.f, 0.1f, 100.f);
		}

		//mat4 cam_mat = glm::translate(mat4(1.f), vec3(0.f, 0.f, -2.414f));
		//mat4 cam_mat = glm::lookAt(vec3(0.f, 0.3f, -2.414f), vec3(0.f, 0.f, 0.f), vec3(0.f, 1.f, 0.f));
		mat4 cam_mat = getCamMat(vec3(0.f, 0.f, -2.414f));
		float cam_angle = 0.f;

		void prepare(VisualizerListInfo* inf, List<int>* mainList, double elapsedSeconds) {
			cam_angle += elapsedSeconds * CAM_DEG_PER_SEC;
			setViewMat(rotMat(cam_mat, vec3(0.f, cam_angle, 0.f)));
		}

		bool drawList(VisualizerListInfo* inf, List<int>* l, int depthInd, int depth) { return depth == 0; }

		void drawElement(VisualizerListInfo* inf, List<int>* l, int depthInd, ListElement<int>* e, int pos, int depth, bool marked) {
			//constexpr mat3 mutate = mat3(1.f, 0.f, 0.f, 0.f, 2.f, 0.f, 0.f, 0.f, 1.f);
			float angstepP = 180.f / l->sz;
			float angstepY = NUMRINGS * 360.f / l->sz;
			
			float dist = calcDisparityf(e->val, pos, inf->realSize);
			vec3 p = rotate(vec3(0.f,0.f,0.2f+dist*0.6f),vec3(90.f - pos*angstepP,pos*angstepY,0.f));

			vec4 col = marked ? vec4(0.f, 0.f, 0.f, 1.f) : colHSV((float)e->val / inf->realSize, 1.f, 1.f);
			drawShape(shape_sphere, p, vec3(EL_SZ, EL_SZ, EL_SZ), vec3(), col);
		}
	}_;
}

Visualizer* visualizer_basic_galaxy = &::_;

#endif