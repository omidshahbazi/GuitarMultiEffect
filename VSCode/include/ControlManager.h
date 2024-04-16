#pragma once
#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include "framework/DSP/Controls/ControlFactory.h"
#include "framework/Common.h"

class SingleLED;
class DualLED;
class TripleLED;
class Button;
class Switch;
class Potentiometer;

class ControlManager
{
public:
	ControlManager(IHAL *HAL);

	void Update(void)
	{
		m_Factory.Process();
	}

	SingleLED *CreateSingleLED(const char *Name, GPIOPins Pin);
	DualLED *CreateDualLED(const char *Name, GPIOPins RedPin, GPIOPins GreenPin);
	TripleLED *CreateTripleLED(const char *Name, GPIOPins RedPin, GPIOPins GreenPin, GPIOPins BluePin);

	Button *CreateButton(const char *Name, GPIOPins Pin);

	Switch *CreateSwitch(const char *Name, GPIOPins Pin);

	Potentiometer *CreatePotentiometer(const char *Name, GPIOPins Pin);

private:
	void CheckIfGPIOIsUsed(GPIOPins Pin);
	void MarkGPIOAsUsed(GPIOPins Pin);

private:
	IHAL *m_HAL;
	ControlFactory m_Factory;
	bool m_UsedGPIOs[(uint8)GPIOPins::COUNT];
};

#endif