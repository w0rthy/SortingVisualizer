#include "main.h"

#if MODE==MODE_SORTCHECK
//
#include <cstdio>
#include <iostream> //DELETE
#include "../fwk/lists.h"
#include "../sorts/sorts.h"

void shuffle(ArrayList<int>& arr) {
	for (int i = 0; i < arr.sz; i++)
		arr.swap(i, rand() % arr.sz);
}

bool checkSorted_(ArrayList<int>& arr) {
	for (int i = 0; i < arr.sz-1; i++) {
		if (arr[i] > arr[i + 1])
			return false;
	}
	return true;
}

double timeFunc(function<void()> f) {
	auto start = std::chrono::high_resolution_clock::now();
	f();
	auto end = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1000000.0;
}

int main(int argc, char* argv) {
	srand(time(0));

	//Disable sort thread cycling
	state.accessValueMul = 0.0;

	//Make an array to test with
	ArrayList<int> arr(1000);
	List<int>* w = &arr;

	//Set it up
	state.accessRestricted = false;
	for (int i = 0; i < arr.sz; i++)
		arr[i] = i+1;
	shuffle(arr);

	printf("[Beginning test of all sorting algorithms]\n");
	printf("#######################################\n");

	vector<Sort*> failures;

	for (auto& sort : sorts) {
		if (!sort->profiled)
			continue;

		state.accessRestricted = true;
		double time = timeFunc([&] { sort->sort(arr, w); });
		state.accessRestricted = false;

		bool success = checkSorted_(arr);
		if (!success)
			failures.push_back(sort);
		printf("%s\n  %s\n  %lld accesses\n  %.2f millis\n", sort->name.c_str(), success ? "Pass" : "!FAIL!", state.accessCount, time);

		shuffle(arr);
	}

	printf("#######################################\n");
	printf("[All tests completed with %llu failures]\n",failures.size());
	for (auto i : failures)
		printf("  Fail: %s\n", i->name.c_str());

}

#endif