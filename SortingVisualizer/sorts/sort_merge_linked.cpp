#include "../fwk/fwk_sort.h"

//Selection Sort (Linked)
LinkedListElement<int>* mergeSortLinked_(LinkedListElement<int>* start, LinkedListElement<int>* r, LinkedListElement<int>* end, int range) {
	LinkedListElement<int>* tmp = r->prev(), * tmp2 = r->next();

	int subrange = 1;
	while (subrange < range && tmp2 != end) {
		tmp2 = mergeSortLinked_(tmp->next(), tmp2, end, subrange);
		subrange *= 2;
	}
	r = tmp->next();


	int lrange = range;
	while (range > 0 && lrange > 0 && r != end) {
		if (r->val < start->val) {
			tmp = r->next();
			r->moveBefore(start);
			r = tmp;
			range--;
		}
		else {
			start = start->next();
			lrange--;
		}
	}
	while (range > 0 && r != end) {
		r = r->next();
		range--;
	}
	return r;
}

void mergeSortLinked(LinkedListElement<int>* start, LinkedListElement<int>* end) {
	LinkedListElement<int>* markers = new LinkedListElement<int>[2];
	markers[0].val = -1;
	markers[1].val = -1;
	markers[0].moveBefore(start);
	markers[1].moveAfter(end);

	auto pos = start->next();
	int range = 1;
	while ((pos = mergeSortLinked_(markers[0].next(), pos, &markers[1], range)) != &markers[1]) {
		range *= 2;
	}

	delete[] markers;
}

namespace {
	struct _ : public LinkedSort {
		_() {
			name = "Merge Sort (Linked)";
			accessQuota = 1800;
		}
		void sort(LinkedList<int>& list) {
			mergeSortLinked(list.head, list.tail);
		}
	} _;
}
Sort* sort_merge_linked = &::_;