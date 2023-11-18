#include "ControlManager.h"
#include "framework/include/Controls/LED.h"
#include "framework/include/Controls/PushButtonArray.h"
#include "framework/include/Controls/Potentiometer.h"

ControlManager::ControlManager(GPIOPins PushButtonArray1Pin)
	: m_PushButtonArray1(nullptr),
	  m_UsedGPIOs({})
{
	CheckIfGPIOIsUsed(PushButtonArray1Pin);
	m_PushButtonArray1 = m_Factory.Create<PushButtonArray>(PushButtonArray1Pin, 1);
}

LED *ControlManager::CreateLED(GPIOPins Pin)
{
	CheckIfGPIOIsUsed(Pin);

	return m_Factory.Create<LED>(Pin);
}

bool ControlManager::BindToPushButton(uint8 Index, ButtonPressedEventHandler &&OnPressed)
{
	if (OnPressed == nullptr)
		return false;

	m_PushButtonArray1->Bind(Index, nullptr, nullptr, std::forward<ButtonPressedEventHandler>(OnPressed));

	return true;
}

Potentiometer *ControlManager::CreatePotentiometer(GPIOPins Pin)
{
	CheckIfGPIOIsUsed(Pin);

	return m_Factory.Create<Potentiometer>(Pin);
}

void ControlManager::CheckIfGPIOIsUsed(GPIOPins Pin)
{
	ASSERT(!m_UsedGPIOs[(uint32)Pin], "ControlManager", "GPIOPin%i is already in use", (uint32)Pin);
}

void ControlManager::MarkGPIOAsUsed(GPIOPins Pin)
{
	m_UsedGPIOs[(uint32)Pin] = true;
}