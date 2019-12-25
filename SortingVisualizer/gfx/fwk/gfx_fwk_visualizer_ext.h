#pragma once
#include "../gfx_common.h"
#include "../../fwk/lists.h"
#if GFX_ENABLED

//Contains potentially useful information pre-calculated about a list and its children
struct VisualizerListInfo {
	//Number of original elements
	int realSize = 0;
	//Total number of lists
	int listCount = 0;
	//Total number of elements in all associated lists
	int totalSize = 0;
	//Maximum sublist depth
	int maxDepth = 0;
	//Vector containing the number of lists at each depth
	//TODO??: Convert this to contain pointers to the lists at each depth sorted by offset
	vector<int> depthWidth = { 0 };
	//TODO: A way to uniquely id/number each list that has ever existed for this sort

};

//Create a VisualizerListInfo for the given List
VisualizerListInfo computeListInfo(List<int>* l);

inline int calcDisparity(int val, int pos, int listSize) {
	int diff = abs(val - pos - 1);
	int sz2 = listSize / 2;
	return diff>sz2?diff-sz2:diff;
}

inline float calcDisparityf(int val, int pos, int listSize) {
	return (float)calcDisparity(val, pos, listSize) / (float)listSize * 2.f;
}

#endif