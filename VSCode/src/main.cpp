#define USE_BREAK_ONLY_ASSERT
// #define WAIT_FOR_DEBUGGER
#define USE_DISPLAY

#ifdef USE_SRAM
#define ENABLE_TYPE_CHECK
#else
#undef USE_DISPLAY
#endif

#define ADD_FX_EFFECT
#define ADD_DS_EFFECT
// #define ADD_AMP_EFFECT
#define ADD_EQ_EFFECT
#define ADD_MOD_EFFECT
#define ADD_DEL_EFFECT
#define ADD_REV_EFFECT

#include "../include/Application.h"

static constexpr uint32 TOTAL_SDRAM_SIZE = 64 * 1024 * 1024;
uint8 DSY_SDRAM_BSS g_SDRAMBuffer[TOTAL_SDRAM_SIZE];

int main()
{
	Application app(g_SDRAMBuffer, TOTAL_SDRAM_SIZE);

	app.Initialize();

	while (true)
	{
		app.Update();
	}

	return 0;
}

//  Looper (It needs to have a normalize method and an option to disable auto nomalizations (* 0.5))
// Add expression
//  Fonts
//  Preset Variation like P1A P1B P1C

// #include "../include/framework/DSP/Filters./WaveShaperFilter.h"

// int main()
// {
// 	WaveShaperFilter<float> waveShaper;

// 	static typename WaveShaperFilter<float>::TablePoints points[] = {{-1, -0.8}, {-0.6, -0.8}, {0, 0}, {0.4, 1}, {1, 1}};
// 	waveShaper.SetTable(points, 5);

// 	float value = waveShaper.Process(-1.5);
// 	value = waveShaper.Process(-1);
// 	value = waveShaper.Process(-0.8);
// 	value = waveShaper.Process(-0.6);
// 	value = waveShaper.Process(0);
// 	value = waveShaper.Process(0.1);
// 	value = waveShaper.Process(0.4);
// 	value = waveShaper.Process(0.4);
// 	value = waveShaper.Process(0.6);
// 	value = waveShaper.Process(1);
// 	value = waveShaper.Process(1.2);
// }