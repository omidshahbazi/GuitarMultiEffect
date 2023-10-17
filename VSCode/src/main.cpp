#include "Application.h"
#include <Arduino.h>

Application g_App;

void setup()
{
	Serial.begin(115200);

	g_App.Initialize();
}

void loop()
{
	g_App.Update();
}
