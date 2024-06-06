// #define ENABLE_TYPE_CHECK
#define USE_BREAK_ONLY_ASSERT
// #define WAIT_FOR_DEBUGGER
#define USE_DISPLAY

#ifndef USE_SRAM
#undef USE_DISPLAY
#endif

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

// Mod?
// Amp?
// Delay
// Looper
// Drum Machine (Daisy has an example)
// TODO: Preset Variation like P1A P1B P1C