#pragma once
#include "../common.h"
#include "fwk_thread.h"
#include "fwk_synch.h"

//The current instance id (to distinguish between different sort runs)
extern int instanceID;

//Called to consider beginning a cycle
bool FWK_canCycle();

//Called directly before a cycle begins
void FWK_preCycle();

//Called directly after a cycle ends
void FWK_postCycle();

//Called directly before a sort begins
void FWK_preSort();

//Called directly after a sort ends
void FWK_postSort();

struct FrameworkState {
	//Setup token system
	TokenSystem token_system;
	TokenState token_draw = (&token_system);
	TokenState token_control = (&token_system);
	TokenState token_sort = (&token_system);

	//Used to safely modify the state
	sortMutex mtx_state;
	//Whether accesses count towards the access quota
	bool accessRestricted = true;
	//Used in conjunction with mtx_sort to speed up onAccess()
	bool sorting = false;
	//Access value of each access
	double accessValuePer = 0.0;
	//Global multiplier for accessValue
	double accessValueMul = 1.0;
	//Current amount of accesses performed
	long long accessCount = 0;
	//Current cycle number
	long long cycleNumber = 0;
	//Total number of time elapsed during this sort
	double sortTime = 0.0;
	//Amount of accesses performed last cycle
	long long cycle_accessCount = 0;
	//Amount of access value currently obtained in this cycle
	double cycle_accessValue = 0.0;
	//Amount of value to target for this cycle
	double cycle_valueLimit = 0.0;
	//Time elapsed between cycles
	double cycle_gapTime = 0.0;
	//Time elapsed during the last cycle
	double cycle_lastTime = 0.0;

	FrameworkState() {
		//Finish token system setup
		token_draw.nxt = &token_control;
		token_control.nxt = &token_sort;
		token_sort.nxt = &token_draw;

		token_sort.intentFunc = [&]() {return FWK_canCycle(); };
		token_sort.onActionBegin = [&]() {FWK_preCycle(); };
		token_sort.onActionEnd = [&]() {FWK_postCycle(); };
	}
};

extern FrameworkState state;