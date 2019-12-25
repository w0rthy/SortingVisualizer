#include "../fwk/fwk_sort.h"

void cocktailShakerSort(ArrayList<int>& arr, int l, int h) {
	int min = l, max = h;
	int tmp, tmp2;
	int strk;
	while(min<max){
		strk = 1;
		tmp = arr[min];
		for (int j = min + 1; j < max; j++) {
			tmp2 = arr[j];
			if (tmp > tmp2) {
				arr.swap(j, j - 1);
				strk = 1;
			}
			else {
				tmp = tmp2;
				strk++;
			}
		}
		max -= strk;
		if (min >= max)
			break;
		strk = 1;
		tmp = arr[max - 1];
		for (int j = max - 2; j >= min; j--) {
			tmp2 = arr[j];
			if (tmp < tmp2) {
				arr.swap(j, j + 1);
				strk = 1;
			}
			else {
				tmp = tmp2;
				strk++;
			}
		}
		min += strk;
	}
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Cocktail Shaker Sort";
			accessQuota = 105000;
		}
		void sort(ArrayList<int>& arr) {
			cocktailShakerSort(arr, 0, arr.sz);
		}
	} _;
}
Sort* sort_cocktail_shaker = &::_;