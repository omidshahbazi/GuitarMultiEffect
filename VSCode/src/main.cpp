#ifdef __PLATFORMIO_BUILD_DEBUG__
#define DEBUG
#endif

#include "Application.h"

Application g_App;

void setup()
{
	g_App.Initialize();
}

void loop()
{
}