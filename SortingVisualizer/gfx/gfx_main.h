#pragma once
#include "../core/main.h"
#if MODE==MODE_VISUALIZER
#define GFX_ENABLED 1
#endif

#if GFX_ENABLED
#include "../common.h"
#include "gfx_common.h"

extern int scrw, scrh;
extern float scraspect;

void gfx_init(int w = 640, int h = 480, string title = "Sorting Visualizer");

#endif