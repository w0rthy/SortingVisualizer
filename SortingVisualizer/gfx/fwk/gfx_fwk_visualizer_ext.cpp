#include "gfx_fwk_visualizer_ext.h"
#if GFX_ENABLED

//Recursive compute function
void computeListInfo_(VisualizerListInfo* inf, List<int>* l, int depth = 0) {
	inf->listCount++;
	inf->totalSize += l->sz;
	if (inf->maxDepth <= depth) {
		inf->maxDepth = depth + 1;
		inf->depthWidth.resize(inf->maxDepth,0);
		inf->depthSize.resize(inf->maxDepth,0);
	}
	inf->depthWidth[depth]++;
	inf->depthSize[depth]+=l->sz;

	l->iterateChildren([&](ListContext<int>& c) {
		computeListInfo_(inf, c.list, depth + 1);
	});
}

VisualizerListInfo computeListInfo(List<int>* l) {
	VisualizerListInfo inf;

	computeListInfo_(&inf, l);

	return inf;
}

#endif