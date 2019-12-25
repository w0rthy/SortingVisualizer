#include "../fwk/fwk_sort.h"

//Merge Sort (Aggressive Out-of-Place)
//Extremely Aggressive Minimal-Access-Count Implemenetation
//TODO: Try using pointers instead of ArrayList references, would be interesting to see if it is faster
ArrayList<int>* mergeSortAggOOP_(ArrayList<int>& arr, int l, int h, int depth) {
	int m = (l + h) / 2;

	if (l >= m)
		if (depth != 0)
			if (h > l) {
				auto tmp = new ArrayList<int>(1);
				(*tmp)[0] = (int)arr[l];

				return tmp;
			}
			else
				return new ArrayList<int>(0);
		else
			return nullptr;

	ArrayList<int>& arr_l = *mergeSortAggOOP_(arr, l, m, depth+1);
	ArrayList<int>& arr_h = *mergeSortAggOOP_(arr, m, h, depth+1);

	ArrayList<int>* arr_to = &arr;
	if (depth != 0) {
		arr_to = new ArrayList<int>(arr_l.sz + arr_h.sz);
		arr.associateAbs(*arr_to, l); //Associate
	}

	ArrayList<int>& tmp = *arr_to;

	tmp.associateAbs(arr_l, l); //Associate
	tmp.associateAbs(arr_h, m); //Associate

	int l_ = 0;
	int h_ = 0;

	int loc = 0;
	int tmp_l = arr_l[l_];
	int tmp_h;
	bool last = false;
	while (l_ < arr_l.sz && h_ < arr_h.sz) {
		last ? tmp_l = arr_l[l_] : tmp_h = arr_h[h_];
		last = tmp_l < tmp_h;
		if (last) {
			tmp[loc] = tmp_l;
			l_++;
		}
		else {
			tmp[loc] = tmp_h;
			h_++;
		}
		loc++;
	}

	while (l_ < arr_l.sz) {
		tmp[loc] = arr_l[l_++];
		loc++;
	}

	while (h_ < arr_h.sz) {
		tmp[loc] = arr_h[h_++];
		loc++;
	}

	delete& arr_l;
	delete& arr_h;

	return &tmp;
}

void mergeSortAggOOP(ArrayList<int>& arr, int l, int h) {
	mergeSortAggOOP_(arr, l, h, 0);
}

namespace {
	struct  _ : public ArraySort {
		_() {
			name = "Merge Sort (Aggressive Out-of-Place)";
			accessQuota = 2000;
		}
		void sort(ArrayList<int>& arr) {
			mergeSortAggOOP(arr, 0, arr.sz);
		}
	} _;
}
Sort* sort_merge_agg_oop = &::_;