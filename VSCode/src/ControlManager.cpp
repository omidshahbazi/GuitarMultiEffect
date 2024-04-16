#include "../include/ControlManager.h"
#include "../include/framework/DSP/Controls/SingleLED.h"
#include "../include/framework/DSP/Controls/DualLED.h"
#include "../include/framework/DSP/Controls/TripleLED.h"
#include "../include/framework/DSP/Controls/Button.h"
#include "../include/framework/DSP/Controls/Switch.h"
#include "../include/framework/DSP/Controls/Potentiometer.h"

#define PROCESS_RATE 10

ControlManager::ControlManager(IHAL *HAL)
	: m_HAL(HAL),
	  m_UsedGPIOs()
{
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

Button *ControlManager::CreateButton(const char *Name, GPIOPins Pin)
{
	CheckIfGPIOIsUsed(Pin);

	MarkGPIOAsUsed(Pin);

	Log::WriteInfo("Controls", "%s: Button %i", Name, Pin);

	return m_Factory.Create<Button>(m_HAL, (uint8)Pin, PROCESS_RATE);
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