#include "ControlManager.h"
#include <framework/include/Controls/LED.h>
#include <framework/include/Controls/Switch.h>
#include <framework/include/Controls/Potentiometer.h>

ControlManager::ControlManager(void)
	: m_UsedGPIOs()
{
}

LED *ControlManager::CreateLED(GPIOPins Pin)
{
	CheckIfGPIOIsUsed(Pin);

	MarkGPIOAsUsed(Pin);

	return m_Factory.Create<LED>(Pin);
}

Switch *ControlManager::CreateSwitch(GPIOPins Pin)
{
	CheckIfGPIOIsUsed(Pin);

	MarkGPIOAsUsed(Pin);

	return m_Factory.Create<Switch>(Pin);
}

Potentiometer *ControlManager::CreatePotentiometer(GPIOPins Pin)
{
	CheckIfGPIOIsUsed(Pin);

	MarkGPIOAsUsed(Pin);

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