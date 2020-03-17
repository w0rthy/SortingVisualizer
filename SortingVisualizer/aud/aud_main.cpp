#include <PA/portaudio.h>
#include "aud_main.h"
#include "../common.h"
#include "aud_common.h"
#include "aud_correct.h"
#include "waves/waves.h"
#include "fwk/aud_fwk_buf.h"
#include "aud_interface.h"

//Core Audio

#if AUD_USE_INTER_BUFFER
vector<AudBufferResult> interBuffer;
#endif
vector<AudBufferResult> streams[AUD_MAX_VIRTUAL_STREAMS];
float tvec[AUD_MAX_VIRTUAL_STREAMS];
int sz;
int graceCycles = 0;
timer audTimer;
double audOutErr = 0.0;

static void aud_synthesize_init() {
	for (int i = 0; i < AUD_MAX_VIRTUAL_STREAMS; i++)
		tvec[i] = 0.f;
}

static int aud_synthesize(const void* bufferIn, void* bufferOut, unsigned long bufferSz, const PaStreamCallbackTimeInfo* tInf, PaStreamCallbackFlags flags, void* dat)
{
	//Strict enforcement of the response time
	//	 Prevents PortAudio from building many buffers in advance leading to periods of time where this function is called more than anticipated, over-consuming the IB,
	//		followed by periods of time where this function is called less than anticipated, bloating the IB and causing the consumption step to be overcalculated leading to additional starvation.
	audOutErr += ((AUD_RESPONSE_TIME*1000.0)-audTimer.interval());
	if (audOutErr >= 1.0)
		Pa_Sleep((long)(audOutErr));

	auto out = (float*)bufferOut;

	int steps = aud_getStepCount();

	int count;

	for (count = 0; count < steps; count++) {
		auto e = aud_readFromBuffer();
#if AUD_USE_INTER_BUFFER
		if (e) {
			interBuffer.push_back(std::move(e));
		}
#else
		if (e) {
			auto& strm = streams[count % AUD_MAX_NOTES_MIX];
			if (count / AUD_MAX_NOTES_MIX == 0) {
				if (!strm.empty()) {
					auto tmp = strm[strm.size() - 1];
					strm.clear();
					strm.push_back(tmp);
				}
				else
					strm.push_back(e);
			}
			strm.push_back(std::move(e));
		}
#endif
		else {
			break;
		}
	}

#if AUD_USE_INTER_BUFFER
	count = 0;
	int ibsz = interBuffer.size();
	if (ibsz >= AUD_MIN_VIRTUAL_STREAMS) {
		for (; count < ibsz && count < AUD_MAX_NOTES_PER_TICK; count++) {
			auto& e = interBuffer[count];
			auto& strm = streams[count % AUD_MAX_VIRTUAL_STREAMS];
			if (count / AUD_MAX_VIRTUAL_STREAMS == 0) {
				if (!strm.empty()) {
					auto tmp = strm[strm.size() - 1];
					strm.clear();
					strm.push_back(tmp);
				}
				else
					strm.push_back(e);
			}
			strm.push_back(std::move(e));
		}
		interBuffer.clear();
	}
#endif
	if (count > 0) {
		sz = count;
		graceCycles = 0;
	}
	else {
		graceCycles++;
		if (graceCycles > AUD_MAX_GRACE_CYCLES)
			sz = 0;
	}

	int numstreams = sz < AUD_MAX_VIRTUAL_STREAMS ? sz : AUD_MAX_VIRTUAL_STREAMS;

	if (count > 0)
		sz += numstreams;

	float mul = aud_volume / (float)numstreams;

	float bigstep = ceilf((float)(sz-numstreams) / (float)numstreams) / (float)bufferSz;
	float smlstep = floorf((float)(sz-numstreams) / (float)numstreams) / (float)bufferSz;

	

	int stepdet = sz % AUD_MAX_VIRTUAL_STREAMS; //If stream is >= to step determinate then it uses small step

	float bigpos = 0.f;
	float smlpos = 0.f;

	float bigbias = 0.f;
	float biglbias = 1.f;

	float smlbias = 0.f;
	float smllbias = 1.f;

	for (unsigned long i = 0; i < bufferSz; i++) {
		*out = 0;
		float pos = bigpos, bias = bigbias, lbias = biglbias;
		for (int j = 0; j < numstreams; j++) {
			if(j==stepdet)
				pos = smlpos, bias = smlbias, lbias = smllbias;
			float& t = tvec[j];
			AudBufferResult& e1 = streams[j][(int)pos], & e2 = streams[j][(int)pos+1];
			float tstep = e1.tstep * lbias + e2.tstep * bias;

			*out += (*e1.wave)(t, tstep) * lbias + (*e2.wave)(t, tstep) * bias;
			t += tstep;
			if (t >= 1.f)
				t -= 1.f;
		}
		*out++ *= mul;
		bigpos += bigstep;
		bigbias = bigpos - (float)(int)bigpos;
		biglbias = 1.f - bigbias;
		smlpos += smlstep;
		smlbias = smlpos - (float)(int)smlpos;
		smllbias = 1.f - smlbias;
	}

	return paContinue;
}

static void aud_onFinished(void* userData) {}

bool aud_initialized = false;
PaStream* aud_stream = nullptr;

void launchAudioThread()
{
	Pa_Initialize();

	PaStreamParameters outParams;
	outParams.device = Pa_GetDefaultOutputDevice();
	if (outParams.device == paNoDevice) {
		printf("Audio failed to initialize: No output device.\n");
		return;
	}
	outParams.channelCount = 1;
	outParams.sampleFormat = paFloat32;
	outParams.suggestedLatency = Pa_GetDeviceInfo(outParams.device)->defaultLowOutputLatency;
	outParams.hostApiSpecificStreamInfo = 0;
	
	aud_synthesize_init();
	Pa_OpenStream(&aud_stream, 0, &outParams, AUD_SAMPLE_RATE, AUD_OUT_BUFFER_SZ, paClipOff, aud_synthesize, nullptr);
	Pa_SetStreamFinishedCallback(aud_stream, &aud_onFinished);
	Pa_StartStream(aud_stream);
}

//Clean up on shutdown
namespace {
	struct _ {
		~_() {
			if (aud_initialized) {
				if (aud_stream) {
					Pa_StopStream(aud_stream);
					Pa_CloseStream(aud_stream);
				}
				Pa_Terminate();
			}
		}
	} _;
}