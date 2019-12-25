#include "fwk.h"
#include "fwk_thread.h"

int instanceID = 0;

FrameworkState state;

timer cycleTimer;

double el = 0.0;

void FWK_preCycle() {
	threadStateFWK_preCycle();

	state.cycle_accessCount = 0;
	state.cycle_accessValue = 0.0;
	state.cycle_gapTime = cycleTimer.intervalSeconds();
	state.cycle_valueLimit = state.cycle_gapTime;
	el += state.cycle_valueLimit;
	
	state.cycleNumber++;
	cycleTimer.start();
}

void FWK_postCycle() {
	state.cycle_lastTime = cycleTimer.intervalSeconds();
	state.sortTime += state.cycle_lastTime;

	threadStateFWK_postCycle();

	state.accessCount += state.cycle_accessCount;
	cycleTimer.start();
}

void FWK_preSort() {
	threadStateFWK_preSort();
	
	//DEBUG
	/*state.token_control.onActionBegin = [] {printf("Token Begin\n"); };
	state.token_control.onActionEnd = [] {printf("Token End\n"); };
	state.token_draw.onActionBegin = [] {printf("Draw Begin\n"); };
	state.token_draw.onActionEnd = [] {printf("Draw End\n"); };
	state.token_sort.onActionBegin = [&]() {FWK_preCycle(); printf("Sort Begin\n"); };
	state.token_sort.onActionEnd = [&]() {
		FWK_postCycle(); printf("Sort End\n"); 
	};*/

	state.accessCount = 0;
	state.sortTime = 0.0;

	cycleTimer.start();
}

void FWK_postSort() {
	threadStateFWK_postSort();
}