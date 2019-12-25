#include "../fwk/fwk_sort.h"

void bubbleSort(ArrayList<int>& arr, int l, int h) {
	int tmp, tmp2;
	for (int i = l; i < h-1; i++) {
		tmp = arr[l];
		for (int j = l+1; j < h - (i - l); j++) {
			tmp2 = arr[j];
			if (tmp > tmp2) {
				arr.swap(j - 1, j);
			}
			else
				tmp = tmp2;
		}
	}
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Bubble Sort";
			accessQuota = 120000;
		}
		void sort(ArrayList<int>& arr) {
			bubbleSort(arr, 0, arr.sz);
		}
	} _;
}
Sort* sort_bubble = &::_;