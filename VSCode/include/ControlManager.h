#pragma once
#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include <framework/include/DSP/Controls/ControlFactory.h>
#include <framework/include/Common.h>
#include <functional>

class SingleLED;
class DualLED;
class TripleLED;
class Switch;
class Potentiometer;

class ControlManager
{
public:
	typedef std::function<void(void)> ButtonPressedEventHandler;

public:
	ControlManager(IHAL *HAL);

	SingleLED *CreateSingleLED(const char *Name, GPIOPins Pin);
	DualLED *CreateDualLED(const char *Name, GPIOPins RedPin, GPIOPins GreenPin);
	TripleLED *CreateTripleLED(const char *Name, GPIOPins RedPin, GPIOPins GreenPin, GPIOPins BluePin);

	Switch *CreateSwitch(const char *Name, GPIOPins Pin);

	Potentiometer *CreatePotentiometer(const char *Name, GPIOPins Pin);

private:
	void CheckIfGPIOIsUsed(GPIOPins Pin);
	void MarkGPIOAsUsed(GPIOPins Pin);

private:
	IHAL *m_HAL;
	ControlFactory m_Factory;
	bool m_UsedGPIOs[GPIO_NUM_MAX];
};

#endif