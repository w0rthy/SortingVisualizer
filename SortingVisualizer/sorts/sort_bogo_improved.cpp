#include "../fwk/fwk_sort.h"
#include "sorts.h"

void bogoImprovedSort(ArrayList<int>& arr, int l, int h) {
	int pos = l;
	bool success;

	int tmp;
	int i;
	while (pos < h-1) {
		tmp = arr[pos];
		for (i = pos + 1; i < h; i++)
			if (arr[i] < tmp)
				break;
		if (i == h) {
			pos++;
		}
		else
			shuffleSort(arr, pos, h);
	}
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Improved Bogo Sort";
			accessQuota = 78000;

			profiled = false;
		}
		void sort(ArrayList<int>& arr) {
			bogoImprovedSort(arr, 0, arr.sz);
		}
	} _;
}
Sort* sort_bogo_improved = &::_;