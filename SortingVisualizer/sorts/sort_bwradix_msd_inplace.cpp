#include "../fwk/fwk_sort.h"

using uint = unsigned int;

inline uint bitseq(uint bitstep) {
	constexpr uint intbits = sizeof(int) * 8;
	return -1u >> (intbits - bitstep);
}

void bwRadixMSDInPlace_(LinkedListElement<int>* first, LinkedListElement<int>* last, uint bits, uint bitstep, int off) {
	if (off <= -(int)bitstep)
		return;

	uint realstep = off < 0 ? (uint)(off + (int)bitstep) : bitstep;
	uint numlists = 1u << realstep;
	uint mask = bitseq(realstep);
	//Create markers for the virtual lists (includes 2 virtual boundary markers, begin and end indices 0 and numlists+1 respectively)
	LinkedListElement<int>* markers = new LinkedListElement<int>[numlists+2];
	auto end = &markers[numlists + 1]; //Virtual boundary end marker

	//Not needed in PERF
	for (uint i = 0; i < numlists+2; i++) {
		markers[i].val = -1;
	}
	//

	if (off < 0)
		off = 0;

	//Position virtual markers and virtual boundary markers
	for (uint i = 0; i < numlists+1; i++) {
		markers[i].moveBefore(first);
	}
	end->moveAfter(last);

	LinkedListElement<int>* tmp = first;
	LinkedListElement<int>* tmp2;
	uint ind;
	while (tmp != end) {
		tmp2 = tmp->next();
		ind = (tmp->val >> off)& mask;
		tmp->moveBefore(&markers[ind+1]);
		tmp = tmp2;
	}

	for (uint j = 0; j < numlists+1; j++) {
		tmp = markers[j].next();
		//Make sure to not attempt to sort empty virtual lists
		if (!(tmp >= markers && tmp < markers + (numlists + 2))) {
			bwRadixMSDInPlace_(tmp, markers[j + 1].prev(), bits, bitstep, off - bitstep);
		}
	}

	delete[] markers;
}

void bwRadixMSDInPlace(LinkedList<int>& list, uint bits, uint bitstep) {
	bwRadixMSDInPlace_(list.head, list.tail, bits, bitstep, bits - bitstep);
}

namespace {
	struct _ : public LinkedSort {
		_() {
			name = "Bitwise Radix MSD Sort (In Place Linked)";
			accessQuota = 1400;
		}
		void sort(LinkedList<int>& list) {
			bwRadixMSDInPlace(list, (int)ceilf(log2f(list.sz + 1)), 3);
		}
	} _;
}
Sort* sort_bwradix_msd_inplace = &::_;