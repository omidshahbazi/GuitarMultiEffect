// #define ENABLE_TYPE_CHECK
#define USE_BREAK_ONLY_ASSERT
// #define WAIT_FOR_DEBUGGER

#pragma GCC push_options
#pragma GCC optimize("Os")

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

// SaveScreen with reset factory, A separated button?
// Enabled disable using the same button for Looper
// Add new effects
// Rename Screen
// Preset volume
// Looper
// Drum Machine
// TODO: Preset Variation