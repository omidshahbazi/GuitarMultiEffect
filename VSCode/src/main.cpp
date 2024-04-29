#undef _DEBUG
#define WAIT_FOR_DEBUGGER

#define AUTO_WAH_EFFECT
#define CHORUS_EFFECT
#define DISTORTION_EFFECT
#define FLANGER_EFFECT
#define LOOPER_EFFECT
#define OVERDRIVE_EFFECT
#define PHASER_EFFECT
#define REVERB_EFFECT
#define TREMOLO_EFFECT
#define WAH_EFFECT
#define TEST_EFFECT

// #define SINE_WAVE_PLAYER

#include "../include/Application.h"

static constexpr uint32 TOTAL_SDRAM_SIZE = 64 * 1024 * 1024;
uint8 DSY_SDRAM_BSS m_SDRAMBuffer[TOTAL_SDRAM_SIZE];

int main()
{
	Application app(m_SDRAMBuffer, TOTAL_SDRAM_SIZE);

	app.Initialize();

	while (true)
	{
		// TODO: The CPU gets hot not that much but I guess by limiting the process time through this loop, it wouldn't get that hot anymore, try it out
		app.Delay(1);

		app.Update();
	}

	return 0;
}

// TODO: Test all of the effects on DevBoard and separate the not working ones