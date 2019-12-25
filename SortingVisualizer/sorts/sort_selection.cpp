#include "../fwk/fwk_sort.h"

//Selection Sort (Array)
void selectionSort(ArrayList<int>& arr, int l, int h) {
	int min;
	int ind;

	int tmp;
	for (int i = l; i < h-1; i++) {
		min = -1u>>1; //Must disable C4146 Warning for this functionality
		ind = i;
		for (int j = i; j < h; j++) {
			tmp = arr[j];
			if (tmp < min) {
				min = tmp;
				ind = j;
			}
		}
		arr.swap(i, ind);
	}
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Selection Sort";
			accessQuota = 50000;
		}
		void sort(ArrayList<int>& arr) {
			selectionSort(arr, 0, arr.sz);
		}
	} _;
}
Sort* sort_selection = &::_;