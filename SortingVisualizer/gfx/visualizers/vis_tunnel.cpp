#include "visualizers.h"
#if GFX_ENABLED

#define TUBE_LENGTH 2.f
#define FOV 100.f

namespace {
	struct _ : public Visualizer {
		_() {
			name = "Tunnel Visualizer";
			shader = &shader_basic_3d;
		}

		float zOffset = 0.f;
		mat4 mutate = rotMat(vec3(90.f, 0.f, 0.f));

		void init() {
			usePerspective(FOV, 1.f, 0.1f, 100.f);
			float t = tan(glm::radians(FOV / 2.f));
			zOffset = tanf(glm::radians(90.f) - acosf(powf(2.f * t * t + 1.f, -0.5f))) * 0.5f;
		}

		void prepare(VisualizerListInfo* inf, List<int>* mainList, double elapsedSeconds) {
		}

		bool drawList(VisualizerListInfo* inf, List<int>* l, int depthInd, int depth) { return depth == 0; }

		void drawElement(VisualizerListInfo* inf, List<int>* l, int depthInd, ListElement<int>* e, int pos, int depth, bool marked) {
			float len = TUBE_LENGTH / l->sz;
			float dist = calcDisparityf(e->val, pos, inf->realSize);

			float z = (float)pos * len + zOffset + len/2.f;

			vec4 col = marked ? vec4(0.f, 0.f, 0.f, 1.f) : colHSV((float)e->val / inf->realSize, 1.f, 1.f);
			drawShape(shape_ring, mutate, vec3(0.f, 0.f, z), vec3(1.f, 1.f, len), vec3(), col);
		}
	}_;
}

Visualizer* visualizer_tunnel = &::_;

#endif