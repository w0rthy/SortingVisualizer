#include "../fwk/fwk_sort.h"

void shellSort(ArrayList<int>& arr, int l, int h) {
	int gap = (h-l) / 2;
	int tmp;
	int loc;
	for (int gap = (h - l) / 2; gap > 0; gap /= 2) {
		for (int i = 0; i < gap; i++) {
			for (int j = l+i+gap; j < h; j += gap) {
				tmp = arr[j];
				loc = j - gap;
				while (loc >= l && arr[loc] > tmp) {
					arr.swap(loc, loc + gap);
					loc -= gap;
				}
			}
		}
	}
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Shell Sort";
			accessQuota = 4400;
		}
		void sort(ArrayList<int>& arr) {
			shellSort(arr, 0, arr.sz);
		}
	} _;
}
Sort* sort_shell = &::_;