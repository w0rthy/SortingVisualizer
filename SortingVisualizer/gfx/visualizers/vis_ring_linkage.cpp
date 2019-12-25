#include "visualizers.h"
#if GFX_ENABLED

#define CAM_DEG_PER_SEC 10.f

#define LINK_WIDTH 0.1f
#define RADIUS 1.5f

namespace {
	struct _ : public Visualizer {
		_() {
			name = "Ring Linkage Visualizer";
			shader = &shader_basic_3d;
		}

		void init() {
			usePerspective(45.f, 1.f, 0.1f, 100.f);
		}

		//mat4 cam_mat = rotMat(vec3(30.f, 0.f, 0.f)) * glm::translate(mat4(1.f), vec3(0.f, -2.f, -4.f));
		mat4 cam_mat = getCamMat(vec3(0.f, 2.f, -4.f));
		float cam_angle = 0.f;

		vec3 lastPos;
		vec3 lastDir;
		vec3 curang;
		float angstep = 0.f;
		float link_length;

		void prepare(VisualizerListInfo* inf, List<int>* mainList, double elapsedSeconds) {
			cam_angle += elapsedSeconds * CAM_DEG_PER_SEC;
			setViewMat(rotMat(cam_mat, vec3(0.f, cam_angle, 0.f)));

			link_length = 2.f * 3.14159f * RADIUS / inf->realSize;
			angstep = -360.f / inf->realSize;
			curang = vec3(0.f, angstep / 2.f, 0.f);
			lastPos = vec3(0.f, 0.f, -RADIUS);
			lastDir = rotate(vec3(1.f, 0.f, 0.f), -curang);
		}

		bool drawList(VisualizerListInfo* inf, List<int>* l, int depthInd, int depth) { return depth == 0; }

		void drawElement(VisualizerListInfo* inf, List<int>* l, int depthInd, ListElement<int>* e, int pos, int depth, bool marked) {
			float dist = calcDisparityf(e->val, pos, inf->realSize);
			
			curang = rotate(curang, vec3(dist * angstep, angstep, 0.f));
			lastDir = rotate(lastDir, vec3(dist*angstep,angstep,0.f));
			vec3 halfstep = lastDir * link_length / 2.f;

			vec3 p = lastPos + halfstep;

			vec4 col = marked ? vec4(0.f, 0.f, 0.f, 1.f) : colHSV((float)e->val / inf->realSize, 1.f, 1.f);
			drawShape(shape_cube, p, vec3(link_length, LINK_WIDTH, LINK_WIDTH), curang, col);

			lastPos = p + halfstep;
		}
	}_;
}

Visualizer* visualizer_ring_linkage = &::_;

#endif