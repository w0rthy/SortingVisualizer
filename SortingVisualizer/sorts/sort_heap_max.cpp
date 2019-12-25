#include "../fwk/fwk_sort.h"

void percDwn(ArrayList<int>& arr, int loc, int l, int h, bool max = true) {
	int tmp = arr[loc];
	int c1, c2;
	int v1, v2;
	while (true) {
		c1 = l + (loc - l) * 2 + 1;
		c2 = c1 + 1;
		if (c1 >= h)
			break;
		v1 = arr[c1];
		if (c2 < h) {
			v2 = arr[c2];
			if (v2 > v1) {
				c1 = c2;
				v1 = v2;
			}
		}
		if (v1 <= tmp)
			break;
		arr.swap(loc, c1);
		loc = c1;
	}
}

void heapSortMax(ArrayList<int>& arr, int l, int h) {
	int sz = h - l;
	//Heapify
	for (int i = l + (sz - 2) / 2; i >= l; i--) {
		percDwn(arr, i, l, h);
	}
	//Sort
	int h_;
	for (int i = 1; i < h; i++) {
		h_ = h - i;
		arr.swap(l, h_);
		percDwn(arr, l, l, h_);
	}
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Heap Sort (Max)";
			accessQuota = 4500;
		}
		void sort(ArrayList<int>& arr) {
			heapSortMax(arr, 0, arr.sz);
		}
	} _;
}
Sort* sort_heap_max = &::_;