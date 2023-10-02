#include "Application.h"

Application g_App;

void setup()
{  
    Serial.begin(115200);
    Serial.print("TEST\n");
    g_App.Initialize();
}

void loop()
{
    g_App.Update();
}
