#include "waves.h"

namespace {
	struct _ : public Wave {
		_() {
			name = "Double Sine Wave";

			double mulTbl[] = { 1.,-1.,-1.,1. };
			constexpr int div = AUD_WAVEFORM_TABLE_SIZE / 4;

			for (int i = 0; i < AUD_WAVEFORM_TABLE_SIZE; i++) {
				waveTable[i] = (float)(sin((double)i / (double)AUD_WAVEFORM_TABLE_SIZE * AUD_PI * 4.)*mulTbl[i/div]);
			}

			waveTable[AUD_WAVEFORM_TABLE_SIZE] = waveTable[0]; //For Fast Interpolation
		}
	} _;
}

Wave* wave_dblsine = &::_;