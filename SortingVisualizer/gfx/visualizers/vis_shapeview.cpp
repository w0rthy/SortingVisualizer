#include "visualizers.h"
#if GFX_ENABLED

#define CAM_DEG_PER_SEC 10.f

namespace {
	struct _ : public Visualizer {
		_() {
			name = "Shape Viewer";
			shader = &shader_basic_3d;
		}

		void init() {
			usePerspective(45.f, 1.f, 0.1f, 100.f);
		}

		//mat4 cam_mat = glm::translate(mat4(1.f), vec3(0.f, 0.f, -2.414f));
		mat4 cam_mat = getCamMat(vec3(0.f, 0.f, -2.414f));
		float cam_angle = 0.f;

		void prepare(VisualizerListInfo* inf, List<int>* mainList, double elapsedSeconds) {
			cam_angle += elapsedSeconds * CAM_DEG_PER_SEC;
			//setViewMat(glm::rotate(cam_mat, glm::radians(cam_angle), vec3(0.f, 1.f, 0.f)));
			setViewMat(rotMat(cam_mat, vec3(0.f, cam_angle, 0.f)));
		}

		bool drawList(VisualizerListInfo* inf, List<int>* l, int depthInd, int depth) {
			drawShape(shape_sphere);
			return false;
		}
	}_;
}

//Visualizer* visualizer_basic_galaxy = &::_;

#endif