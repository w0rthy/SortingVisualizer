#include "main.h"
#if MODE==MODE_VISUALIZER
//
#include "../gfx/gfx_main.h"
#include "../fwk/fwk_control.h"
#include "../aud/aud_main.h"

int main(int argc, char** argv) {
	launchControlThread();
	launchAudioThread();

	gfx_init();
}

#endif