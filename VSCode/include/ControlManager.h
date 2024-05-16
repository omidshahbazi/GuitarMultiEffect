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

class ControlManager
{
public:
	ControlManager(IHAL *HAL)
		: m_Factory(HAL)
	{
		Task::Create(
			[&]()
			{
				while (true)
					m_Factory.Process();
			},
			2048, "ControlsTask", 0, 1);
	}

	SingleLED *CreateSingleLED(const char *Name, GPIOPins Pin)
	{
		Log::WriteInfo("Controls", "%s: LED %i", Name, Pin);

		return m_Factory.CreateSingleLED((uint8)Pin);
	}

	DualLED *CreateDualLED(const char *Name, GPIOPins RedPin, GPIOPins GreenPin)
	{
		Log::WriteInfo("Controls", "%s: LED %i", Name, RedPin);
		Log::WriteInfo("Controls", "%s: LED %i", Name, GreenPin);

		return m_Factory.CreateDualLED((uint8)RedPin, (uint8)GreenPin);
	}

	TripleLED *CreateTripleLED(const char *Name, GPIOPins RedPin, GPIOPins GreenPin, GPIOPins BluePin)
	{
		Log::WriteInfo("Controls", "%s: LED %i", Name, RedPin);
		Log::WriteInfo("Controls", "%s: LED %i", Name, GreenPin);
		Log::WriteInfo("Controls", "%s: LED %i", Name, BluePin);

		return m_Factory.CreateTripleLED((uint8)RedPin, (uint8)GreenPin, (uint8)BluePin);
	}

	Button *CreateButton(const char *Name, GPIOPins Pin)
	{
		Log::WriteInfo("Controls", "%s: Button %i", Name, Pin);

		return m_Factory.CreateButton((uint8)Pin);
	}

	Switch *CreateSwitch(const char *Name, GPIOPins Pin)
	{
		Log::WriteInfo("Controls", "%s: Switch %i", Name, Pin);

		return m_Factory.CreateSwitch((uint8)Pin);
	}

	Potentiometer *CreatePotentiometer(const char *Name, GPIOPins Pin)
	{
		Log::WriteInfo("Controls", "%s: Pot %i", Name, Pin);

		return m_Factory.CreatePotentiometer((uint8)Pin, true);
	}

private:
	ControlFactory<(uint8)GPIOPins::COUNT, 10> m_Factory;
};

#endif