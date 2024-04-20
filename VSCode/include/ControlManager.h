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

#define PROCESS_RATE 100

class ControlManager
{
public:
	ControlManager(IHAL *HAL)
		: m_HAL(HAL),
		  m_UsedGPIOs{}
	{
	}

	void Update(void)
	{
		m_Factory.Process();
	}

	SingleLED *CreateSingleLED(const char *Name, GPIOPins Pin)
	{
		CheckIfGPIOIsUsed((uint8)Pin);

		MarkGPIOAsUsed((uint8)Pin);

		Log::WriteInfo("Controls", "%s: LED %i", Name, Pin);

		return m_Factory.Create<SingleLED>(m_HAL, (uint8)Pin, PROCESS_RATE);
	}
	DualLED *CreateDualLED(const char *Name, GPIOPins RedPin, GPIOPins GreenPin)
	{
		CheckIfGPIOIsUsed((uint8)RedPin);
		CheckIfGPIOIsUsed((uint8)GreenPin);

		MarkGPIOAsUsed((uint8)RedPin);
		MarkGPIOAsUsed((uint8)GreenPin);

		Log::WriteInfo("Controls", "%s: LED %i", Name, RedPin);
		Log::WriteInfo("Controls", "%s: LED %i", Name, GreenPin);

		return m_Factory.Create<DualLED>(m_HAL, (uint8)RedPin, (uint8)GreenPin, PROCESS_RATE);
	}
	TripleLED *CreateTripleLED(const char *Name, GPIOPins RedPin, GPIOPins GreenPin, GPIOPins BluePin)
	{
		CheckIfGPIOIsUsed((uint8)RedPin);
		CheckIfGPIOIsUsed((uint8)GreenPin);
		CheckIfGPIOIsUsed((uint8)BluePin);

		MarkGPIOAsUsed((uint8)RedPin);
		MarkGPIOAsUsed((uint8)GreenPin);
		MarkGPIOAsUsed((uint8)BluePin);

		Log::WriteInfo("Controls", "%s: LED %i", Name, RedPin);
		Log::WriteInfo("Controls", "%s: LED %i", Name, GreenPin);
		Log::WriteInfo("Controls", "%s: LED %i", Name, BluePin);

		return m_Factory.Create<TripleLED>(m_HAL, (uint8)RedPin, (uint8)GreenPin, (uint8)BluePin, PROCESS_RATE);
	}

	Button *CreateButton(const char *Name, GPIOPins Pin)
	{
		CheckIfGPIOIsUsed((uint8)Pin);

		MarkGPIOAsUsed((uint8)Pin);

		Log::WriteInfo("Controls", "%s: Button %i", Name, Pin);

		return m_Factory.Create<Button>(m_HAL, (uint8)Pin, PROCESS_RATE);
	}

	Switch *CreateSwitch(const char *Name, GPIOPins Pin)
	{
		CheckIfGPIOIsUsed((uint8)Pin);

		MarkGPIOAsUsed((uint8)Pin);

		Log::WriteInfo("Controls", "%s: Switch %i", Name, Pin);

		return m_Factory.Create<Switch>(m_HAL, (uint8)Pin, PROCESS_RATE);
	}

	Potentiometer *CreatePotentiometer(const char *Name, AnalogPins Pin)
	{
		CheckIfGPIOIsUsed((uint8)Pin);

		MarkGPIOAsUsed((uint8)Pin);

		Log::WriteInfo("Controls", "%s: Pot %i", Name, Pin);

		return m_Factory.Create<Potentiometer>(m_HAL, (uint8)Pin, PROCESS_RATE, true);
	}

private:
	void CheckIfGPIOIsUsed(uint8 Pin)
	{
		ASSERT(!m_UsedGPIOs[Pin], "GPIOPin%i is already in use", Pin);
	}
	void MarkGPIOAsUsed(uint8 Pin)
	{
		m_UsedGPIOs[Pin] = true;
	}

private:
	IHAL *m_HAL;
	ControlFactory m_Factory;
	bool m_UsedGPIOs[(uint8)GPIOPins::COUNT];
};

#endif