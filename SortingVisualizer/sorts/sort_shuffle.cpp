#include "../fwk/fwk_sort.h"

void shuffleSort(ArrayList<int>& arr, int l, int h) {
	//srand(time(0)); //Temporary, should implement a srand() function in common.h that automatically manages the seed
	int rnd;
	int sz = h - l;
	for (int i = 0; i < sz - 1; i++) {
		rnd = rand() % (sz-i);
		arr.swap(l + i, l + i + rnd);
	}
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Shuffle";
			ranked = false;
			accessFunc = [](int n) {return n * 3; };
			sortTime = 1.0;
		}
		void sort(ArrayList<int>& arr) {
			shuffleSort(arr, 0, arr.sz);
		}
	} _;
}
Sort* sort_shuffle = &::_;