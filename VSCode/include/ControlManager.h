#pragma once
#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include <framework/include/DSP/Controls/ControlFactory.h>
#include <framework/include/Common.h>
#include <framework/include/DSP/Controls/SingleLED.h>
#include <framework/include/DSP/Controls/DualLED.h>
#include <framework/include/DSP/Controls/TripleLED.h>
#include <framework/include/DSP/Controls/Button.h>
#include <framework/include/DSP/Controls/Switch.h>
#include <framework/include/DSP/Controls/Potentiometer.h>
#include <framework/include/Task.h>

#define PROCESS_RATE 10

class ControlManager
{
public:
	ControlManager(IHAL *HAL)
		: m_HAL(HAL),
		  m_UsedGPIOs()
	{
		Task::Create(
			[&]()
			{
				while (true)
				{
					m_HAL->Delay(1);

					m_Factory.Process();
				}
			},
			2048, "ControlsTask", 0, 1);
	}

	SingleLED *CreateSingleLED(const char *Name, GPIOPins Pin)
	{
		CheckIfGPIOIsUsed(Pin);

		MarkGPIOAsUsed(Pin);

		Log::WriteInfo("Controls", "%s: LED %i", Name, Pin);

		return m_Factory.Create<SingleLED>(m_HAL, (uint8)Pin, PROCESS_RATE);
	}

	DualLED *CreateDualLED(const char *Name, GPIOPins RedPin, GPIOPins GreenPin)
	{
		CheckIfGPIOIsUsed(RedPin);
		CheckIfGPIOIsUsed(GreenPin);

		MarkGPIOAsUsed(RedPin);
		MarkGPIOAsUsed(GreenPin);

		Log::WriteInfo("Controls", "%s: LED %i", Name, RedPin);
		Log::WriteInfo("Controls", "%s: LED %i", Name, GreenPin);

		return m_Factory.Create<DualLED>(m_HAL, (uint8)RedPin, (uint8)GreenPin, PROCESS_RATE);
	}

	TripleLED *CreateTripleLED(const char *Name, GPIOPins RedPin, GPIOPins GreenPin, GPIOPins BluePin)
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

	Button *CreateButton(const char *Name, GPIOPins Pin)
	{
		CheckIfGPIOIsUsed(Pin);

		MarkGPIOAsUsed(Pin);

		Log::WriteInfo("Controls", "%s: Button %i", Name, Pin);

		return m_Factory.Create<Button>(m_HAL, (uint8)Pin, PROCESS_RATE);
	}

	Switch *CreateSwitch(const char *Name, GPIOPins Pin)
	{
		CheckIfGPIOIsUsed(Pin);

		MarkGPIOAsUsed(Pin);

		Log::WriteInfo("Controls", "%s: Switch %i", Name, Pin);

		return m_Factory.Create<Switch>(m_HAL, (uint8)Pin, PROCESS_RATE);
	}

	Potentiometer *CreatePotentiometer(const char *Name, GPIOPins Pin)
	{
		CheckIfGPIOIsUsed(Pin);

		MarkGPIOAsUsed(Pin);

		Log::WriteInfo("Controls", "%s: Pot %i", Name, Pin);

		return m_Factory.Create<Potentiometer>(m_HAL, (uint8)Pin, PROCESS_RATE, true);
	}

private:
	void CheckIfGPIOIsUsed(GPIOPins Pin)
	{
		ASSERT(!m_UsedGPIOs[(uint32)Pin], "GPIOPin%i is already in use", (uint32)Pin);
	}

	void MarkGPIOAsUsed(GPIOPins Pin)
	{
		m_UsedGPIOs[(uint32)Pin] = true;
	}

private:
	IHAL *m_HAL;
	ControlFactory m_Factory;
	bool m_UsedGPIOs[(uint8)GPIOPins::COUNT];
};

#endif