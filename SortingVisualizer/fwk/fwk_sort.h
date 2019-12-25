#pragma once
#include "../common.h"
#include "../fwk/lists.h"

//SAMPLE SORT STRUCTURE

//namespace {
//	struct  _ : public ArraySort {
//		_() {
//			name = "Default Sort";
//			accessQuota = 50000;
//		}
//		void sort(ArrayList<int>& arr) {
//			sortFunc();
//		}
//	} _;
//}

struct Sort;

//The current/last running sort
extern Sort* currentSort;

extern vector<Sort*> sorts;

struct Sort {
	//Information about this sort
	enum class ListType {
		Array,
		Linked
	} type;

	//Name of the sort
	string name = "Default Sort";

	//Whether this sort should be profiled (false for really bad ones)
	bool profiled = true;

	//How many accesses per second the algorithm should be allowed (Determines speed)
	long long accessQuota = 0;

	Sort() {
		sorts.push_back(this);
	}

protected:
	//Function to perform actual sorting
	virtual void sort_(ArrayList<int>& arr, List<int>*& watch) = 0;
public:
	//Function to handle entire sorting process
	virtual void sort(ArrayList<int>& arr, List<int>*& watch) {
		FWK_preSort();
		state.sorting = true;
		state.token_sort();
		currentSort = this;
		if(accessQuota>0)
			state.accessValuePer = 1.0 / accessQuota;
		sort_(arr, watch);
		state.token_sort--;
		state.sorting = false;
		instanceID++; //Next sort
		FWK_postSort();
	}
};

struct ArraySort : public Sort {
	ArraySort() {
		type = ListType::Array;
	}

	virtual void sort(ArrayList<int>& arr) = 0;

	void sort_(ArrayList<int>& arr, List<int>*& watch) {
		sort(arr);
	}
};

struct LinkedSort : public Sort {
	LinkedSort() {
		type = ListType::Linked;
	}

	virtual void sort(LinkedList<int>& list) = 0;

	void sort_(ArrayList<int>& arr, List<int>*& watch) {
		LinkedList<int> list;
		//Copy form array to list
		state.accessRestricted = false;
		arr.iterate([&](ListElement<int>& a) { list += a; });
		state.accessRestricted = true;
		//Perform the sort
		watch = &list;
		sort(list);
		//Copy back to array
		state.accessRestricted = false;
		if (list.sz == arr.sz) {
			int tmp = 0;
			list.iterate([&](ListElement<int>& a) {arr[tmp++] = a; });
		}
		state.accessRestricted = true;
		watch = &arr;
	}
};