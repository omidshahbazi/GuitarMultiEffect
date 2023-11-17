#include "ControlManager.h"
#include "framework/include/Controls/LED.h"
#include "framework/include/Controls/PushButtonArray.h"
#include "framework/include/Controls/Potentiometer.h"

ControlManager::ControlManager(GPIOPins PushButtonArray1Pin)
	: m_PushButtonArray1(nullptr)
{
	m_PushButtonArray1 = m_Factory.Create<PushButtonArray>(PushButtonArray1Pin, 1);
}

LED *ControlManager::CreateLED(GPIOPins Pin)
{
	return m_Factory.Create<LED>(Pin);
}

bool ControlManager::BindToPushButton(uint8 Index, ButtonPressedEventHandler &&OnPressed)
{
	if (OnPressed == nullptr)
		return false;

	m_PushButtonArray1->Bind(Index, nullptr, nullptr,
							 [&]()
							 {
								 OnPressed();
							 });

	return true;
}

Potentiometer *ControlManager::CreatePotentiometer(GPIOPins Pin)
{
	return m_Factory.Create<Potentiometer>(Pin);
}