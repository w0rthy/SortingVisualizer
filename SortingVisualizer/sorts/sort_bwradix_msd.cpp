#include "../fwk/fwk_sort.h"

using uint = unsigned int;

inline uint bitseq(uint bitstep) {
	constexpr uint intbits = sizeof(int) * 8;
	return -1u >> (intbits - bitstep);
}

void bwRadixMSD_(LinkedList<int>& list, uint bits, uint bitstep, int off) {
	if (off<=-(int)bitstep)
		return;

	uint realstep = off < 0 ? (uint)(off+(int)bitstep) : bitstep;
	uint numlists = 1u << realstep;
	uint mask = bitseq(realstep);
	LinkedList<int>* lists = new LinkedList<int>[numlists]();

	//Associate (Disable in PERF)
	for (uint i = 0; i < numlists; i++) {
		list.associateRel(lists[i],(i*list.sz)/numlists);
	}
	//
	
	if (off < 0)
		off = 0;

	LinkedListElement<int>* tmp = list.head;
	LinkedListElement<int>* tmp2;
	uint ind;
	while (tmp != nullptr) {
		tmp2 = tmp->next();
		ind = (tmp->val >> off)& mask;
		lists[ind].addBack(tmp);
		tmp = tmp2;
	}

	for (uint j = 0; j < numlists; j++) {
		bwRadixMSD_(lists[j], bits, bitstep, off-bitstep);
		lists[j].moveListAfter(&list);
	}

	delete[] lists;
}

void bwRadixMSD(LinkedList<int>& list, uint bits, uint bitstep) {
	bwRadixMSD_(list, bits, bitstep, bits - bitstep);
}

namespace {
	struct _ : public LinkedSort {
		_() {
			name = "Bitwise Radix MSD Sort (Linked)";
			accessQuota = 580;
		}
		void sort(LinkedList<int>& list) {
			bwRadixMSD(list, (int)ceilf(log2f(list.sz + 1)), 3);
		}
	} _;
}
Sort* sort_bwradix_msd = &::_;