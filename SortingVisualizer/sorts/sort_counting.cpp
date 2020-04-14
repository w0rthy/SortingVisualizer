#include "../fwk/fwk_sort.h"
#include "sorts_util.h"

//For visual clarity
#define AUX_HEIGHT_DIV 1

void countingSort(ArrayList<int>& arr, int l, int h) {
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
	for (int i = l; i < h; i++) {
		accumulator[arr[i] - 1]+=1;
	}

	//Writeback
	int tmp;
	int off = 0;
	for (int i = 1; i <= max && off < sz; i++) {
		tmp = accumulator[i - 1];
		for (int j = 0; j < tmp; j++)
			arr[l + off + j] = i;
		off += tmp;
	}
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Counting Sort";
			accessQuota = 500;
		}
		void sort(ArrayList<int>& arr) {
			countingSort(arr, 0, arr.sz);
		}
	} _;
}
Sort* sort_counting = &::_;