#include "../fwk/fwk_sort.h"

void insertionSort(ArrayList<int>& arr, int l, int h) {
	int tmp;
	int loc;
	for (int i = l+1; i < h; i++) {
		tmp = arr[i];
		loc = i-1;
		while (loc >= l) {
			if (arr[loc] > tmp)
				arr.swap(loc, loc + 1);
			else
				break;
			loc--;
		}
	}
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Insertion Sort";
			accessQuota = 97000;
		}
		void sort(ArrayList<int>& arr) {
			insertionSort(arr, 0, arr.sz);
		}
	} _;
}
Sort* sort_insertion = &::_;