#include "waves.h"

namespace {
	struct _ : public Wave {
		_() {
			name = "Saw Wave";

			constexpr float ampMul = (float)(1./(AUD_PI/2.));
			constexpr double rate = 2. / (double)AUD_WAVEFORM_TABLE_SIZE;
			double offTbl[] = { 0.,0.5,-1,-0.5 };
			constexpr int div = AUD_WAVEFORM_TABLE_SIZE / 4;

			for (int i = 0; i < AUD_WAVEFORM_TABLE_SIZE; i++) {
				waveTable[i] = (float)(offTbl[i/div] + (double)(i % div) * rate) * ampMul;
			}

			waveTable[AUD_WAVEFORM_TABLE_SIZE] = waveTable[0]; //For Fast Interpolation
		}
	} _;
}

Wave* wave_saw = &::_;