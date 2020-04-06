#include "visualizers.h"
#if GFX_ENABLED

#define CAM_DEG_PER_SEC 10.0

//Horizontal:Vertical Ratio
#define RATIO 2.0
#define RADIUS 0.5f
#define RADIUS_DIFF 0.3f

namespace {
	struct _ : public Visualizer {
		_() {
			name = "Sphere Agitation Visualizer";
			shader = &shader_basic_3d;
		}

		Shape* verts;

		void init() {
			verts = new Shape();
		}

		void deinit() {
			delete verts;
		}

		mat4 cam_mat = getCamMatLookingAt(vec3(0.f, 0.f, -2.414f));
		float cam_angle = 0.f;

		void prepare(VisualizerListInfo* inf, List<int>* mainList, double elapsedSeconds) {
			cam_angle += elapsedSeconds * CAM_DEG_PER_SEC;
			setViewMat(rotMat(cam_mat, vec3(0.f, cam_angle, 0.f)));
			usePerspective(45.f, scraspect, 0.1f, 100.f);
		}

		bool drawList(VisualizerListInfo* inf, List<int>* l, int depthInd, int depth) { 
			if (depth != 0)
				return false;

			float x = sqrtf((float)l->sz);
			float tmpsqrt = sqrtf(RATIO);

			float numhor = x * tmpsqrt;
			float numvert = x / tmpsqrt;

			int maxhor = (int)ceilf(numhor);
			int maxvert = (int)ceilf(numvert);

			int total = maxhor * maxvert;
			int extra = total - l->sz;
			float percentextra = (float)extra / (float)total;

			vertex* varr = new vertex[total];

			float pitchstep = 180.f / (float)(maxvert + 1);
			float yawstep = 360.f / (float)(maxhor);

			float accum = 0.f;
			int pos = 0;
			int i = 0;
			int j = 0;
			l->iterate([&](ListElement<int>& a) {
				float radius = RADIUS*(calcDifff(a.val,pos,l->sz)*RADIUS_DIFF+1.f);
				vec4 col = elementIsMarked(inf, l, depthInd, &a, pos, depth) ? vec4(0.f, 0.f, 0.f, 1.f) : colHSV((float)a.val / inf->realSize, 1.f, 1.f);
				for (; i < maxvert; i++) {
					vec3 tmp = rotate(vec3(0.f, 1.f, 0.f), vec3(pitchstep * (i + 1), 0.f, 0.f));
					for (; j < maxhor; j++) {
						accum += percentextra;
						varr[i * maxhor + j] = vertex(rotate(tmp, vec3(0.f, yawstep * j, 0.f))*radius, NONORMAL, col);
						if (accum >= 1.f)
							accum -= 1.f;
						else if(pos<l->sz-1) {
							j++;
							pos++;
							return;
						}
					}
					j = 0;
				}
			});

			auto& vertices = verts->vertices;
			vertices.clear();

			//Top and Bottom
			int tmp = (maxvert - 1) * maxhor;
			for (int i = 0; i < maxhor; i++) {
				vertices += vertex(vec3(0.f, RADIUS, 0.f),NONORMAL,vec4(1.f,0.f,0.f,1.f));
				vertices += varr[i];
				vertices += varr[(i + 1) % maxhor];

				vertices += varr[tmp + i];
				vertices += vertex(vec3(0.f, -RADIUS, 0.f), NONORMAL, vec4(1.f, 0.f, 0.f, 1.f));
				vertices += varr[tmp + (i + 1) % maxhor];
			}

			//Rest
			for (int i = 0; i < total-maxhor; i += maxhor) {
				for (int j = 0; j < maxhor; j++) {
					vertices += varr[i + j];
					vertices += varr[i + maxhor + j];
					vertices += varr[i + maxhor + (j + 1) % maxhor];

					vertices += varr[i + (j + 1) % maxhor];
					vertices += varr[i + j];
					vertices += varr[i + maxhor + (j + 1) % maxhor];
				}
			}
			verts->build();
			drawShape(verts);

			return false;
		}

	}_;
}

Visualizer* visualizer_sphere_agitation = &::_;

#endif