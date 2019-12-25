#include "waves.h"

namespace {
	struct _ : public Wave {
		_() {
			name = "Square Wave";

			constexpr float ampMul = (float)(1. / AUD_PI);
			constexpr double rate = 4. / (double)AUD_WAVEFORM_TABLE_SIZE;
			double offTbl[] = { 1.,1.,-1.,-1. };
			constexpr int div = AUD_WAVEFORM_TABLE_SIZE / 4;

			for (int i = 0; i < AUD_WAVEFORM_TABLE_SIZE; i++) {
				waveTable[i] = (float)(offTbl[i/div]) * ampMul;
			}

			waveTable[AUD_WAVEFORM_TABLE_SIZE] = waveTable[0]; //For Fast Interpolation
		}
	} _;
}

Wave* wave_square = &::_;