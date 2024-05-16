#pragma once
#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include "framework/DSP/Controls/ControlFactory.h"
#include "framework/Common.h"
#include "framework/DSP/Controls/SingleLED.h"
#include "framework/DSP/Controls/DualLED.h"
#include "framework/DSP/Controls/TripleLED.h"
#include "framework/DSP/Controls/Button.h"
#include "framework/DSP/Controls/Switch.h"
#include "framework/DSP/Controls/Potentiometer.h"
#include "framework/DSP/Controls/RotaryButton.h"

class ControlManager
{
public:
	ControlManager(IHAL *HAL)
		: m_Factory(HAL)
	{
	}

	void Update(void)
	{
		m_Factory.Process();
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

	Potentiometer *CreatePotentiometer(const char *Name, AnalogPins Pin)
	{
		Log::WriteInfo("Controls", "%s: Pot %i", Name, Pin);

		return m_Factory.CreatePotentiometer((uint8)Pin, false);
	}

	Rotary *CreateRotary(const char *Name, GPIOPins LeftPin, GPIOPins RightPin)
	{
		Log::WriteInfo("Controls", "%s: Rotary %i", Name, LeftPin);
		Log::WriteInfo("Controls", "%s: Rotary %i", Name, RightPin);

		return m_Factory.CreateRotary((uint8)LeftPin, (uint8)RightPin);
	}

	RotaryButton *CreateRotaryButton(const char *Name, GPIOPins LeftPin, GPIOPins RightPin, GPIOPins ButtonPin)
	{
		Log::WriteInfo("Controls", "%s: RotaryButton %i", Name, LeftPin);
		Log::WriteInfo("Controls", "%s: RotaryButton %i", Name, RightPin);
		Log::WriteInfo("Controls", "%s: RotaryButton %i", Name, ButtonPin);

		return m_Factory.CreateRotaryButton((uint8)LeftPin, (uint8)RightPin, (uint8)ButtonPin);
	}

private:
	ControlFactory<(uint8)GPIOPins::COUNT, 100> m_Factory;
};

#endif