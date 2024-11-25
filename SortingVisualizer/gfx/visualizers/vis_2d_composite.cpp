#include "visualizers.h"
#if GFX_ENABLED

#define DEPTH_CONTRACT_SPD 2.0
#define DEPTH_EXPAND_SPD 4.0
#define DEPTH_GRACE_TIME 0.5

namespace {
	struct _ : public Visualizer {
		_() {
			name = "2D Composite Visualizer";
		}

		float maxDepth = 1.f;
		float graceTime = 0.f;

		void prepare(VisualizerListInfo* inf, List<int>* mainList, double elapsedSeconds) {
			if (inf->maxDepth >= (int)ceil(maxDepth)) {
				maxDepth = fmin(maxDepth + (float)((double)inf->maxDepth * elapsedSeconds * DEPTH_CONTRACT_SPD), inf->maxDepth);
				graceTime = 0.f;
			}
			else {
				if (graceTime < DEPTH_GRACE_TIME) {
					graceTime += (float)elapsedSeconds;
					double ovr = (double)graceTime - DEPTH_GRACE_TIME;
					elapsedSeconds = ovr;
				}
				if (graceTime >= DEPTH_GRACE_TIME)
					maxDepth = fmax(maxDepth - (float)((double)maxDepth * elapsedSeconds * DEPTH_EXPAND_SPD), inf->maxDepth);
			}
		}

		bool drawList(VisualizerListInfo* inf, List<int>* l, int depthInd, int depth) { return depth < maxDepth; }

		void drawElement(VisualizerListInfo* inf, List<int>* l, int depthInd, ListElement<int>* e, int pos, int depth, bool marked) {
			int sz = inf->depthSize[depth];
			sz = sz > inf->realSize ? sz : inf->realSize;

			float fh = (float)fontHeight(0)/(float)scrh*scraspect*2.f;
			float w = (2.f / (float)sz);
			float h = 2.f / (float)maxDepth;
			float eh = (h - fh) / 2.f;
			float val = (float)e->val / (float)sz;
			float x = -1.f + 2.f * (float)(l->offset+pos) / (float)sz + w / 2.f;
			float y = -1.f + 2.f * (float)depth / (float)maxDepth;
			//vec4 col = marked ? vec4(0.f, 0.f, 0.f, 1.f) : colHSV((float)e->val / inf->realSize, 1.f, 1.f);
			//vec4 col = marked ? vec4(1.f) : colHSV((float)e->val / inf->realSize, 1.f, 1.f);
			//vec4 col = colHSV((float)e->val / inf->realSize, 1.f, 1.f)*(marked?vec4(0.5f, 0.5f, 0.5f, 1.f):vec4(1.f));
			//vec4 col = colHSV((float)e->val / inf->realSize, 1.f, 1.f) * (marked ? vec4(1.f) : vec4(0.5f, 0.5f, 0.5f, 1.f));
			vec4 col = colHSV((float)e->val / inf->realSize, 1.f, 1.f) + (marked ? vec4(2.f) : vec4(0.f));
			col /= col[3];
			drawShape(shape_square, vec3(x, y + (h + fh + eh * val) / 2.f, 0.f), vec3(w / 2.f, eh * val, 1.f), vec3(), col);
			drawShape(shape_square, vec3(x, y + (h - fh - eh * val) / 2.f, 0.f), vec3(w / 2.f, eh * val, 1.f), vec3(), col);
			drawFontString2DCenteredCorrected(0, std::to_string(e->val), vec3(x, y + h / 2.f , 0.f), vec3(1.f, 1.f, 1.f), vec3(), col);
		}
	}_;
}

Visualizer* visualizer_2d_composite = &::_;

#endif