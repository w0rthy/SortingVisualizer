#pragma once
struct Wave;

extern Wave* defaultWave;

void playFreq(float freq, Wave* wave = nullptr);

void playSound(int val);

extern float aud_volume;