#pragma once
#include "../fwk/lists.h"

inline void swapTo(ArrayList<int>& arr, int pos, int dest) {
	int dir = pos < dest ? 1 : -1;

	int tmp;
	while (pos != dest) {
		tmp = pos + dir;
		arr.swap(pos, tmp);
		pos = tmp;
	}
}

inline int analyzeMax(ArrayList<int>& arr, int l, int h) {
	int max = arr[l];

	int tmp;
	for (int i = l + 1; i < h; i++) {
		tmp = arr[i];
		if (tmp > max)
			max = tmp;
	}

	return max;
}