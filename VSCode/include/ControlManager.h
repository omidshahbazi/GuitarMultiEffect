#pragma once
#ifndef CONTROL_MANAGER_H
#define CONTROL_MANAGER_H

#include <framework/include/Controls/ControlFactory.h>
#include <functional>

class LED;
class PushButtonArray;
class Potentiometer;

class ControlManager
{
public:
	typedef std::function<void(void)> ButtonPressedEventHandler;

public:
	ControlManager(GPIOPins PushButtonArray1Pin);

	LED *CreateLED(GPIOPins Pin);

	bool BindToPushButton(uint8 Index, ButtonPressedEventHandler &&OnPressed);

	Potentiometer *CreatePotentiometer(GPIOPins Pin);

private:
	void CheckIfGPIOIsUsed(GPIOPins Pin);
	void MarkGPIOAsUsed(GPIOPins Pin);

private:
	ControlFactory m_Factory;
	PushButtonArray *m_PushButtonArray1;
	bool m_UsedGPIOs[GPIO_NUM_MAX];
};

#endif