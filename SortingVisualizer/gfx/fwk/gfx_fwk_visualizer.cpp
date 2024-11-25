#include "gfx_fwk_visualizer.h"
#if GFX_ENABLED
#include "../visualizers/visualizers.h"

vector<Visualizer*> visualizers;

Visualizer* currentVisualizer = visualizer_2d_composite;

#endif