#pragma once
#include "lists.h"

//The main list to be used when visualizing
extern List<int>* watchList;
extern int watchListSize;

extern thread controlThread;

void launchControlThread();
