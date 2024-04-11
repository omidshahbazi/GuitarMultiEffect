#include "ControlManager.h"
#include <framework/include/DSP/Controls/LED.h>
#include <framework/include/DSP/Controls/Switch.h>
#include <framework/include/DSP/Controls/Potentiometer.h>
#include <framework/include/Task.h>

#define PROCESS_RATE 10

ControlManager::ControlManager(IHAL *HAL)
	: m_HAL(HAL),
	  m_UsedGPIOs()
{
	Task::Create(
		[&]()
		{
			while (true)
			{
				Task::Delay(1);

				m_Factory.Process();
			}
		},
		2048, "ControlsTask", 0, 1);
}

LED *ControlManager::CreateLED(const char *Name, GPIOPins Pin)
{
	CheckIfGPIOIsUsed(Pin);

	MarkGPIOAsUsed(Pin);

	Log::WriteInfo("Controls", "%s: LED %i", Name, Pin);

	return m_Factory.Create<LED>(m_HAL, (uint8)Pin, PROCESS_RATE);
}

Switch *ControlManager::CreateSwitch(const char *Name, GPIOPins Pin)
{
	CheckIfGPIOIsUsed(Pin);

	MarkGPIOAsUsed(Pin);

	Log::WriteInfo("Controls", "%s: Switch %i", Name, Pin);

	return m_Factory.Create<Switch>(m_HAL, (uint8)Pin, PROCESS_RATE);
}

Potentiometer *ControlManager::CreatePotentiometer(const char *Name, GPIOPins Pin)
{
	CheckIfGPIOIsUsed(Pin);

	MarkGPIOAsUsed(Pin);

	Log::WriteInfo("Controls", "%s: Pot %i", Name, Pin);

	return m_Factory.Create<Potentiometer>(m_HAL, (uint8)Pin, PROCESS_RATE);
}

void ControlManager::CheckIfGPIOIsUsed(GPIOPins Pin)
{
	ASSERT(!m_UsedGPIOs[(uint32)Pin], "GPIOPin%i is already in use", (uint32)Pin);
}

void ControlManager::MarkGPIOAsUsed(GPIOPins Pin)
{
	m_UsedGPIOs[(uint32)Pin] = true;
}