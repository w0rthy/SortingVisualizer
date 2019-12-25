#include "../fwk/fwk_sort.h"

using uint = unsigned int;

inline uint bitseq(uint bitstep) {
	constexpr uint intbits = sizeof(int) * 8;
	return -1u >> (intbits - bitstep);
}

//Add dummy elements to the front of the list to act as virtual lists for the movements (might be just slightly slower than out-of-place)
void bwRadixLSDInPlaceLinked(LinkedList<int>& list, uint bits, uint bitstep) {
	uint numlists = 1u << bitstep;
	uint usedlists = numlists;
	LinkedListElement<int>* markers = new LinkedListElement<int>[numlists]; //Mark virtual list locations

	//Not necessary, disable in PERF
	for (uint i = 0; i < numlists; i++) {
		markers[i].val = -1; 
	}
	//
	uint realstep = bitstep;
	uint mask = bitseq(bitstep);
	for (uint i = 0; i < bits; i += bitstep) {
		if (i + bitstep >= bits) {
			realstep = bits - i;
			mask = bitseq(realstep);
			usedlists = 1u << realstep;
		}

		//Move dummies to the front of the list
		for (uint j = numlists; j > 0; j--) {
			list.addFront(&markers[j-1]);
		}

		LinkedListElement<int>* tmp = markers[numlists-1].next();
		LinkedListElement<int>* tmp2;
		uint ind;
		while (tmp != nullptr) {
			tmp2 = tmp->next();
			ind = (tmp->val >> i)& mask;
			tmp->moveBefore(&markers[ind]);
			tmp = tmp2;
		}
	}

	//for (uint i = 0; i < numlists; i++)
	//	markers[i].extract();

	delete[] markers;
}

namespace {
	struct _ : public LinkedSort {
		_() {
			name = "Bitwise Radix LSD Sort (In Place Linked)";
			accessQuota = 600;
		}
		void sort(LinkedList<int>& list) {
			bwRadixLSDInPlaceLinked(list, (int)ceilf(log2f(list.sz + 1)), 4);
		}
	} _;
}
Sort* sort_bwradix_lsd_inplace = &::_;