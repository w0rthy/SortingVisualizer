#include "../fwk/fwk_sort.h"

void quickSort(ArrayList<int>& arr, int l, int h) {
	int m = (l + h) / 2;
	if (l >= m)
		return;
	
	//Partition
	int piv = arr[m];
	arr.swap(l, m);
	int l_ = l, h_ = h;
	int tmp, tmp2;
	while (true) {
		do {
			l_++;
			tmp = arr[l_];
		} while (l_ < h_ -1 && tmp < piv);
		do {
			h_--;
			tmp2 = arr[h_];
		} while (l_ < h_ && tmp2 > piv);
		if (l_ >= h_)
			break;
		arr.swap(l_, h_);
	}
	m = tmp < piv ? l_ : l_ - 1;
	arr.swap(l, m);
	//

	quickSort(arr, l, m);
	quickSort(arr, m+1, h);
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Quick Sort";
			accessQuota = 2100;
		}
		void sort(ArrayList<int>& arr) {
			quickSort(arr, 0, arr.sz);
		}
	} _;
}
Sort* sort_quick = &::_;