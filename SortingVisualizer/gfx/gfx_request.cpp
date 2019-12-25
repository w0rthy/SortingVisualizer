#include "gfx_request.h"
#if GFX_ENABLED

Visualizer* requestVisualizer = nullptr;

void gfx_checkRequests() {
	if (requestVisualizer && requestVisualizer != currentVisualizer)
		(*requestVisualizer)();
}

#endif