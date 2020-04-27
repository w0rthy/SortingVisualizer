#include "gfx_draw.h"
#if GFX_ENABLED
#include "fwk/gfx_fwk_visualizer.h"
#include "../fwk/fwk_control.h"
#include "../fwk/fwk_sort.h"

vector<int> depthIndices;
vector<int> depthSizes;

bool gfx_drawList(VisualizerListInfo* inf, List<int>* list, int depth = 0) {
	int depthInd = depthIndices[depth]; //Pending depth index

	if (!currentVisualizer->drawList(inf, list, depthInd, depth))
		return false;
	
	depthIndices[depth]++; //Take the index

	int off_err = depthSizes[depth]-list->offset;
	if (off_err > 0)
		list->offset += off_err;
	int i = 0;
	ListElement<int>* prev = nullptr;
	list->iterate([&](ListElement<int>& e) {
		if (e.val != -1) {
			currentVisualizer->drawElement(inf, list, depthInd, &e, i, depth, currentVisualizer->elementIsMarked(inf, list, depthInd, &e, i, depth));
			if (prev)
				currentVisualizer->drawLink(inf, list, depthInd, prev, &e, i - 1, i, depth);
			prev = &e;
		}
		i++;
		});
	if (off_err > 0)
		list->offset -= off_err;
	depthSizes[depth] += i;

	list->iterateChildren([&](ListContext<int>& c) {
		int childDepthInd = depthIndices[depth + 1]; //depth ind if child is drawn
		if (gfx_drawList(inf, c.list, depth + 1)) {
			currentVisualizer->drawListLink(inf, list, c.list, depthInd, childDepthInd, depth);
		}
		});
	return true;
}

std::string getNumString(long long a) {
	constexpr long long thresh = 10;

	constexpr long long b = 1000 * 1000 * 1000;
	constexpr long long m = 1000 * 1000;
	constexpr long long k = 1000;
	if (a >= thresh * b) {
		a /= b;
		return std::to_string(a) + "B";
	}
	else if (a >= thresh * m) {
		a /= m;
		return std::to_string(a) + "M";
	}
	else if (a >= thresh * k) {
		a /= k;
		return std::to_string(a) + "K";
	}

	return std::to_string(a);
}

void gfx_drawStatus(VisualizerListInfo* inf) {
	constexpr float hgap = 0.f;
	float vgap = (float)fontHeight(0) * 2.f / 1080.f;

	vec3 pos(-1.f+hgap, 1.f, -1.f);

	string text;
	
	text = "Visualizer: " + (currentVisualizer?currentVisualizer->name:std::string());
	drawFontString2DCorrected(0, text, pos);
	pos[1] -= vgap + 0.05f;

	text = "Sort: " + (currentSort?currentSort->name:std::string());
	drawFontString2DCorrected(0, text, pos);
	pos[1] -= vgap;

	text = "Access Function: " + (currentSort?currentSort->accessFuncStr:std::string());
	drawFontString2DCorrected(0, text, pos);
	pos[1] -= vgap;

	text = "Rank: " + (currentSort?(currentSort->ranked?std::to_string(currentSortRank + 1):std::string("Unranked")):std::string("None")) + " of " + std::to_string(profilerRanking.size());
	drawFontString2DCorrected(0, text, pos);
	pos[1] -= vgap;

	text = "Accesses: " + getNumString(state.accessCount);
	drawFontString2DCorrected(0, text, pos);
	pos[1] -= vgap;

	text = "  Per Second: " + getNumString(state.accessValuePer > 0.0 ? (1.0/(state.accessValuePer*state.accessValueMul)):0.0);
	drawFontString2DCorrected(0, text, pos);
}

timer drawTimer;

void gfx_drawFrame() {
	if (!currentVisualizer)
		return;

	VisualizerListInfo inf = computeListInfo(watchList);
	inf.realSize = watchListSize;

	//Prepare the visualizer to render
	currentVisualizer->prepare(&inf, watchList, drawTimer.intervalSeconds());

	depthIndices.clear();
	depthIndices.resize(inf.maxDepth,0);
	depthSizes.clear();
	depthSizes.resize(inf.maxDepth,0);

	gfx_drawList(&inf, watchList);

	gfx_drawStatus(&inf);
}

#endif