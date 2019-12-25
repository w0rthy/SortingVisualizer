#include "../fwk/fwk_sort.h"

//Selection Sort (Linked)
void selectionSort(LinkedListElement<int>* head, LinkedListElement<int>* tail) {
	int min;
	LinkedListElement<int>* ind;
	LinkedListElement<int>* end = tail->next();

	int tmp;
	for (auto i = head; i != tail; i = i->next()) {
		min = -1u >> 1; //Must disable C4146 Warning for this functionality
		ind = i;
		for (auto j = i; j != end; j = j->next()) {
			tmp = j->val;
			if (tmp < min) {
				min = tmp;
				ind = j;
			}
		}
		ind->swapVal(i);
	}
}

namespace {
	struct _ : public LinkedSort {
		_() {
			name = "Selection Sort (Linked)";
			accessQuota = 50000;
		}
		void sort(LinkedList<int>& list) {
			selectionSort(list.head, list.tail);
		}
	} _;
}
Sort* sort_selection_linked = &::_;