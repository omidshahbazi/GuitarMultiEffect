// #define ENABLE_TYPE_CHECK

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