#pragma once
#include "../aud_common.h"
#include "aud_fwk_wave.h"

#define AUD_BUFFER_SZ 1000

struct AudBufferEntry {
	float tstep; //Frequency
	Wave* wave;
	float t = 0.f;
};

struct AudBufferResult {
	float tstep;
	Wave* wave;
	float t;

	AudBufferResult() { tstep = -1.f; }

	AudBufferResult(AudBufferEntry& a) {
		tstep = a.tstep;
		wave = a.wave;
	}

	inline operator bool() { return tstep >= 0.f; }
};

extern AudBufferEntry aud_internal_buffer[];

extern int aud_ib_readLoc;
extern int aud_ib_writeLoc;
extern float aud_ib_stepSize;

int aud_getStepCount();

inline void aud_addToBuffer(float tstep, Wave* wave) {
	auto& e = aud_internal_buffer[aud_ib_writeLoc];
	e.tstep = tstep;
	e.wave = wave;

	aud_ib_writeLoc++;
	if (aud_ib_writeLoc >= AUD_BUFFER_SZ)
		aud_ib_writeLoc = 0;

	if (aud_ib_writeLoc == aud_ib_readLoc) {
		aud_ib_readLoc++;
		if (aud_ib_readLoc >= AUD_BUFFER_SZ)
			aud_ib_readLoc = 0;
	}
}

//Returns the next entry from the buffer. Evaluate the return value as a bool to check for success.
inline AudBufferResult aud_readFromBuffer() {
	int rdloc = aud_ib_readLoc;
	if (rdloc == aud_ib_writeLoc)
		return AudBufferResult();
	if (rdloc >= AUD_BUFFER_SZ)
		rdloc = 0;
	auto e = AudBufferResult(aud_internal_buffer[rdloc]);
	aud_ib_readLoc++;
	if (aud_ib_readLoc >= AUD_BUFFER_SZ)
		aud_ib_readLoc = 0;
	return e;
}