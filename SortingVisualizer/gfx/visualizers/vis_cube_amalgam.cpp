#include "visualizers.h"
#if GFX_ENABLED

#define RADIUS 1.f
#define EL_SZ 0.6f
#define CAM_DEG_PER_SEC 10.f

namespace {
	struct _ : public Visualizer {
		_() {
			name = "Cube Amalgam Visualizer";
			shader = &shader_basic_3d;
		}

		vector<mat4> mutations;

		void init() {
			usePerspective(55.f, 1.f, 0.1f, 100.f);

			mutations.push_back(mat4(1.f));
			for (int i = 1; i < 4; i++) {
				mutations.push_back(rotMat(vec3(0.f, 0.f, 90.f * i)));
			}
			mutations.push_back(rotMat(vec3(90.f, 0.f, 0.f)));
			mutations.push_back(rotMat(vec3(-90.f, 0.f, 0.f)));
		}

		//mat4 cam_mat = rotMat(vec3(30.f, 0.f, 0.f)) * glm::translate(mat4(1.f), vec3(0.f, -2.f, -4.f));
		mat4 cam_mat = getCamMat(vec3(0.f, 2.f, 4.f));
		float cam_angle = 0.f;

		int side_length;
		float el_sz;

		void prepare(VisualizerListInfo* inf, List<int>* mainList, double elapsedSeconds) {
			cam_angle += elapsedSeconds * CAM_DEG_PER_SEC;
			setViewMat(rotMat(cam_mat, vec3(0.f, cam_angle, 0.f)));

			side_length = ceilf(sqrtf(mainList->sz));
			el_sz = 2.f * RADIUS / side_length;
		}

		bool drawList(VisualizerListInfo* inf, List<int>* l, int depthInd, int depth) { return depth == 0; }

		void drawElement(VisualizerListInfo* inf, List<int>* l, int depthInd, ListElement<int>* e, int pos, int depth, bool marked) {
			float dist = calcDisparityf(e->val, pos, inf->realSize);

			int c = pos / side_length;
			//int r = pos % side_length; //DEFAULT
			int r = abs((pos % side_length) - (side_length - 1) * (c % 2)); //STAGGERED ROW DIRECTIONS

			vec3 p(
				-RADIUS + el_sz/2.f + (float)c / (float)side_length * 2.f * RADIUS,
				RADIUS - dist * 2.f * RADIUS,
				-RADIUS + el_sz/2.f + (float)r / (float)side_length * 2.f * RADIUS
			);

			vec4 col = marked ? vec4(0.f, 0.f, 0.f, 1.f) : colHSV((float)e->val / inf->realSize, 1.f, 1.f);
			for (auto& mat : mutations)
				drawShape(shape_cube, mat*calcModelMat(p, EL_SZ * vec3(el_sz, 0.02f, el_sz), vec3()), col);
		}
	}_;
}

Visualizer* visualizer_cube_amalgam = &::_;

#endif