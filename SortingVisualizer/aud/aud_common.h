#pragma once

#define AUD_SAMPLE_RATE 44100

//Controls how responsive the audio system is (lower is more responsive)
#define AUD_WISH_RESPONSE_TIME 0.001

//Maximum number of virtual streams to mix simultaneously
#define AUD_MAX_VIRTUAL_STREAMS 8
//Minimum number of virtual streams to be mixed simulatneously
//	Uses an intermediary buffer to intercept notes from the IB until a sufficient amount to feed the minimum number of streams has been obtained
#define AUD_MIN_VIRTUAL_STREAMS AUD_MAX_VIRTUAL_STREAMS
//Amount of time to forcibly prevent playing no sound when starved of accesses from the IB by playing the last synthesized buffer
#define AUD_MAX_GRACE_TIME 0.05

//How much of the internal access buffer to target for consumption over the course of an interpolated cycle
//	Low values increases resiliance of audio quality against abnormally long cycles but introduces a small delay
#define AUD_IB_CYCLE_CONSUMPTION_MUL 1.0
//Whether to always calculate the target step count for consuming the internal access buffer every call
//	Also improves resiliance of audio quality against abnormally long cycles, but warps the audio's relative time to be non-linear and sporadic
#define AUD_IB_ALWAYS_CALCULATE_CYCLE_CONSUMPTION 0

//
//NON-ADJUSTABLE
//
#define AUD_USE_INTER_BUFFER AUD_MIN_VIRTUAL_STREAMS>1

#define AUD_OUT_BUFFER_SZ (int)(AUD_SAMPLE_RATE*AUD_WISH_RESPONSE_TIME)

#define AUD_RESPONSE_TIME ((double)AUD_OUT_BUFFER_SZ/(double)AUD_SAMPLE_RATE)

#define AUD_MAX_GRACE_CYCLES ((int)(((double)AUD_MAX_GRACE_TIME-0.00001)/(double)AUD_RESPONSE_TIME)+1)