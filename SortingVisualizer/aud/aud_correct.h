#pragma once

#define AUD_PITCH_CRCT_MAX_FREQ 3400
#define AUD_PITCH_CRCT_FREQ_STEP 10
#define AUD_PITCH_CRCT_TBL_SIZE AUD_PITCH_CRCT_MAX_FREQ/AUD_PITCH_CRCT_FREQ_STEP

extern float aud_pitch_correction_table[AUD_PITCH_CRCT_TBL_SIZE];

inline float getPitchCorrectMul(float freq) {
	int ind = (int)freq / AUD_PITCH_CRCT_FREQ_STEP;
	if (ind >= AUD_PITCH_CRCT_TBL_SIZE)
		ind = AUD_PITCH_CRCT_TBL_SIZE - 1;
	return aud_pitch_correction_table[ind];
}