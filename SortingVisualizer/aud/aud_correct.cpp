#include "aud_correct.h"

float aud_pitch_correction_table[AUD_PITCH_CRCT_TBL_SIZE];

inline constexpr float sqr(float a) { return a * a; }

inline constexpr float correct(float freq) {
	float a = 1.f;
	if (freq < 200.f)
		return a;
	//Correct 200-400
	a = a / 2.f;
	if (freq < 400.f)
		return (1 / (freq / 400.f)) * a;
	//Correct 600-1500
	if (freq < 600.f)
		return a;
	if (freq < 1500.f)
		return (sqr((freq - 1050.f) / 450.f) * 0.7f + 0.3f) * a;
	//Correct 1800-3400
	if (freq < 1800.f)
		return a;
	if (freq < 3400.f)
		return (sqr((freq - 2600.f) / 800.f) * 0.6f + 0.4f) * a;
	return a;
}

//Construct table
namespace {
	struct _ {
		constexpr _() {
			for (int i = 0; i < AUD_PITCH_CRCT_TBL_SIZE; i++) {
				aud_pitch_correction_table[i] = correct(i * AUD_PITCH_CRCT_FREQ_STEP);
			}
		}
	} _;
}