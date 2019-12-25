#include "../fwk/fwk_sort.h"

//Merge Sort (Aggressive Out-of-Place)
//Extremely Aggressive Minimal-Access-Count Implemenetation
//TODO: Try using pointers instead of ArrayList references, would be interesting to see if it is faster
void mergeSortOOP(ArrayList<int>& arr, int l, int h) {
	int m = (l + h) / 2;

	if (l >= m)
		return;

	mergeSortOOP(arr, l, m);
	mergeSortOOP(arr, m, h);

	ArrayList<int>& arr_tmp = *new ArrayList<int>(h - l);

	arr.associateAbs(arr_tmp, l); //Associate

	int l_ = l;
	int h_ = m;

	int loc = 0;
	int tmp_l = arr[l_];
	int tmp_h;
	bool last = false;
	while (l_ < m && h_ < h) {
		last ? tmp_l = arr[l_] : tmp_h = arr[h_];
		last = tmp_l < tmp_h;
		if (last) {
			arr_tmp[loc] = tmp_l;
			l_++;
		}
		else {
			arr_tmp[loc] = tmp_h;
			h_++;
		}
		loc++;
	}

	while (l_ < m) {
		arr_tmp[loc] = arr[l_++];
		loc++;
	}

	while (h_ < h) {
		arr_tmp[loc] = arr[h_++];
		loc++;
	}

	for (int i = 0; i < arr_tmp.sz; i++) {
		arr[l + i] = arr_tmp[i];
	}

	delete& arr_tmp;
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Merge Sort (Out-of-Place)";
			accessQuota = 3000;
		}
		void sort(ArrayList<int>& arr) {
			mergeSortOOP(arr, 0, arr.sz);
		}
	} _;
}
Sort* sort_merge_oop = &::_;