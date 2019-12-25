#include "visualizers.h"
#if GFX_ENABLED

#define MAX_DEPTH 10

namespace {
	struct _ : public Visualizer {
		_() {
			name = "Basic Multi-List Visualizer";
		}

		bool drawList(VisualizerListInfo* inf, List<int>* l, int depthInd, int depth) { return depth < MAX_DEPTH; }

		void drawElement(VisualizerListInfo* inf, List<int>* l, int depthInd, ListElement<int>* e, int pos, int depth, bool marked) {
			float width = 2.f / inf->realSize;
			float height = (float)e->val / inf->realSize * 2.f / MAX_DEPTH;
			float x = ((float)l->offset + (float)pos) / inf->realSize * 2.f - 1.f + width / 2.f;
			float y = -1.f + (float)depth / MAX_DEPTH * 2.f + height / 2.f;
			vec4 col = marked ? vec4(0.f, 0.f, 0.f, 1.f) : colHSV((float)e->val / inf->realSize, 1.f, 1.f);
			//drawShape(shape_square, vec3(x, y, 0.f), vec3(width, height, 1.f),vec3(),col);
		}
	}_;
}

Visualizer* visualizer_basic_multilist = &::_;

#endif