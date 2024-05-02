#pragma once
#ifndef EFFECT_H
#define EFFECT_H

#include "../ControlManager.h"
#include "../framework/Common.h"
#include "../framework/DSP/DSPs/IDSP.h"

template <typename T>
class Effect
{
public:
	Effect(ControlManager *ControlManager, GPIOPins RedLEDPin, GPIOPins GreenLEDPin, GPIOPins EnableButtonPin)
		: m_Enabled(false)
	{
		m_EnabledLED = ControlManager->CreateDualLED("Enabled", RedLEDPin, GreenLEDPin);
		m_EnabledLED->SetColor(DUAL_LED_GREEN);
		m_EnabledLED->SetConstantBrighness(1);

		m_EnabledSwitch = ControlManager->CreateSwitch("Enabled", EnableButtonPin);
		m_EnabledSwitch->SetOnTurnedOnListener([this](void)
											   { OnButtonDown(); });

		m_EnabledSwitch->SetOnTurnedOffListener([&](float heldTime)
												{ OnButtonUp(heldTime); });

		SetEnabled(true);
	}

	void Apply(T *Buffer, uint16 Count)
	{
		if (!m_Enabled)
			return;

		GetDSP()->ProcessBuffer(Buffer, Count);
	}

protected:
	void ChangeEnabledLEDBlinkingRate(float Rate = 1)
	{
		if (!m_Enabled)
			return;

		m_EnabledLED->SetBlinkingBrighness(1, Rate);
	}

	virtual void SetEnabled(bool Value)
	{
		m_Enabled = Value;

		if (m_Enabled)
			ChangeEnabledLEDBlinkingRate();
		else
			m_EnabledLED->SetConstantBrighness(0);
	}

	virtual void OnButtonDown(void)
	{
	}

	virtual void OnButtonUp(float HeldTime)
	{
		m_Enabled = !m_Enabled;

		SetEnabled(m_Enabled);
	}

	virtual IDSP<T> *GetDSP(void) = 0;

private:
	bool m_Enabled;
	DualLED *m_EnabledLED;
	Switch *m_EnabledSwitch;
};

#endif