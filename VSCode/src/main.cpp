#define _DEBUG

// #define SIMPLE_LOW_HIGH_PASS_FILTER

// #define AUTO_WAH_EFFECT
// #define CHORUS_EFFECT
// #define DISTORTION_EFFECT
// #define FLANGER_EFFECT
// #define NOISE_GATE_EFFECT
#define OVERDRIVE_EFFECT
// #define PHASER_EFFECT
// #define REVERB_EFFECT
// #define TREMOLO_EFFECT
// #define WAH_EFFECT

// #define LOOPER_EFFECT
// #define COMPRESSOR_EFFECT
// #define SUSTAIN_EFFECT
// #define TEST_EFFECT

// #define SINE_WAVE_PLAYER

#include "../include/Application.h"

int main()
{
	Application app;

	app.Initialize();

	while (true)
	{
		app.Delay(1);

		app.Update();
	}

	return 0;
}