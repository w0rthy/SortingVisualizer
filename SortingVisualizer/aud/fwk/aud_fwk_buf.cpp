#include "aud_fwk_buf.h"
#include "../../fwk/fwk.h"

AudBufferEntry aud_internal_buffer[AUD_BUFFER_SZ];

int aud_ib_readLoc = 0;
int aud_ib_writeLoc = 0;
float aud_ib_stepSize = 0.f;

long long lastCalcCycle = 0;

float steperr = 0.f;


int aud_getStepCount() {
	int cycle = state.cycleNumber;
#if AUD_IB_ALWAYS_CALCULATE_CYCLE_CONSUMPTION
	if (true) {
#else
	if (lastCalcCycle < cycle || !aud_ib_stepSize) {
#endif
		//Calculate new stepsize
		double gapTime = state.cycle_gapTime + state.cycle_lastTime;
		float sz = (float)(aud_ib_writeLoc - aud_ib_readLoc);
		if (sz < 0.f)
			sz += (float)AUD_BUFFER_SZ;

		sz *= AUD_IB_CYCLE_CONSUMPTION_MUL;

		aud_ib_stepSize = fminf((float)((double)sz / (gapTime / AUD_RESPONSE_TIME)),(float)sz);
		lastCalcCycle = cycle;
	}
	//Calculate step size
	int step = (int)aud_ib_stepSize;
	steperr += aud_ib_stepSize - (float)step;
	if (steperr >= 1.f) {
		int tmp = (int)steperr;
		step += tmp;
		steperr -= (float)tmp;
	}
	return step;
}