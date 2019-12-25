#include "../fwk/fwk_sort.h"

using uint = unsigned int;

inline uint bitseq(uint bitstep) {
	constexpr uint intbits = sizeof(int)*8;
	return -1u >> (intbits - bitstep);
}

void bwRadixLSD(LinkedList<int>& list, uint bits, uint bitstep) {
	uint numlists = 1u << bitstep;
	uint usedlists = numlists;
	LinkedList<int>* lists = new LinkedList<int>[numlists]();

	//Associate (Disable in PERF)
	list.associate(lists[0]);
	for (uint i = 0; i < numlists-1; i++) {
		lists[i].associate(lists[i+1]);
	}
	//

	uint realstep = bitstep;
	uint mask = bitseq(bitstep);
	for (uint i = 0; i < bits; i += bitstep) {
		if (i + bitstep >= bits) {
			realstep = bits - i;
			mask = bitseq(realstep);
			usedlists = 1u<<realstep;
		}
		LinkedListElement<int>* tmp = list.head;
		LinkedListElement<int>* tmp2;
		uint ind;
		while (tmp != nullptr) {
			tmp2 = tmp->next();
			ind = (tmp->val >> i) & mask;
			lists[ind].addBack(tmp);
			tmp = tmp2;
		}

		for (uint j = 0; j < usedlists; j++) {
			lists[j].moveListAfter(&list);
		}
	}

	delete[] lists;
}

namespace {
	struct _ : public LinkedSort {
		_() {
			name = "Bitwise Radix LSD Sort (Linked)";
			accessQuota = 800;
		}
		void sort(LinkedList<int>& list) {
			bwRadixLSD(list,(int)ceilf(log2f(list.sz+1)),3);
		}
	} _;
}
Sort* sort_bwradix_lsd = &::_;