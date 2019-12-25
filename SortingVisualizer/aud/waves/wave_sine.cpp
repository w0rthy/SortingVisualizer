#include "waves.h"

namespace {
	struct _ : public Wave {
		_() {
			name = "Sine Wave";

			for (int i = 0; i < AUD_WAVEFORM_TABLE_SIZE; i++) {
				waveTable[i] = (float)sin((double)i / (double)AUD_WAVEFORM_TABLE_SIZE * AUD_PI * 2.);
			}

			waveTable[AUD_WAVEFORM_TABLE_SIZE] = waveTable[0]; //For Fast Interpolation
		}
	} _;
}

Wave* wave_sine = &::_;