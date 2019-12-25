#include "fwk_thread.h"
#include "fwk.h"

FrameworkThreadState threadStates[MAX_SORT_THREADS];

thread_local int threadId = -1;

int threadCount = 0;
bool multiThreading = false;

mutex mtx_threadFwk;
FrameworkThreadState& threadState() {
	if (threadId < 0) {
		mtx_threadFwk.lock();
		for (int i = 0; i < MAX_SORT_THREADS; i++) {
			if (!threadStates[i].inUse) {
				threadStates[i].inUse = true;
				threadId = i;
				break;
			}
			threadId = MAX_SORT_THREADS - 1;
		}
		multiThreading = ++threadCount > 1;
		mtx_threadFwk.unlock();
	}
	return threadStates[threadId];
}

void threadStateFWK_preCycle() {
	for (int i = 0; i < MAX_SORT_THREADS; i++) {
		auto& thr = threadStates[i];
		thr.cycle_accessCount = 0;
	}
}

void threadStateFWK_postCycle() {
	for (int i = 0; i < MAX_SORT_THREADS; i++) {
		auto& thr = threadStates[i];
		state.cycle_accessCount += thr.cycle_accessCount;
		thr.accessCount += thr.cycle_accessCount;
	}
}

void threadStateFWK_preSort() {
	for (int i = 0; i < MAX_SORT_THREADS; i++) {
		auto& thr = threadStates[i];
		thr.accessCount = 0;
	}
}

void threadStateFWK_postSort() {

}

//Automatically remove a threadState from use
namespace {
	struct _ {
		~_() {
			if (threadId != -1) {
				mtx_threadFwk.lock();
				threadStates[threadId].inUse = false;
				multiThreading = --threadCount > 1;
				mtx_threadFwk.unlock();
			}
		}
	} thread_local _;
}