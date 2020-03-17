#include "aud_interface.h"
#include "fwk/aud_fwk_buf.h"
#include "aud_main.h"
#include "waves/waves.h"
#include "../fwk/fwk_control.h"

Wave* defaultWave = wave_square;

float aud_volume = 1.f;

int pitchArraySz = 0;
float* pitchArray = nullptr;

void rebuildPitchTable() {
	if (watchListSize == pitchArraySz)
		return;

	if (pitchArray != nullptr)
		delete[] pitchArray;
	pitchArraySz = watchListSize;
	pitchArray = new float[pitchArraySz];

	double tmp = (double)AUD_BASE_PITCH;

	//Determine exponential rate
#if AUD_PITCH_BOUND_BY_OCTAVES
	double rate = pow(pow(2., (double)AUD_PITCH_NUM_OCTAVES),1./(double)(pitchArraySz-1));
#else
	double rate = pow((double)AUD_PITCH_MAX_FREQ / tmp,1./(double)(pitchArraySz - 1));
#endif
	//Fill table
	for (int i = 0; i < pitchArraySz; i++) {
		pitchArray[i] = tmp;
		tmp = tmp * rate;
	}
}

inline void playFreq(float freq, Wave* wave) {
	aud_addToBuffer(freq / (float)AUD_SAMPLE_RATE, wave ? wave : defaultWave);
}

void playSound(int val) {
	val = val - 1; //Correct for range
	if (!watchList || val < 0)
		return;
	//Rebuild pitch table if necessary
	rebuildPitchTable();

	playFreq(pitchArray[val]);
	//playFreq(400.f + (float)val / (float)watchListSize * 2000.f);
}