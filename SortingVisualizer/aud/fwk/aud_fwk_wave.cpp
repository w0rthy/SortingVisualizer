#include "aud_fwk_wave.h"

vector<Wave*> waves;

void mergeWaves(Wave* to, Wave* from1, Wave* from2, float bias) {
	for (int i = 0; i < AUD_WAVEFORM_TABLE_SIZE + 1; i++) {
		to->waveTable[i] = from1->waveTable[i] * (1.f - bias) + from2->waveTable[i] * bias;
	}
}