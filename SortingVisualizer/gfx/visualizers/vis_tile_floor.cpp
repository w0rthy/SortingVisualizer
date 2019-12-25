#include "visualizers.h"
#if GFX_ENABLED

#define CAM_DEG_PER_SEC 10.f

namespace {
	struct _ : public Visualizer {
		_() {
			name = "Tile Floor Visualizer";
			shader = &shader_basic_3d;
		}

		void init() {
			usePerspective(55.f, 1.f, 0.1f, 100.f);
		}

		//mat4 cam_mat = rotMat(vec3(30.f,0.f,0.f))*glm::translate(mat4(1.f), vec3(0.f, -0.7f, -2.414f));
		mat4 cam_mat = getCamMat(vec3(0.f, 1.f, -3.f));
		float cam_angle = 0.f;

		int side_length;
		float el_sz;

		void prepare(VisualizerListInfo* inf, List<int>* mainList, double elapsedSeconds) {
			cam_angle += elapsedSeconds * CAM_DEG_PER_SEC;
			setViewMat(rotMat(cam_mat, vec3(0.f, cam_angle, 0.f)));

			side_length = ceilf(sqrtf(mainList->sz));
			el_sz = 2.f/side_length;
		}

		bool drawList(VisualizerListInfo* inf, List<int>* l, int depthInd, int depth) { return depth == 0; }

		void drawElement(VisualizerListInfo* inf, List<int>* l, int depthInd, ListElement<int>* e, int pos, int depth, bool marked) {
			float dist = calcDisparityf(e->val, pos, inf->realSize);

			int c = pos / side_length;
			//int r = pos % side_length; //DEFAULT
			int r = abs((pos % side_length) - (side_length - 1) * (c % 2)); //STAGGERED ROW DIRECTIONS

			vec3 p(-1.f+(float)c/(float)side_length*2.f,-0.5f+dist*1.f,-1.f+(float)r/(float)side_length*2.f);

			vec4 col = marked ? vec4(0.f, 0.f, 0.f, 1.f) : colHSV((float)e->val / inf->realSize, 1.f, 1.f);
			drawShape(shape_cube, p, vec3(el_sz, 0.02f, el_sz), vec3(), col);
		}
	}_;
}

Visualizer* visualizer_tile_floor = &::_;

#endif