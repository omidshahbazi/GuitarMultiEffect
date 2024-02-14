#pragma once
#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include <framework/include/Controls/ControlFactory.h>
#include <functional>

class LED;
class Switch;
class Potentiometer;

class ControlManager
{
public:
	typedef std::function<void(void)> ButtonPressedEventHandler;

public:
	ControlManager(void);

	LED *CreateLED(const char *Name, GPIOPins Pin);

	Switch *CreateSwitch(const char *Name, GPIOPins Pin);

	Potentiometer *CreatePotentiometer(const char *Name, GPIOPins Pin);

private:
	void CheckIfGPIOIsUsed(GPIOPins Pin);
	void MarkGPIOAsUsed(GPIOPins Pin);

private:
	ControlFactory m_Factory;
	bool m_UsedGPIOs[GPIO_NUM_MAX];
};

#endif