#include "../fwk/fwk_sort.h"

//Selection Sort (Linked)
void quickSortLinked_(LinkedListElement<int>* start, LinkedListElement<int>* end) {
	auto pivot = start->next();

	if (pivot == end)
		return;

	int pivval = pivot->val;

	auto tmp = pivot->next();
	LinkedListElement<int>* tmp2;
	while (tmp != end) {
		tmp2 = tmp->next();
		if(tmp->val < pivval)
			tmp->moveBefore(pivot);
		tmp = tmp2;
	}

	quickSortLinked_(start, pivot);
	quickSortLinked_(pivot, end);
}

void quickSortLinked(LinkedListElement<int>* start, LinkedListElement<int>* end) {
	LinkedListElement<int>* markers = new LinkedListElement<int>[2];
	markers[0].val = -1;
	markers[1].val = -1;
	markers[0].moveBefore(start);
	markers[1].moveAfter(end);
	quickSortLinked_(&markers[0], &markers[1]);

	delete[] markers;
}

namespace {
	struct _ : public LinkedSort {
		_() {
			name = "Quick Sort (Linked)";
			accessQuota = 2100;
		}
		void sort(LinkedList<int>& list) {
			quickSortLinked(list.head, list.tail);
		}
	} _;
}
Sort* sort_quick_linked = &::_;