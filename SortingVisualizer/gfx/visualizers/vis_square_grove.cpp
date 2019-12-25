#include "visualizers.h"
#if GFX_ENABLED

#define CAM_DEG_PER_SEC 10.f
#define TREE_HEIGHT 0.1f
#define TREE_TRUNK_HEIGHT 0.05f
#define TREE_SIZE_MUL 0.6f
#define TREE_TRUNK_SIZE_MUL 0.5f
#define TREE_SPROUT_DELAY 1.f

namespace {
	struct _ : public Visualizer {
		_() {
			name = "Square Grove Visualizer";
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
			el_sz = 2.f / side_length * TREE_SIZE_MUL;
		}

		bool drawList(VisualizerListInfo* inf, List<int>* l, int depthInd, int depth) {
			//Draw the SOIL
			drawShape(shape_cube, vec3(0.f,-TREE_HEIGHT-TREE_TRUNK_HEIGHT/2.f,0.f), vec3(2.f+el_sz*2.f, TREE_HEIGHT+TREE_TRUNK_HEIGHT, 2.f+el_sz*2.f), vec3(), vec4(87.f/255.f,59.f/255.f,12.f/255.f,1.f));
			return depth == 0;
		}

		void drawElement(VisualizerListInfo* inf, List<int>* l, int depthInd, ListElement<int>* e, int pos, int depth, bool marked) {
			float dist = calcDisparityf(e->val, pos, inf->realSize);
			//dist = sqrtf(dist);
			constexpr float distmul = 1.f+TREE_SPROUT_DELAY;
			dist = fminf(distmul * dist,1.f);

			int c = pos / side_length;
			//int r = pos % side_length; //DEFAULT
			int r = abs((pos % side_length) - (side_length - 1) * (c % 2)); //STAGGERED ROW DIRECTIONS

			vec3 p(
				-1.f + (float)c / (float)side_length * 2.f,
				TREE_TRUNK_HEIGHT - dist * (TREE_HEIGHT+TREE_TRUNK_HEIGHT),
				-1.f + (float)r / (float)side_length * 2.f
			);

			vec4 col = marked ? vec4(1.f, 0.f, 0.f, 1.f) : vec4(0.f,0.7f,0.f,1.f);
			drawShape(shape_cone, p, vec3(el_sz, TREE_HEIGHT, el_sz), vec3(), col);

			drawShape(shape_cylinder, p - vec3(0.f, TREE_HEIGHT / 2.f + TREE_TRUNK_HEIGHT / 2.f, 0.f), vec3(el_sz * TREE_TRUNK_SIZE_MUL, TREE_TRUNK_HEIGHT, el_sz * TREE_TRUNK_SIZE_MUL), vec3(), vec4(193.f / 255.f, 154.f / 255.f, 107.f / 255.f, 1.f));
		}
	}_;
}

Visualizer* visualizer_square_grove = &::_;

#endif