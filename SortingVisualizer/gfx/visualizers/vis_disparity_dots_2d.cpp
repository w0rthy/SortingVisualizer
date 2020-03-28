#include "visualizers.h"
#if GFX_ENABLED

#define DEPTH_CONTRACT_SPD 2.0
#define DEPTH_EXPAND_SPD 4.0
#define DEPTH_GRACE_TIME 0.5

#define RADIUS 0.9f
#define EL_SZ 0.01f

namespace {
	struct _ : public Visualizer {
		_() {
			name = "Disparity Dots (2D) Visualizer";
		}

		float angPer = 0.f;
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

		float minRadius = 0.f;
		float maxRadius = 1.f;

		bool drawList(VisualizerListInfo* inf, List<int>* l, int depthInd, int depth) {
			angPer = -360.f / l->sz;
			minRadius = 1.f - (float)(depth + 1) / maxDepth;
			maxRadius = (1.f - (float)(depth) / maxDepth - minRadius);
			return depth < maxDepth;
		}

		void drawElement(VisualizerListInfo* inf, List<int>* l, int depthInd, ListElement<int>* e, int pos, int depth, bool marked) {
			float dist = calcDisparityf(e->val, pos, inf->realSize);
			vec3 p = (mat3)rotMat(vec3(0.f, 0.f, angPer * e->val)) * vec3(0.f, 1.f, 0.f) * (((1.f - dist) * maxRadius + minRadius) * RADIUS);

			vec4 col = marked ? vec4(0.f, 0.f, 0.f, 1.f) : colHSV((float)e->val / inf->realSize, 1.f, 1.f);
			drawShape(shape_circle, p, vec3(EL_SZ, EL_SZ, EL_SZ), vec3(), col);
			//drawFontString2DCenteredCorrected(0, std::to_string(e->val), p, vec3(0.5f,0.5f,0.5f),vec3(),col);
		}
	}_;
}

Visualizer* visualizer_disparity_dots_2d = &::_;

#endif