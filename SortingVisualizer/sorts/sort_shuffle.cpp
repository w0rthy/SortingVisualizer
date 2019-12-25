#include "../fwk/fwk_sort.h"

void shuffleSort(ArrayList<int>& arr, int l, int h) {
	int rnd;
	int sz = h - l;
	for (int i = l; i < h; i++) {
		rnd = rand() % sz;
		arr.swap(i, l + rnd);
	}
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Shuffle";
			accessQuota = 2000;
		}
		void sort(ArrayList<int>& arr) {
			shuffleSort(arr, 0, arr.sz);
		}
	} _;
}
Sort* sort_shuffle = &::_;