#include "ControlManager.h"
#include <framework/include/Controls/LED.h>
#include <framework/include/Controls/Switch.h>
#include <framework/include/Controls/Potentiometer.h>

ControlManager::ControlManager(void)
	: m_UsedGPIOs()
{
}

LED *ControlManager::CreateLED(const char *Name, GPIOPins Pin)
{
	CheckIfGPIOIsUsed(Pin);

	MarkGPIOAsUsed(Pin);

	Log::WriteInfo("Controls", "%s: LED %i", Name, Pin);

	return m_Factory.Create<LED>(Pin);
}

Switch *ControlManager::CreateSwitch(const char *Name, GPIOPins Pin)
{
	CheckIfGPIOIsUsed(Pin);

	MarkGPIOAsUsed(Pin);

	Log::WriteInfo("Controls", "%s: Switch %i", Name, Pin);

	return m_Factory.Create<Switch>(Pin);
}

Potentiometer *ControlManager::CreatePotentiometer(const char *Name, GPIOPins Pin)
{
	CheckIfGPIOIsUsed(Pin);

	MarkGPIOAsUsed(Pin);

	Log::WriteInfo("Controls", "%s: Pot %i", Name, Pin);

	return m_Factory.Create<Potentiometer>(Pin);
}

void ControlManager::CheckIfGPIOIsUsed(GPIOPins Pin)
{
	ASSERT(!m_UsedGPIOs[(uint32)Pin], "GPIOPin%i is already in use", (uint32)Pin);
}

void ControlManager::MarkGPIOAsUsed(GPIOPins Pin)
{
	m_UsedGPIOs[(uint32)Pin] = true;
}