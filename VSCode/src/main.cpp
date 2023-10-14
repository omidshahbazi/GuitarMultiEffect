#include "Application.h"

Application g_App;

void setup()
{
	g_App.Initialize();
}

void loop()
{
	g_App.Update();
}
