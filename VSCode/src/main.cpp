#include "Application.h"
#include <Arduino.h>

Application g_App;

void setup()
{
	g_App.Initialize();

	Serial.begin(115200);
}

void loop()
{
}