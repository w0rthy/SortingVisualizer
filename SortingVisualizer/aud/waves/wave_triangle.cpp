#include "waves.h"

namespace {
	struct _ : public Wave {
		_() {
			name = "Triangle Wave";

			constexpr float ampMul = (float)(1. / (AUD_PI / 2.));
			constexpr double rate = 4./(double)AUD_WAVEFORM_TABLE_SIZE;
			double offTbl[] = {0.,1.,0.,-1.};
			double rateTbl[] = { rate,-rate,-rate,rate };
			constexpr int div = AUD_WAVEFORM_TABLE_SIZE / 4;

			int tmp;
			for (int i = 0; i < AUD_WAVEFORM_TABLE_SIZE; i++) {
				tmp = i / div;
				waveTable[i] = (float)(offTbl[tmp] + (double)(i % div) * rateTbl[tmp]) * ampMul;
			}

			waveTable[AUD_WAVEFORM_TABLE_SIZE] = waveTable[0]; //For Fast Interpolation
		}
	} _;
}

Wave* wave_triangle = &::_;