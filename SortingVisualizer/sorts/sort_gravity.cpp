#include "../fwk/fwk_sort.h"
#include "sorts_util.h"

void gravitySort(ArrayList<int>& arr, int l, int h) {
	int max = analyzeMax(arr, l, h);
	int sz = h - l;

	ArrayList<int> accumulator(max);

	//Prepare accumulator
	state.accessRestricted = false;
	for (int i = 0; i < max; i++)
		accumulator[i] = 0;
	state.accessRestricted = true;

	//Not Needed in PERF
	arr.associate(accumulator);

	//Fill the accumulator
	int tmp;
	for (int i = l; i < h; i++) {
		tmp = arr[i];
		for (int j = 0; j < tmp; j++)
			accumulator[j]++;
	}

	//Writeback
	int off = 0;
	for (int i = max; i > 0 && off < sz; i--) {
		tmp = accumulator[i - 1] - off;
		for (int j = 1; j <= tmp; j++)
			arr[h - off - j] = i;
		off += tmp;
	}
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Gravity Sort";
			accessQuota = 50000;
		}
		void sort(ArrayList<int>& arr) {
			gravitySort(arr, 0, arr.sz);
		}
	} _;
}
Sort* sort_gravity = &::_;