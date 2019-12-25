#include "main.h"
#if MODE==MODE_PROFILER
//
#include "../fwk/fwk.h"
#include "../profiler/profiler_main.h"

int main(int argc, char** argv) {
	//Disable sort thread cycling
	state.accessValueMul = 0.0;

	runProfiler();
}

#endif