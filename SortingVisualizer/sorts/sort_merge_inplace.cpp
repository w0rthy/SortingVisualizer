#include "../fwk/fwk_sort.h"
#include "sorts.h"
#include "sorts_util.h"

//In-Place Merging Strategies
void mergeStratStandard(ArrayList<int>& arr, int l, int m, int h) {
	while (l < m && m < h) {
		if (arr[l] > arr[m]) {
			swapTo(arr, m, l);
			m++;
		}
		l++;
	}
}

void mergeStratShaker(ArrayList<int>& arr, int l, int m, int h) {
	h--;
	bool dir = true;
	while (l!=m && m<=h) {
		if (dir) {
			if (arr[l] > arr[m]) {
				swapTo(arr, m, l);
				m++;
			}
			l++;
			dir = false;
		}
		else {
			if (arr[m - 1] > arr[h]) {
				swapTo(arr, m - 1, h);
				m--;
			}
			h--;
			dir = true;
		}
	}
}

void mergeStratWeave(ArrayList<int>& arr, int l, int m, int h) {
	int dist = h - m;
	for (int i = 0; i < dist; i++) {
		swapTo(arr, m + i, l + i * 2);
	}
	insertionSort(arr, l, h);
}

//Merge Sort In-Place
void mergeSortInPlace(ArrayList<int>& arr, int l, int h, void (*mergeStrat)(ArrayList<int>&,int,int,int)) {
	int m = (l + h) / 2;
	if (l >= m)
		return;

	mergeSortInPlace(arr, l, m, mergeStrat);
	mergeSortInPlace(arr, m, h, mergeStrat);

	mergeStrat(arr, l, m, h);
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Merge Sort (In Place: Standard)";
			accessQuota = 72000;
		}
		void sort(ArrayList<int>& arr) {
			mergeSortInPlace(arr, 0, arr.sz, mergeStratStandard);
		}
	} _;

	struct  __ : public ArraySort {
		__() {
			name = "Merge Sort (In Place: Shaker)";
			accessQuota = 72000;
		}
		void sort(ArrayList<int>& arr) {
			mergeSortInPlace(arr, 0, arr.sz, mergeStratShaker);
		}
	} __;

	struct  ___ : public ArraySort {
		___() {
			name = "Merge Sort (In Place: Weave)";
			accessQuota = 83000;
		}
		void sort(ArrayList<int>& arr) {
			mergeSortInPlace(arr, 0, arr.sz, mergeStratWeave);
		}
	} ___;
}
Sort* sort_merge_inplace_standard = &::_;
Sort* sort_merge_inplace_shaker = &::__;
Sort* sort_merge_inplace_weave = &::___;