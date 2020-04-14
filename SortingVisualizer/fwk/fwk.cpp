#include "fwk.h"
#include "fwk_thread.h"

int instanceID = 0;

FrameworkState state;

timer cycleTimer;

bool FWK_canCycle() {
	state.cycle_gapTime = cycleTimer.intervalSeconds();
	state.cycle_valueLimit += state.cycle_gapTime;

	return state.accessValuePer < state.cycle_valueLimit;
}

void FWK_preCycle() {
	threadStateFWK_preCycle();

	state.cycle_accessCount = 0;
	state.cycle_accessValue = 0.0;
	
	state.cycleNumber++;
	cycleTimer.start();
}

void FWK_postCycle() {
	state.cycle_lastTime = cycleTimer.intervalSeconds();
	state.sortTime += state.cycle_lastTime;

	threadStateFWK_postCycle();

	state.cycle_valueLimit -= state.cycle_accessValue;
	state.accessCount += state.cycle_accessCount;
	cycleTimer.start();
}

void FWK_preSort() {
	threadStateFWK_preSort();

	state.accessCount = 0;
	state.sortTime = 0.0;
	state.cycle_valueLimit = 0.0;

	cycleTimer.start();
}

void FWK_postSort() {
	threadStateFWK_postSort();
}