#include "ControlManager.h"
#include <framework/include/DSP/Controls/SingleLED.h>
#include <framework/include/DSP/Controls/DualLED.h>
#include <framework/include/DSP/Controls/TripleLED.h>
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

SingleLED *ControlManager::CreateSingleLED(const char *Name, GPIOPins Pin)
{
	CheckIfGPIOIsUsed(Pin);

	MarkGPIOAsUsed(Pin);

	Log::WriteInfo("Controls", "%s: LED %i", Name, Pin);

	return m_Factory.Create<SingleLED>(m_HAL, (uint8)Pin, PROCESS_RATE);
}

DualLED *ControlManager::CreateDualLED(const char *Name, GPIOPins RedPin, GPIOPins GreenPin)
{
	CheckIfGPIOIsUsed(RedPin);
	CheckIfGPIOIsUsed(GreenPin);

	MarkGPIOAsUsed(RedPin);
	MarkGPIOAsUsed(GreenPin);

	Log::WriteInfo("Controls", "%s: LED %i", Name, RedPin);
	Log::WriteInfo("Controls", "%s: LED %i", Name, GreenPin);

	return m_Factory.Create<DualLED>(m_HAL, (uint8)RedPin, (uint8)GreenPin, PROCESS_RATE);
}

TripleLED *ControlManager::CreateTripleLED(const char *Name, GPIOPins RedPin, GPIOPins GreenPin, GPIOPins BluePin)
{
	CheckIfGPIOIsUsed(RedPin);
	CheckIfGPIOIsUsed(GreenPin);
	CheckIfGPIOIsUsed(BluePin);

	MarkGPIOAsUsed(RedPin);
	MarkGPIOAsUsed(GreenPin);
	MarkGPIOAsUsed(BluePin);

	Log::WriteInfo("Controls", "%s: LED %i", Name, RedPin);
	Log::WriteInfo("Controls", "%s: LED %i", Name, GreenPin);
	Log::WriteInfo("Controls", "%s: LED %i", Name, BluePin);

	return m_Factory.Create<TripleLED>(m_HAL, (uint8)RedPin, (uint8)GreenPin, (uint8)BluePin, PROCESS_RATE);
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

	return m_Factory.Create<Potentiometer>(m_HAL, (uint8)Pin, PROCESS_RATE, true);
}

void ControlManager::CheckIfGPIOIsUsed(GPIOPins Pin)
{
	ASSERT(!m_UsedGPIOs[(uint32)Pin], "GPIOPin%i is already in use", (uint32)Pin);
}

void ControlManager::MarkGPIOAsUsed(GPIOPins Pin)
{
	m_UsedGPIOs[(uint32)Pin] = true;
}