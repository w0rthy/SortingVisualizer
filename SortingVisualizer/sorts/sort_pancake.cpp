#include "../fwk/fwk_sort.h"
#include "sorts.h"

void pancakeFlip(ArrayList<int>& arr, int l, int pos) {
	int sz = pos - l + 1;
	for (int i = 0; i < sz / 2; i++)
		arr.swap(l + i, pos - i);
}

void pancakeSort(ArrayList<int>& arr, int l, int h) {
	int sz = h - l;
	
	int max;
	int ind;
	int tmp;

	for (int i = 0; i < sz-1; i++) {
		ind = l;
		max = arr[ind];
		for (int j = l+1; j < h - i; j++) {
			tmp = arr[j];
			if (tmp > max) {
				max = tmp;
				ind = j;
			}
		}
		pancakeFlip(arr, l, ind);
		pancakeFlip(arr, l, h - i - 1);
	}
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Pancake Sort";
			accessQuota = 160000;
		}
		void sort(ArrayList<int>& arr) {
			pancakeSort(arr, 0, arr.sz);
		}
	} _;
}
Sort* sort_pancake = &::_;