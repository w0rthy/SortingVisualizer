#pragma once
#include "gfx_common.h"
#if GFX_ENABLED
#include "fwk/gfx_fwk_visualizer.h"
#include "fwk/gfx_fwk_shader.h"

extern Visualizer* requestVisualizer;

void gfx_checkRequests();

#endif