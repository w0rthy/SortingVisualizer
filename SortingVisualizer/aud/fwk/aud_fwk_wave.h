#pragma once
#include "../aud_correct.h"
#include "../aud_common.h"
#include "../../common.h"

#define AUD_WAVEFORM_TABLE_SIZE 1024

struct Wave;

extern vector<Wave*> waves;

struct Wave {
	string name = "Default Waveform";
	float waveTable[AUD_WAVEFORM_TABLE_SIZE+1];

	//Returns the amplitude associated with this waveform for the given time (0.0 <= t < 1.0)
	inline float operator[](float t) {
		float find = (t * (float)AUD_WAVEFORM_TABLE_SIZE);
		int ind = (int)find;
		float amt = find - ind;
		return waveTable[ind] * (1.f - amt) + waveTable[ind + 1] * amt;
	}

	//Returns a pitch corrected amplitude for this waveform given the time (0.0 <= t < 1.0)
	inline float operator()(float t, float tstep) {
		return (*this)[t] * getPitchCorrectMul(tstep*(float)AUD_SAMPLE_RATE);
	}

	Wave(){
		waves.push_back(this);
	}
};

//Merge 2 waves (from1 and from2) into wave to. bias determines how much of from2 should be used at the expense of from1
void mergeWaves(Wave* to, Wave* from1, Wave* from2, float bias = 0.5f);