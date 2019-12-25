#pragma once
#include "../common.h"
#include <thread>

#define MAX_SORT_THREADS 16

struct FrameworkThreadState {
	bool inUse = false;
	//Access value multiplier for this thread
	double accessValueMul = 1.0;
	//Number of accesses this sort
	long long accessCount = 0;
	//Number of accesses this cycle
	long long cycle_accessCount = 0;
};

extern FrameworkThreadState threadStates[];

//Current id of this thread
extern thread_local int threadId;

//Number of threads currently active
extern int threadCount;

FrameworkThreadState& threadState();

void threadStateFWK_preCycle();

void threadStateFWK_postCycle();

void threadStateFWK_preSort();

void threadStateFWK_postSort();