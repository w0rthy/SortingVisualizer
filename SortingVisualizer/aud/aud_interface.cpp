#include "aud_interface.h"
#include "fwk/aud_fwk_buf.h"
#include "aud_main.h"
#include "waves/waves.h"
#include "../fwk/fwk_control.h"

Wave* defaultWave = wave_dblsine;

float aud_volume = 1.f;

void playFreq(float freq, Wave* wave) {
	aud_addToBuffer(freq / (float)AUD_SAMPLE_RATE, wave ? wave : defaultWave);
}

void playSound(int val) {
	if (!watchList || val < 0)
		return;
	playFreq(400.f + (float)val / (float)watchListSize * 2000.f);
}