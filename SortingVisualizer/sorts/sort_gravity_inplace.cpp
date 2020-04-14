#include "../fwk/fwk_sort.h"
#include "sorts_util.h"

void gravitySortInPlace(ArrayList<int>& arr, int l, int h) {
	int max = analyzeMax(arr, l, h);
	int sz = h - l;

	ArrayList<int>** abacus = new ArrayList<int>*[sz];
	for (int i = 0; i < sz; i++)
		abacus[i] = new ArrayList<int>(max);

	//Fill abacus
	for (int i = 0; i < sz; i++) {
		int& tmp = arr[l + i];
		for (int j = 0; j < tmp; j++)
			(*abacus[i])[j] = -1;
		//Fill rest with zeroes
		for (int j = tmp; j < max; j++) {
			(*abacus[i])[j] = 0;
		}
	}

	for (int i = max - 1; i >= 0; i--) {
		int sum = 0;
		for (int j = 0; j < sz; j++) {
			if ((*abacus[j])[i]) {
				sum++;
				(*abacus[j])[i] = 0;
				arr[l + j]--;
			}
		}
		for (int j = sz - 1; j >= sz - sum; j--) {
			(*abacus[j])[i] = -1;
			arr[l + j]++;
		}
	}
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Gravity Sort (In Place)";
			accessQuota = 400400;
		}
		void sort(ArrayList<int>& arr) {
			gravitySortInPlace(arr, 0, arr.sz);
		}
	} _;
}
Sort* sort_gravity_inplace = &::_;